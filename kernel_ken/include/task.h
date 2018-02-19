
// 
// task.h -- Declares functions and structures for multitasking.
//             Written for JamesM's kernel development tutorials.
//


#ifndef TASK_H
#define TASK_H

#include "common.h"

typedef struct
{
  uint32_t esp, ebp, ebx, esi, edi, eflags;
  uint32_t id;                  // task ID.
} task_t;


typedef struct task_list
{
  task_t *task;
  struct task_list *next;
} task_list_t;


void TASK_init ();
uint32_t TASK_fork();
void TASK_exit();

void switch_task (struct task_list *next);

void schedule ();

#endif

