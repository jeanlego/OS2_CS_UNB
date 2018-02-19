
// 
// task.c -- Defines functions and structures for multitasking.
//             Written for JamesM's kernel development tutorials.
//

#include "task.h"
#include "kheap.h"
#include "monitor.h"

task_list_t *ready_queue = NULL;
task_list_t *current_task = NULL;
extern void switch_task (struct task_list *next);

uint32_t next_tid = 1;

void TASK_init ()
{
  task_t *task = kmalloc (sizeof (task_t));
  task->id  = next_tid++;
  current_task = (task_list_t*) kmalloc (sizeof (task_list_t*));
  current_task->task = task;
  current_task->next = 0;
  ready_queue = 0;
  asm volatile ("sti");
}

void TASK_exit()
{
  register uint32_t val asm ("eax");

  monitor_write ("task exited with value ");
  monitor_write_dec(val);
  monitor_write("\n");
  for (;;) ;
}

int fn(void *arg)
{
  
	monitor_write("pid #");
	monitor_write_dec(get_pid());
	monitor_write(" ----- exiting child ------- \n");
	return 1;
}

uint32_t TASK_fork()
{
  uint32_t *stack = kmalloc (0x400) + 0x3F0;
  task_t *task = kmalloc (sizeof (task_t));
  memset (task, 0, sizeof (task_t));
  task->id = next_tid++;
  
  *--stack = (uint32_t)((void*)0x567);
  *--stack = (uint32_t)&TASK_exit;
  *--stack = (uint32_t)&fn;

  task->esp = (uint32_t)stack;
  task->ebp = 0;
  task->eflags = 0x200; // Interrupts enabled.
  
    // Create a new list item for the new task.
  task_list_t *item = (task_list_t*) kmalloc (sizeof (task_list_t*));
  item->task = task;
  item->next = ready_queue;
  ready_queue = item;
  // if (!ready_queue)
  // {
  //   // Special case if the ready queue is empty.
  //   ready_queue = item;
  // }
  // else
  // {
  //   // Iterate through the ready queue to the end.
  //   task_list_t *iterator = ready_queue;
  //   while (iterator->next)
  //     iterator = iterator->next;

  //   // Add the item.
  //   iterator->next = item;
  // }

  return task->id;
}

void TASK_schedule()
{
  if (!ready_queue) return;

  // Iterate through the ready queue to the end.
  task_list_t *iterator = ready_queue;
  while (iterator->next)
    iterator = iterator->next;

  // Add the old task to the end of the queue, and remove it from the start.
  iterator->next = current_task;
  current_task->next = 0;
  task_list_t *new_task = ready_queue;

  ready_queue = ready_queue->next;

  // Switch to the new task.
  switch_task (new_task);
}

uint32_t get_pid()
{
  return current_task->task->id;
}
