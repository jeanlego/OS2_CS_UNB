
//
// scheduler.c -- Defines functions and structures for preempting tasks.
//                Written for JamesM's kernel development tutorials.
//

#include "scheduler.h"
#include "heap.h"

task_list_t *ready_queue = 0;
task_list_t *current_task = 0;

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

