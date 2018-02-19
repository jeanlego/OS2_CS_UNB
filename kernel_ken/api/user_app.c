#include "kernel_ken.h"
#include "user_app.h"

void user_app()
{
    // TODO add user apps here !
  int ret = fork();
  for(int i = 0; i < 2; i++)
    print("b");
  print("\n ----- exit parent ------- \n");
  return 0;
}
