// common.c -- Defines some global functions.
//             From JamesM's kernel development tutorials.

#include "string.h"

// Copy len bytes from src to dest.
void unchecked_memcpy(unsigned char *dest, const unsigned char *src, unsigned int len)
{
    WARN_ASSERT(dest,
        "DESTINATION for memset is not malloc'd");
    WARN_ASSERT(src, 
        "SOURCE for memset is not malloc'd");
    if(!dest)
        return;
    
    if(!src)
        return; 
    int warn_overwrite =0;    
    for(int i=0; i<len; i++)
    {
        if(dest[i] != '\0')
            warn_overwrite++;
            
        dest[i] = src[i];
    }
    
    WARN_ASSERT(!warn_overwrite, 
        "we are overwriting data on destination memcpy!");
    
}

// Write len Byte copies of val into dest.
void unchecked_memset(unsigned char *dest, const unsigned char val, unsigned int len)
{
    WARN_ASSERT(dest,
        "DESTINATION for memset is not malloc'd");
        
    if(!dest)
        return;
        
    int warn_overwrite =0;    
    for(int i=0; i<len; i++)
    {
        if(dest[i] != '\0')
            warn_overwrite++;
            
        dest[i] = val;
    }
    
    WARN_ASSERT(!warn_overwrite, 
        "we are overwriting data on destination memset!");
    
}

int strlen(const char *str1)
{
    WARN_ASSERT(str1,
        "string for strlen is not malloc'd");
        
    if(!str1)
        return 0;
    unsigned int count=0;
    while(str1[count++] != '\0');
    
    return count;
}

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2)
{
    WARN_ASSERT(str1,
        "first string for strcmp is not malloc'd");
    WARN_ASSERT(str2,
        "second string for strcmp is not malloc'd");
        
    if(!str1 && !str2)
        return -1;
        
    else if(!str1 && str2)
        return -1;
    
    else if(str1 && !str2)
        return -1;
        
    int str1_len = strlen(str1);
    int str2_len = strlen(str2);
    
    int len_compare = str1_len - str2_len;
    if(len_compare)
        return len_compare;
    
    int diff_count = 0;    
    for(int i=0; i<str1_len; i++)
        if(str1[i] != str2[i])
            diff_count++;
    
    return diff_count;
}

// Copy the NULL-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src)
{
    WARN_ASSERT(dest,
        "DESTINATION for strcpy is not malloc'd");
    WARN_ASSERT(src, 
        "SOURCE for strcpy is not malloc'd");
    
    if(!dest)
        return NULL;
    
    if(!src)
        return dest;

    memcpy(dest,src, strlen(src));

    return dest;
}

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
char *strcat(char *dest, const char *src)
{
    
    WARN_ASSERT(dest,
        "DESTINATION for strcat is not malloc'd");
    WARN_ASSERT(src, 
        "SOURCE for strcat is not malloc'd");
        
    if(!dest)
        return NULL;
    
    if(!src)
        return dest;
    
    memcpy(&dest[strlen(dest)],src,strlen(src));
    return dest;
}
