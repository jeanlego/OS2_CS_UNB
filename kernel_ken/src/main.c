// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "task.h"
#include "kheap.h"

extern uint32_t placement_address;
uint32_t initial_esp;


int kernel_main(struct multiboot *mboot_ptr, uint32_t initial_stack)
{
    initial_esp = initial_stack;
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();
    // Initialise the PIT to 100Hz
    asm volatile("sti");
    init_timer(50);

    // Start paging.
    initialise_paging((mboot_ptr->mem_lower+mboot_ptr->mem_upper)/4);

    // Start multitasking.
    initialise_tasking();

        // Create a new process in a new address space which is a clone of this

        int ret = fork();
        monitor_write("fork(");
        monitor_write(") returned ");
        monitor_write_hex(ret);
        monitor_write(", and getpid() returned ");
        monitor_write_hex(getpid());
        monitor_write("\n============================================================================\n");

    return 0;
}
