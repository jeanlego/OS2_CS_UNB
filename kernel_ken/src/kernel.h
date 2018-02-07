#ifndef KERNEL
#define KERNEL

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
   Size information for a hole/block
**/
typedef struct
{
    u32int magic;   // Magic number, used for error checking and identification.
    u8int is_hole;   // 1 if this is a hole. 0 if this is a block.
    u32int size;    // size of the block, including the end footer.
} header_t;

typedef struct
{
    u32int magic;     // Magic number, same as in header_t.
    header_t *header; // Pointer to the block header.
} footer_t;

typedef struct
{
    ordered_array_t index;
    u32int start_address; // The start of our allocated space.
    u32int end_address;   // The end of our allocated space. May be expanded up to max_address.
    u32int max_address;   // The maximum address the heap can be expanded to.
    u8int supervisor;     // Should extra pages requested by us be mapped as supervisor-only?
    u8int readonly;       // Should extra pages requested by us be mapped as read-only?
} heap_t;

// This structure defines a 'task' - a process.
typedef struct task
{
    int id;                // Process ID.
    u32int esp, ebp;       // Stack and base pointers.
    u32int eip;            // Instruction pointer.
    page_directory_t *page_directory; // Page directory.
    struct task *next;     // The next task in a linked list.
} task_t;

// Create a new heap for a new process/thread. start and end indicate the bounds
//   for the new heap. 
heap_t *create_heap(u32int start, u32int end, u32int max, u8int supervisor, u8int readonly);

// General deallocation function for memory in the kernel's heap. p is
//   the address of the memory to be freed.
void kfree(void *p);

// Causes the current process' stack to be forcibly moved to a new location.
//   The parameters specify the new location and size of the stack.
void move_stack(void *new_stack_start, u32int size);

#endif

