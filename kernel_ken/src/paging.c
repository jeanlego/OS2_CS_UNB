// paging.c -- Defines the interface for and structures relating to paging.
//             Written for JamesM's kernel development tutorials.

#include "paging.h"
#include "kheap.h"
#include "monitor.h"

// The kernel's page directory
page_directory_t *kernel_directory=0;

// The current page directory;
page_directory_t *current_directory=0;

// A bitset of frames - used or free.
uint32_t *frames;
uint32_t nframes;

// Defined in kheap.c
extern uint32_t placement_address;
extern heap_t *kheap;

void copy_page_physical(uint32_t src, uint32_t dest);

// Function to allocate a frame.
void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
	uint32_t idx = 0x0;
	if(page->frame)
		return;
		
	while (idx < nframes)
    {
        if (frames[idx/ARCH] == BAD) // nothing free, exit early.
        	idx += ARCH;
        	
        else if(frames[idx/ARCH]&(0x1 << (idx%ARCH)))
        	idx++;
        	
        else
        {
    	    frames[(idx/ARCH)] |= (0x1 << (idx%ARCH));
		    page->present = 1;
		    page->rw = (is_writeable==1)?1:0;
		    page->user = (is_kernel==1)?0:1;
		    page->frame = idx;
			return;
        }
    }
    PANIC("NO FRAMES LEFT!\n");
}

// Function to deallocate a frame.
void free_frame(page_t *page)
{
    if (!page->frame)
        return;
    
    frames[(page->frame/PAGE_SZ)/ARCH] &= ~(0x1 << (page->frame/PAGE_SZ)%ARCH);
    page->frame = 0x0;
}

void initialise_paging(size_t memsz)
{

    nframes = memsz;
    uint32_t phys;
    
    frames = (uint32_t*)kmalloc_ap(nframes/ARCH,&phys);
    memset(frames, 0, sizeof(uint32_t)*nframes/ARCH);
    
    // Let's make a page directory.
    kernel_directory = (page_directory_t*)kmalloc_ap(sizeof(page_directory_t),&phys);
    memset(kernel_directory, 0, sizeof(page_directory_t));

    
    kernel_directory->physicalAddr = (uint32_t)kernel_directory->tablesPhysical;
    // Map some pages in the kernel heap area.
    // Here we call get_page but not alloc_frame. This causes page_table_t's 
    // to be created where necessary. We can't allocate frames yet because they
    // they need to be identity mapped first below, and yet we can't increase
    // placement_address between identity mapping and enabling the heap!
    uint32_t i = KHEAP_START;
    while (i < KHEAP_START+KHEAP_INITIAL_SIZE)
    {
        get_page(i, 1, kernel_directory);
    	i += PAGE_SZ;
    }

    // We need to identity map (phys addr = virt addr) from
    // 0x0 to the end of used memory, so we can access this
    // transparently, as if paging wasn't enabled.
    i = 0;
    while (i < placement_address +PAGE_SZ)
    {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += PAGE_SZ;
    }

    // Now allocate those pages we mapped earlier.
    i = KHEAP_START; 
    while (i < KHEAP_START+KHEAP_INITIAL_SIZE)
    {
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += PAGE_SZ;
    }

    // Before we enable paging, we must register our page fault handler.
    register_interrupt_handler(14, page_fault);

    // Now, enable paging!
    switch_page_directory(kernel_directory);

    // Initialise the kernel heap.
    kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);

    current_directory = clone_directory(kernel_directory);
    switch_page_directory(current_directory);
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(dir->physicalAddr));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
    // Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    uint32_t table_idx = address / 1024;

    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        uint32_t tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
        return 0;
    }
}


void page_fault(registers_t *regs)
{
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    // Output an error message.
    monitor_write("Page fault! ( ");
    
    monitor_write((!(regs->err_code & 0x1))?    "present ":"");		// Page not present
    monitor_write((regs->err_code & 0x2)?       "read-only ":"");	// Write operation?
    monitor_write((regs->err_code & 0x4)?       "user-mode ":"");	// Processor was in user-mode?
    monitor_write((regs->err_code & 0x8)?   	"reserved ":"");	// Overwritten CPU-reserved bits of page entry?
    monitor_write((regs->err_code & 0x10)?      "id'd ":"");		// Caused by an instruction fetch?
    
    monitor_write(") at 0x");
    monitor_write_hex(faulting_address);
    monitor_write(" - EIP: ");
    monitor_write_hex(regs->eip);
    monitor_write("\n");
    PANIC("Page fault");
}

static page_table_t *clone_table(page_table_t *src, uint32_t *physAddr)
{
    // Make a new page table, which is page aligned.
    page_table_t *table = (page_table_t*)kmalloc_ap(sizeof(page_table_t), physAddr);
    // Ensure that the new table is blank.
    memset(table, 0, sizeof(page_directory_t));

    // For every entry in the table...
    int i;
    for (i = 0; i < 1024; i++)
    {
        // If the source entry has a frame associated with it...
        if (!src->pages[i].frame)
            continue;
        // Get a new frame.
        alloc_frame(&table->pages[i], 0, 0);
        // Clone the flags from source to destination.
        if (src->pages[i].present) table->pages[i].present = 1;
        if (src->pages[i].rw)      table->pages[i].rw = 1;
        if (src->pages[i].user)    table->pages[i].user = 1;
        if (src->pages[i].accessed)table->pages[i].accessed = 1;
        if (src->pages[i].dirty)   table->pages[i].dirty = 1;
        // Physically copy the data across. This function is in process.s.
        copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
    }
    return table;
}

page_directory_t *clone_directory(page_directory_t *src)
{
    uint32_t phys;
    // Make a new page directory and obtain its physical address.
    page_directory_t *dir = (page_directory_t*)kmalloc_ap(sizeof(page_directory_t), &phys);
    // Ensure that it is blank.
    memset(dir, 0, sizeof(page_directory_t));

    // Get the offset of tablesPhysical from the start of the page_directory_t structure.
    uint32_t offset = (uint32_t)dir->tablesPhysical - (uint32_t)dir;

    // Then the physical address of dir->tablesPhysical is:
    dir->physicalAddr = phys + offset;

    // Go through each page table. If the page table is in the kernel directory, do not make a new copy.
    int i;
    for (i = 0; i < 1024; i++)
    {
        if (!src->tables[i])
            continue;

        if (kernel_directory->tables[i] == src->tables[i])
        {
            // It's in the kernel, so just use the same pointer.
            dir->tables[i] = src->tables[i];
            dir->tablesPhysical[i] = src->tablesPhysical[i];
        }
        else
        {
            // Copy the table.
            uint32_t phys;
            dir->tables[i] = clone_table(src->tables[i], &phys);
            dir->tablesPhysical[i] = phys | 0x07;
        }
    }
    return dir;
}
