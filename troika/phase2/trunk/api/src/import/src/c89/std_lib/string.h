#include <common.h>

/*
extern char* strcpy(char* s, const char* ct);
extern char* strncpy(char* s, const char* ct, size_t n);
*/
/** $alias="strcat"
    \note  this function deviates from ANSI C in that it returns a new string as
    its result, rather than writing the result into the first argument string */
extern cstring* strcat_wrapper(cstring* s, const cstring* ct);
/*
extern char* strncat(char* s, const char* ct, size_t n);
*/


/** $alias="strcmp" */
extern int strcmp_wrapper(const cstring* cs, const cstring* ct);


/*
extern int strncmp(const char* cs, const char* ct, size_t n);
extern int strcoll(const char* cs, const char* ct);
extern char* strchr(const cstring* cs, int c);
extern char* strrchr(const cstring* cs, int c);
extern size_t strspn(const char* cs, const char* ct);
extern size_t strcspn(const char* cs, const char* ct);
extern char* strpbrk(const char* cs, const char* ct);
extern char* strstr(const char* cs, const char* ct);
*/
/** $alias="strlen"
    \note  this function deviates from ANSI C in that it returns an int rather
    than a size_t */
extern int strlen_wrapper(const cstring* cs);
/*
extern char* strerror(int n);
extern char* strtok(char* s, const char* t);
extern size_t strxfrm(char* s, const char* ct, size_t n);
extern void* memcpy(void* s, const void* ct, size_t n);
extern void* memmove(void* s, const void* ct, size_t n);
extern int memcmp(const void* cs, const void* ct, size_t n);
extern void* memchr(const void* cs, int c, size_t n);
extern void* memset(void* s, int c, size_t n);
*/
