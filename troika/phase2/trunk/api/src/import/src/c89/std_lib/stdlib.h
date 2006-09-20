#include <common.h>


extern int abs(int n);
/*
extern long labs(long n);
extern div_t div(int num, int denom);
extern ldiv_t ldiv(long num, long denom);
*/
extern double atof(const cstring* s);
extern int atoi(const cstring* s);
/*
extern long atol(const char* s);
extern double strtod(const char* s, char** endp);
extern long strtol(const char* s, char** endp, int base);
extern unsigned long strtoul(const char* s, char** endp, int base);
extern void* calloc(size_t nobj, size_t size);
extern void* malloc(size_t size);
extern void* realloc(void* p, size_t size);
extern void free(void* p);
extern void abort();
extern void exit(int status);
extern int atexit(void (*fcm)(void));
*/
extern int system(const cstring* s);
/** $alias="getenv" */
extern cstring* getenv_wrapper(const cstring* name);
/*
extern void* bsearch(const void* key, const void* base, size_t n, size_t size, int (*cmp)(const void* keyval, const void* datum));
extern void qsort(void* base, size_t n, size_t size, int (*cmp)(const void*, const void*));
extern int rand(void);
extern void srand(unsigned int seed);
*/
