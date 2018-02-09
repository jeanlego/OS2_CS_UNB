// common.h -- Defines typedefs and some global functions.
//             From JamesM's kernel development tutorials.

#ifndef STRING_H
#define STRING_H

#include "global.h"
#include "error.h"

// Copy len bytes from src to dest.
void memcpy(unsigned char *dest, const unsigned char *src, unsigned int len);

// Write len copies of val into dest.
void memset(unsigned char *dest, unsigned char val, unsigned int len);

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2);

// Copy the NULL-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src);

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
char *strcat(char *dest, const char *src);




#endif // COMMON_H
