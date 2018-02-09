#ifndef KERNEL
#define KERNEL

#include "global.h"
#include "error.h"
#include "ordered_array.h"
#include "paging.h"

/**
   Size information for a hole/block
**/
typedef struct
{
    unsigned int magic;   // Magic number, used for error checking and identification.
    unsigned char is_hole;   // 1 if this is a hole. 0 if this is a block.
    unsigned int size;    // size of the block, including the end footer.
} header_t;

typedef struct
{
    unsigned int magic;     // Magic number, same as in header_t.
    header_t *header; // Pointer to the block header.
} footer_t;

typedef struct
{
    ordered_array_t index;
    unsigned int start_address; // The start of our allocated space.
    unsigned int end_address;   // The end of our allocated space. May be expanded up to max_address.
    unsigned int max_address;   // The maximum address the heap can be expanded to.
    unsigned char supervisor;     // Should extra pages requested by us be mapped as supervisor-only?
    unsigned char readonly;       // Should extra pages requested by us be mapped as read-only?
} heap_t;

// This structure defines a 'task' - a process.
typedef struct task
{
    int id;                // Process ID.
    unsigned int esp, ebp;       // Stack and base pointers.
    unsigned int eip;            // Instruction pointer.
    page_directory_t *page_directory; // Page directory.
    struct task *next;     // The next task in a linked list.
} task_t;

// Create a new heap for a new process/thread. start and end indicate the bounds
//   for the new heap. 
heap_t *create_heap(unsigned int start, unsigned int end, unsigned int max, unsigned char supervisor, unsigned char readonly);

// General deallocation function for memory in the kernel's heap. p is
//   the address of the memory to be freed.
void kfree(void *p);

// Causes the current process' stack to be forcibly moved to a new location.
//   The parameters specify the new location and size of the stack.
void move_stack(void *new_stack_start, unsigned int size);

#endif

