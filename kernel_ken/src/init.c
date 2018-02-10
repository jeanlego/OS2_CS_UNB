#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "init.d"
#include "kernel.h"
#include "error.h"
#include "global.h"


void __init()
{
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();

    initialise_paging();
    
    monitor_write("Hello, paging world!\n");

    unsigned int *ptr = (unsigned int*)0xA0000000;
    unsigned int do_page_fault = *ptr;
    do_page_fault++;
}