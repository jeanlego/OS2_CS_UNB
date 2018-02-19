
// 
// kheap.h -- Declares functions and structures for the kernel heap.
//           Written for JamesM's kernel development tutorials.
//

#ifndef USER_APP
#ifndef HEAP_H
#define HEAP_H

#include "common.h"

// Returns a pointer to a chunk of memory, minimum size 'l' bytes.
void *kmalloc (uint32_t l);

// Takes a chunk of memory allocated with kmalloc, and returns it to the pool of usable memory.
void kfree (void *p);

#endif
#endif

