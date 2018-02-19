// 
// task.c - Implements the functionality needed to multitask.
//          Written for JamesM's kernel development tutorials.
//

#include "task.h"
#include "paging.h"
#include "kheap.h"
#include "common.h"
#include <stddef.h>
#include <stdint.h>

// The currently running task.
volatile task_t *current_task;

// The start of the task linked list.
volatile task_t *ready_queue;

extern void perform_task_switch(uint32_t a, uint32_t b, uint32_t c, uint32_t d);

// Some externs are needed to access members in paging.c...
extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern void alloc_frame(page_t*,int,int);
extern uint32_t initial_esp;
extern uint32_t read_eip();


// The next available process ID.
uint32_t next_pid = 1;

static task_t *make_task(page_directory_t *dir, uint32_t id)
{
    task_t *new_task = (task_t *)kmalloc(sizeof(task_t));
    new_task->id = id;
    new_task->esp = new_task->ebp = new_task->eip = 0;
    new_task->page_directory = dir;
    new_task->next = NULL;
    return new_task;
}

void initialise_tasking()
{
    task_t *new_task = make_task(current_directory, next_pid++);
    // Rather important stuff happening, no interrupts please!
    asm volatile("cli");
    // Relocate the stack so we know where it is.
    move_stack((void*)0xE0000000, 0x2000);
    // Initialise the first task (kernel task)
    current_task = ready_queue = new_task;
    // Reenable interrupts.
    asm volatile("sti");
}

void move_stack(void *new_stack_start, uint32_t size)
{
  uint32_t i;
  // Allocate some space for the new stack.
  for( i = (uint32_t)new_stack_start;
       i >= ((uint32_t)new_stack_start-size);
       i = (i-0x1000 > i)?0:i-0x1000)
  {
    // General-purpose stack is in user-mode.
    alloc_frame( get_page(i, 1, current_directory), 0 /* User mode */, 1 /* Is writable */ );
  }
  
  // Flush the TLB by reading and writing the page directory address again.
  uint32_t pd_addr;
  asm volatile("mov %%cr3, %0" : "=r" (pd_addr));
  asm volatile("mov %0, %%cr3" : : "r" (pd_addr));

  // Old ESP and EBP, read from registers.
  uint32_t old_stack_pointer; asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
  uint32_t old_base_pointer;  asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

  // Offset to add to old stack addresses to get a new stack address.
  uint32_t offset            = (uint32_t)new_stack_start - initial_esp;

  // New ESP and EBP.
  uint32_t new_stack_pointer = old_stack_pointer + offset;
  uint32_t new_base_pointer  = old_base_pointer  + offset;

  // Copy the stack.
  memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp-old_stack_pointer);

  // Backtrace through the original stack, copying new values into
  // the new stack.  
  for(i = (uint32_t)new_stack_start; i > (uint32_t)new_stack_start-size; i = (i-4 > i)?0 :i-4)
  {
    uint32_t tmp = * (uint32_t*)i;
    // If the value of tmp is inside the range of the old stack, assume it is a base pointer
    // and remap it. This will unfortunately remap ANY value in this range, whether they are
    // base pointers or not.
    if (( old_stack_pointer < tmp) && (tmp < initial_esp))
    {
      tmp = tmp + offset;
      uint32_t *tmp2 = (uint32_t*)i;
      *tmp2 = tmp;
    }
  }

  // Change stacks.
  asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
  asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
}

void switch_task()
{
    // If we haven't initialised tasking yet, just return.
    if (!ready_queue)
        return;

    // Read esp, ebp now for saving later on.
    uint32_t esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    eip = read_eip();

    // Have we just switched tasks?
    if (eip == 0x12345)
        return;
        
    // No, we didn't switch tasks. Let's save some register values and switch.
    current_task->eip = eip;
    current_task->esp = esp;
    current_task->ebp = ebp;
    
    asm volatile("cli");
    // Get the next task to run.
    current_task = ready_queue;
    current_directory = current_task->page_directory;
    perform_task_switch(current_task->eip, current_directory->physicalAddr, current_task->ebp, current_task->esp);
    asm volatile("sti");
}

int fork()
{
    if (!current_task)
        return -1;
        
    page_directory_t *directory = clone_directory(current_directory);
    task_t *new_task = make_task(directory, next_pid++);
    
    // We are modifying kernel structures, and so cannot
    asm volatile("cli");
    new_task->next = ready_queue;
    ready_queue = new_task;
    asm volatile("sti");
    
    uint32_t esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    // This will be the entry point for the new process.
    eip = read_eip();
    
    // We could be the parent or the child here - check.
    if (eip == 0x12345)
        return 0;

    new_task->esp = esp;
    new_task->ebp = ebp;
    new_task->eip = eip;

	return new_task->id;
}

int getpid()
{
    return current_task->id;
}
