#include "kernel.h"
// Create a new heap for a new process/thread. start and end indicate the bounds
//   for the new heap. 
heap_t *create_heap(unsigned int start, unsigned int end, unsigned int max, unsigned char supervisor, unsigned char readonly)
{
    return NULL;
}

// General deallocation function for memory in the kernel's heap. p is
//   the address of the memory to be freed.
void kfree(void *p)
{
    
}

// Causes the current process' stack to be forcibly moved to a new location.
//   The parameters specify the new location and size of the stack.
void move_stack(void *new_stack_start, unsigned int size)
{
    
}
