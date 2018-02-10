// common.h -- Defines typedefs and some global functions.
//             From JamesM's kernel development tutorials.

#ifndef STRING_H
#define STRING_H

#include "global.h"
#include "error.h"

// Copy len bytes from src to dest.
void unchecked_memcpy(unsigned char *dest, const unsigned char *src, unsigned int len);
#define memcpy(dest,src,len) unchecked_memcpy((unsigned char *)dest,(const unsigned char *)src,len)

// Write len byte copies of val into dest.
// clear out some warning by casting automaticaly using macro
void unchecked_memset(unsigned char *dest, const unsigned char val, unsigned int len);
#define memset(dest,val,len) unchecked_memset((unsigned char *)dest,(const unsigned char)val,len)

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
