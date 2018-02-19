
// 
// task.h -- Declares functions and structures for multitasking.
//             Written for JamesM's kernel development tutorials.
//

#ifndef task_H
#define task_H

#include "common.h"

typedef struct
{
  uint32_t esp, ebp, ebx, esi, edi, eflags;
  uint32_t id;                  // task ID.
} task_t;

task_t *init_tasking ();

task_t *create_task (int (*fn)(void*), void *arg, uint32_t *stack);

struct task_list;

void switch_task (struct task_list *next);

#endif

