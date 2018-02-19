
// 
// task.c -- Defines functions and structures for multitasking.
//             Written for JamesM's kernel development tutorials.
//

#include "task.h"
#include "kheap.h"
#include "monitor.h"

task_list_t *ready_queue = NULL;
task_list_t *current_task = NULL;

uint32_t next_tid = 0;

void task_exit ();

extern void _create_task(int (*)(void*), void*, uint32_t*, task_t*);

task_t *init_tasking ()
{
  task_t *task = kmalloc (sizeof (task_t));
  task->id  = next_tid++;
  
  current_task = task;

  return task;
}

task_t *create_task (int (*fn)(void*), void *arg, uint32_t *stack)
{
  task_t *task = kmalloc (sizeof (task_t));
  memset (task, 0, sizeof (task_t));
  task->id = next_tid++;
  
  *--stack = (uint32_t)arg;
  *--stack = (uint32_t)&task_exit;
  *--stack = (uint32_t)fn;

  task->esp = (uint32_t)stack;
  task->ebp = 0;
  task->eflags = 0x200; // Interrupts enabled.
  task_is_ready(task);

  return task;
}

void task_exit ()
{
  register uint32_t val asm ("eax");

  monitor_write ("task exited with value ");
  monitor_write_dec(val);
  monitor_write("\n");
  for (;;) ;
}

void init_scheduler (task_t *initial_task)
{
  current_task = (task_list_t*) kmalloc (sizeof (task_list_t*));
  current_task->task = initial_task;
  current_task->next = 0;
  ready_queue = 0;
}

void task_is_ready (task_t *t)
{
  // Create a new list item for the new task.
  task_list_t *item = (task_list_t*) kmalloc (sizeof (task_list_t*));
  item->task = t;
  item->next = 0;

  if (!ready_queue)
  {
    // Special case if the ready queue is empty.
    ready_queue = item;
  }
  else
  {
    // Iterate through the ready queue to the end.
    task_list_t *iterator = ready_queue;
    while (iterator->next)
      iterator = iterator->next;

    // Add the item.
    iterator->next = item;
  }
}

void task_not_ready (task_t *t)
{
  // Attempt to find the task in the ready queue.
  task_list_t *iterator = ready_queue;

  // Special case if the task is first in the queue.
  if (iterator->task == t)
  {
    ready_queue = iterator->next;
    kfree (iterator);
    return;
  }

  while (iterator->next)
  {
    if (iterator->next->task == t)
    {
      task_list_t *tmp = iterator->next;
      iterator->next = tmp->next;
      kfree (tmp);
    }
    iterator = iterator->next;
  }
}

void schedule ()
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
