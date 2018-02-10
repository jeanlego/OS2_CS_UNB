#ifndef ERROR_H
#define ERROR_H

#define PANIC(msg)          panic(msg, __FILE__, __LINE__)
#define ASSERT(b)           if(b)   (void)0; \
                            else    panic_assert(__FILE__, __LINE__, #b);\

#ifdef DEBUG                            
    #define WARN_ASSERT(b,mess) if(b)   (void)0; \
                                else{ \
                                    warning(__FILE__, __LINE__, mess);\
                                    __asm__("int $3"); \
                                }
#else
    #define WARN_ASSERT(b,mess) (void)0;
#endif 

void panic(const char *message, const char *file, unsigned int line);
void panic_assert(const char *file, unsigned int line, const char *desc);

void warning(const char *file, unsigned int line, const char *desc);
void warning_assert(const char *file, unsigned int line, const char *desc);

#endif // ERROR_H