
//
// vmm.c -- Defines the functions related to paging and virtual memory management.
//          Written for JamesM's kernel development tutorials.
//

#include "paging.h"
#include "idt.h"
#include "common.h"
#include "monitor.h"
#include "multiboot.h"

uint32_t heap_max = HEAP_START;
header_t *heap_first = 0;

uint32_t pmm_stack_loc = PMM_STACK_ADDR;
uint32_t pmm_stack_max = PMM_STACK_ADDR;
uint32_t pmm_location;
char pmm_paging_active = 0;

uint32_t *page_directory = (uint32_t *)PAGE_DIR_VIRTUAL_ADDR;
uint32_t *page_tables = (uint32_t *)PAGE_TABLE_VIRTUAL_ADDR;

page_directory_t *current_directory;

extern char pmm_paging_active;

void page_fault (registers_t *regs);

void init_paging(multiboot_t *mboot_ptr)
{
  // Ensure the initial page allocation location is page-aligned.
  pmm_location = (mboot_ptr->mem_upper + 0x1000) & PAGE_MASK;

  // Register the page fault handler.
  register_interrupt_handler (14, &page_fault);

  // Create a page directory.
  page_directory_t *pd = (page_directory_t*)pmm_alloc_page ();
  
  // Initialise it.
  memset (pd, 0, 0x1000);

  // Identity map the first 4 MB.
  pd[0] = pmm_alloc_page () | PAGE_PRESENT | PAGE_WRITE;
  uint32_t *pt = (uint32_t*) (pd[0] & PAGE_MASK);
  for (int i = 0; i < 1024; i++)
    pt[i] = i*0x1000 | PAGE_PRESENT | PAGE_WRITE;

  // Assign the second-last table and zero it.
  pd[1022] = pmm_alloc_page () | PAGE_PRESENT | PAGE_WRITE;
  pt = (uint32_t*) (pd[1022] & PAGE_MASK);
  memset (pt, 0, 0x1000);
  
  // The last entry of the second-last table is the directory itself.
  pt[1023] = (uint32_t)pd | PAGE_PRESENT | PAGE_WRITE;

  // The last table loops back on the directory itself.
  pd[1023] = (uint32_t)pd | PAGE_PRESENT | PAGE_WRITE;

  // Set the current directory.
  switch_page_directory (pd);

  // Enable paging.
  uint32_t cr0;
  asm volatile ("mov %%cr0, %0" : "=r" (cr0));
  cr0 |= 0x80000000;
  asm volatile ("mov %0, %%cr0" : : "r" (cr0));

  // We need to map the page table where the physical memory manager keeps its page stack
  // else it will panic on the first "pmm_free_page".
  uint32_t pt_idx = PAGE_DIR_IDX((PMM_STACK_ADDR>>12));
  page_directory[pt_idx] = pmm_alloc_page () | PAGE_PRESENT | PAGE_WRITE;
  memset (page_tables[pt_idx*1024], 0, 0x1000);

  // Paging is now active. Tell the physical memory manager.
  pmm_paging_active = 1;
  
  
  // Find all the usable areas of memory and inform the physical memory manager about them.
  uint32_t i = mboot_ptr->mmap_addr;
  while (i < mboot_ptr->mmap_addr + mboot_ptr->mmap_length)
  {
    mmap_entry_t *me = (mmap_entry_t*) i;

    // Does this entry specify usable RAM?
    if (me->type == 1)
    {
      uint32_t j;
      // For every page in this entry, add to the free page stack.
      for (j = me->base_addr_low; j < me->base_addr_low+me->length_low; j += 0x1000)
      {
        pmm_free_page (j);
      }
    }

    // The multiboot specification is strange in this respect - the size member does not include "size" itself in its calculations,
    // so we must add sizeof (uint32_t).
    i += me->size + sizeof (uint32_t);
  }

}

uint32_t pmm_alloc_page ()
{
  if (pmm_paging_active)
  {
    // Quick sanity check.
    if (pmm_stack_loc == PMM_STACK_ADDR)
      PANIC("Error:out of memory.");

    // Pop off the stack.
    pmm_stack_loc -= sizeof (uint32_t);
    uint32_t *stack = (uint32_t*)pmm_stack_loc;
  
    return *stack;
  }
  else
  {
    return pmm_location += 0x1000;
  }
}

void pmm_free_page (uint32_t p)
{
  // Ignore any page under "location", as it may contain important data initialised
  // at boot (like paging structures!)
  if (p < pmm_location) return;

  // If we've run out of space on the stack...
  if (pmm_stack_max <= pmm_stack_loc)
  {
    // Map the page we're currently freeing at the top of the free page stack.
    map (pmm_stack_max, p, PAGE_PRESENT | PAGE_WRITE);
    // Increase the free page stack's size by one page.
    pmm_stack_max += 4096;
  }
  else
  {
    // Else we have space on the stack, so push.
    uint32_t *stack = (uint32_t*)pmm_stack_loc;
    *stack = p;
    pmm_stack_loc += sizeof (uint32_t);
  }
}

void switch_page_directory (page_directory_t *pd)
{
  current_directory = pd;
  asm volatile ("mov %0, %%cr3" : : "r" (pd));
}

void map (uint32_t va, uint32_t pa, uint32_t flags)
{
  uint32_t virtual_page = va / 0x1000;
  uint32_t pt_idx = PAGE_DIR_IDX(virtual_page);

  // Find the appropriate page table for 'va'.
  if (page_directory[pt_idx] == 0)
  {
    // The page table holding this page has not been created yet.
    page_directory[pt_idx] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;
    memset (page_tables[pt_idx*1024], 0, 0x1000);
  }

  // Now that the page table definately exists, we can update the PTE.
  page_tables[virtual_page] = (pa & PAGE_MASK) | flags;
}

void unmap (uint32_t va)
{
  uint32_t virtual_page = va / 0x1000;
  
  page_tables[virtual_page] = 0;
  // Inform the CPU that we have invalidated a page mapping.
  asm volatile ("invlpg (%0)" : : "a" (va));
}

char get_mapping (uint32_t va, uint32_t *pa)
{
  uint32_t virtual_page = va / 0x1000;
  uint32_t pt_idx = PAGE_DIR_IDX(virtual_page);

  // Find the appropriate page table for 'va'.
  if (!page_directory[pt_idx] || !page_tables[virtual_page])
    return 0;

  if (pa) 
    *pa = page_tables[virtual_page] & PAGE_MASK;
    
  return 1;
}

void page_fault(registers_t *regs)
{
    // Output an error message.
    monitor_write("Page fault! ( ");
    
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    monitor_write((regs->err_code & 0x1) ? ""            :"present ");
    monitor_write((regs->err_code & 0x2) ? "read-only "  :"");
    monitor_write((regs->err_code & 0x4) ? "user-mode "  :"");
    monitor_write((regs->err_code & 0x8) ? "reserved "   :"");
    monitor_write((regs->err_code & 0x10)? "idied "      :"");

    monitor_write(") at 0x");
    monitor_write_hex(faulting_address);
    monitor_write(" - EIP: ");
    monitor_write_hex(regs->eip);
    monitor_write("\n");
    
    PANIC("Page fault");
}



