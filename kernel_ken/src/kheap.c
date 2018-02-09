// kheap.c -- Kernel heap functions, also provides
//            a placement malloc() for use before the heap is 
//            initialised.
//            Written for JamesM's kernel development tutorials.

#include "kheap.h"

// end is defined in the linker script.
extern unsigned int end;
unsigned int placement_address = (unsigned int)&end;

unsigned int kmalloc_int(unsigned int sz, int align, unsigned int *phys)
{
    // This will eventually call malloc() on the kernel heap.
    // For now, though, we just assign memory at placement_address
    // and increment it by sz. Even when we've coded our kernel
    // heap, this will be useful for use before the heap is initialised.
    int mask = placement_address / FRAME_SIZE;
    // Align the placement address;
    if (align && mask)
        placement_address = (mask+align)*FRAME_SIZE;
    
    if (phys)
        *phys = placement_address;
    
    unsigned int tmp = placement_address;
    placement_address += sz;
    return tmp;
}

unsigned int kmalloc_a(unsigned int sz)
{
    return kmalloc_int(sz, 1, 0);
}

unsigned int kmalloc_p(unsigned int sz, unsigned int *phys)
{
    return kmalloc_int(sz, 0, phys);
}

unsigned int kmalloc_ap(unsigned int sz, unsigned int *phys)
{
    return kmalloc_int(sz, 1, phys);
}

unsigned int kmalloc(unsigned int sz)
{
    return kmalloc_int(sz, 0, 0);
}
