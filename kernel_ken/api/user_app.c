#include "kernel_ken.h"
#include "user_app.h"

int fn(void *arg)
{
  for(int i = 0; i < 2; i++)
    print("a");
  print("\n ----- exit child ------- \n");
  return 1;
}

void user_app()
{
    // TODO add user apps here !
    
  void *stack = (void *)alloc(0x400 + 0x3F0,1);

  //create_task(&fn, (void*)0x567, stack);
  int ret = fork();
  for(int i = 0; i < 2; i++)
    print("b");
  print("\n ----- exit parent ------- \n");
  return 0;
}
