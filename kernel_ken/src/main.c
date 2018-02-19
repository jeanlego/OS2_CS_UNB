// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include "multiboot.h"
#include "common.h"
#include "monitor.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"
#include "thread.h"

int fn(void *arg)
{
  for(int i = 0; i < 2; i++)
    monitor_write("a");
  monitor_write("\n ----- exit child ------- \n");
  return 1;
}


int kernel_main(multiboot_t *mboot_ptr)
{

  monitor_clear();

  monitor_write("Hello, world!\n");

  init_gdt ();
  init_idt ();

  init_timer (20);


  init_pmm (mboot_ptr->mem_upper);
  init_vmm ();
  init_heap ();



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
  
  asm volatile ("sti");
  
  thread_t *kernel_thread = init_threading();
  init_scheduler (kernel_thread);



  uint32_t *stack = kmalloc (0x400) + 0x3F0;

  thread_t *t = create_thread(&fn, (void*)0x567, stack);
  for(int i = 0; i < 2; i++)
    monitor_write("b");
  monitor_write("\n ----- exit parrent ------- \n");
  return 0;

}
