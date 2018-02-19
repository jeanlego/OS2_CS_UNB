#include "kernel_ken.h"
#include "user_app.h"

// int fn(void *arg)
// {
//   for(int i = 0; i < 2; i++)
//     monitor_write("a");
//   monitor_write("\n ----- exit child ------- \n");
//   return 1;
// }

void user_app()
{
    // TODO add user apps here !
    
//   uint32_t *stack = kmalloc (0x400) + 0x3F0;

//   task_t *t = create_task(&fn, (void*)0x567, stack);
//   for(int i = 0; i < 2; i++)
//     monitor_write("b");
//   monitor_write("\n ----- exit parrent ------- \n");
//   return 0;
}
