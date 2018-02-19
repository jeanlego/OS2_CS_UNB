
//
// scheduler.h -- Defines functions and structures for preempting tasks.
//                Written for JamesM's kernel development tutorials.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "common.h"
#include "task.h"

typedef struct task_list
{
  task_t *task;
  struct task_list *next;
} task_list_t;

void init_scheduler (task_t *initial_task);

void task_is_ready (task_t *t);

void task_not_ready (task_t *t);

void schedule ();

#endif
