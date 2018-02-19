#ifndef USER_APP
// 
// task.c -- Defines functions and structures for multitasking.
//             Written for JamesM's kernel development tutorials.
//

#include "task.h"
#include "kheap.h"
#include "monitor.h"
#include "scheduler.h"

task_t *current_task;
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

#endif