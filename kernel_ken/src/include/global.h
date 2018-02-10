// common.h -- Defines typedefs and some global functions.
//             From JamesM's kernel development tutorials.

#ifndef GLOBAL_H
#define GLOBAL_H

#define ARCH_BIT_WIDTH sizeof(void*) * 8

#define FRAME_SIZE 4096

#define N_PAGE 1024

// The size of physical memory. For the moment we 
// assume it is 16MB big.
#define MEM_END_PG 0x1000000 

#define END_ADDR ~(0x0)

#define NULL 0

//uncomment to debug
#define DEBUG

#endif // GLOBAL_H
