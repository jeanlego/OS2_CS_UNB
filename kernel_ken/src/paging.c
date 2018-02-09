// paging.c -- Defines the interface for and structures relating to paging.
//             Written for JamesM's kernel development tutorials.

#include "paging.h"
#include "kheap.h"
#include "monitor.h"

// The kernel's page directory
page_directory_t *kernel_directory=0;

// The current page directory;
page_directory_t *current_directory=0;

// A bitset of frames_bitset - used or free.
unsigned int *frames_bitset;
unsigned int nframes;

// Defined in kheap.c
extern unsigned int placement_address;

// Macros used in the bitset algorithms.
#define FRAME_ID(addr)          (addr/FRAME_SIZE)
#define BIT_ID(addr)            (FRAME_ID(addr)/ARCH_BIT_WIDTH)
#define BIT_OFF(addr)           (FRAME_ID(addr)%ARCH_BIT_WIDTH)

// Static function to set a bit in the frames_bitset bitset
#define set_frame(frame_addr)   frames_bitset[BIT_ID(frame_addr)] \
                                    |= (0x1 << BIT_OFF(frame_addr))

// Static function to clear a bit in the frames_bitset bitset
#define clear_frame(frame_addr) frames_bitset[BIT_ID(frame_addr)] \
                                    &= ~(0x1 << BIT_OFF(frame_addr))


// Static function to test if a bit is set.
#define test_frame(frame_addr)  (frames_bitset[BIT_ID(frame_addr)] \
                                    & (0x1 << BIT_OFF(frame_addr)))
                                    
#define NEXT_FRAME_PKT  ARCH_BIT_WIDTH*FRAME_SIZE
#define NEXT_FRAME      FRAME_SIZE

// Static function to find the first free frame.
static unsigned int first_frame()
{
    unsigned int frame_addr = 0;
    while(frame_addr < MEM_END_PG)
    {
        // this whole frame is full so skip to the next one
        if(frames_bitset[BIT_ID(frame_addr)] == END_ADDR)
            frame_addr += NEXT_FRAME_PKT;
            
        // this particular frame is used
        else if(test_frame(frame_addr))
            frame_addr += NEXT_FRAME;
        
        //this one is free
        else
            return FRAME_ID(frame_addr);
    }
}

// Function to allocate a frame.
void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
    if (page->frame != 0)
        return;

    unsigned int idx = first_frame();
    ASSERT(idx != END_ADDR 
        && "no frame left!");
        
    set_frame(idx*FRAME_SIZE);
    page->present = 1;
    page->rw = (is_writeable)?1:0;
    page->user = (is_kernel)?0:1;
    page->frame = idx;
}

// Function to deallocate a frame.
void free_frame(page_t *page)
{
    unsigned int frame;
    if (!(frame=page->frame))
        return;
        
    clear_frame(frame);
    page->frame = 0x0;
}

void initialise_paging()
{
    nframes = MEM_END_PG / FRAME_SIZE;
    
    // the tail address gives us the count for the number of frame bit_set needed 
    frames_bitset = (unsigned int*)kmalloc(BIT_ID(MEM_END_PG));
    memset(frames_bitset, 0, sizeof(int)*BIT_ID(MEM_END_PG));
    
    // Let's make a page directory.
    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    current_directory = kernel_directory;

    // We need to identity map (phys addr = virt addr) from
    // 0x0 to the end of used memory, so we can access this
    // transparently, as if paging wasn't enabled.
    // NOTE that we use a while loop here deliberately.
    // inside the loop body we actually change placement_address
    // by calling kmalloc(). A while loop causes this to be
    // computed on-the-fly rather than once at the start.
    int i = 0;
    while (i < placement_address)
    {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += FRAME_SIZE;
    }
    // Before we enable paging, we must register our page fault handler.
    register_interrupt_handler(14, page_fault);

    // Now, enable paging!
    switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    unsigned int cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(unsigned int address, int make, page_directory_t *dir)
{
    // Turn the address into an index.
    int idx = FRAME_ID(address);
    // Find the page table containing this address.
    unsigned int table_idx = idx / N_PAGE;
    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[idx%N_PAGE];
    }
    else if(make)
    {
        unsigned int tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[idx%N_PAGE];
    }
    else
    {
        return 0;
    }
}


void page_fault(registers_t regs)
{
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    unsigned int faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
    // The error code gives us details of what happened.
    int present   = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;           // Write operation?
    int us = regs.err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;          // Caused by an instruction fetch?
    
    // Output an error message.
    monitor_write("Page fault! ( ");
    if (present) {monitor_write("present ");}
    if (rw) {monitor_write("read-only ");}
    if (us) {monitor_write("user-mode ");}
    if (reserved) {monitor_write("reserved ");}
    monitor_write(") at ");
    monitor_write_hex(faulting_address);
    monitor_write("\n");
    PANIC("Page fault");
}
