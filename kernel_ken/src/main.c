// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>
#include "kernel_ken.h"
#include "init.d"

int main(void)
{
    __init();
    // user application starts here

    return 0;
}
