// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "task.h"
#include "syscall.h"

extern u32int placement_address;
u32int initial_esp;

int main(struct multiboot *mboot_ptr, u32int initial_stack)
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
    initialise_paging();

    // Start multitasking.
    initialise_tasking();

    initialise_syscalls();

    switch_to_user_mode();

    syscall_monitor_write("Hello, user world!\n");

    return 0;
}
