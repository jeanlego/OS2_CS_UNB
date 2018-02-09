// kheap.h -- Interface for kernel heap functions, also provides
//            a placement malloc() for use before the heap is 
//            initialised.
//            Written for JamesM's kernel development tutorials.

#ifndef KHEAP_H
#define KHEAP_H

#include "string.h"
#include "error.h"

/* These are EDITABLE by you to implement your kernel. The values here are initial
 * defines and struct fields. Feel free to add more fields to these structs, change
 * the define values, etc. as you see fit.
 *
 * I have included some function prototypes at the bottom that you will perhaps want
 * to implement, but I will leave it to you to decide if you want to follow my advice
 * or go your own way :)
 */

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000

#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0x123890AB
#define HEAP_MIN_SIZE     0x70000


/**
   Allocate a chunk of memory, sz in size. If align == 1,
   the chunk must be page-aligned. If phys != 0, the physical
   location of the allocated chunk will be stored into phys.

   This is the internal version of kmalloc. More user-friendly
   parameter representations are available in kmalloc, kmalloc_a,
   kmalloc_ap, kmalloc_p.
**/
unsigned int kmalloc_int(unsigned int sz, int align, unsigned int *phys);

/**
   Allocate a chunk of memory, sz in size. The chunk must be
   page aligned.
**/
unsigned int kmalloc_a(unsigned int sz);

/**
   Allocate a chunk of memory, sz in size. The physical address
   is returned in phys. Phys MUST be a valid pointer to unsigned int!
**/
unsigned int kmalloc_p(unsigned int sz, unsigned int *phys);

/**
   Allocate a chunk of memory, sz in size. The physical address 
   is returned in phys. It must be page-aligned.
**/
unsigned int kmalloc_ap(unsigned int sz, unsigned int *phys);

/**
   General allocation function.
**/
unsigned int kmalloc(unsigned int sz);

#endif // KHEAP_H
