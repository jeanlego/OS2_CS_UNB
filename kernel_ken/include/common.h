// common.h -- Defines typedefs and some global functions.
//             From JamesM's kernel development tutorials.

#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdint.h>


void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

#define BRK break_point();

#define ARCH sizeof(void*)*8

#define BAD 0xFFFFFFFF

#define PAGE_SZ 0x1000

#define UCHAR_MAX 255

void break_point();

// https://raw.githubusercontent.com/klange/toaruos/master/kernel/libc.c

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

extern void * memcpy(void * restrict dest, const void * restrict src, size_t n);
extern void * checked_memset(void * dest, int c, size_t n);
#define memset(dest,c,n) checked_memset((void *) dest,c,n);
extern void * memchr(const void * src, int c, size_t n);
extern void * memrchr(const void * m, int c, size_t n);
extern void * memmove(void *dest, const void *src, size_t n);

extern int memcmp(const void *vl, const void *vr, size_t n);

extern char * strdup(const char * s);
extern char * stpcpy(char * restrict d, const char * restrict s);
extern char * strcpy(char * restrict dest, const char * restrict src);
extern char * strchrnul(const char * s, int c);
extern char * strchr(const char * s, int c);
extern char * strrchr(const char * s, int c);
extern char * strpbrk(const char * s, const char * b);
extern char * strstr(const char * h, const char * n);
extern char * strcat(char *dest, const char *src);

extern int strcmp(const char * l, const char * r);

extern size_t strcspn(const char * s, const char * c);
extern size_t strspn(const char * s, const char * c);
extern size_t strlen(const char * s);

extern int atoi(const char * s);

/* Non-standard broken strtok_r */
extern char * strtok_r(char * str, const char * delim, char ** saveptr);




extern void panic(char *message, char *file, uint32_t line);
extern void panic_assert(char *file, uint32_t line, char *desc);

#endif // COMMON_H
