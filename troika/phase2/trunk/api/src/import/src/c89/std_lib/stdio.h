#include <common.h>

/*
extern FILE* fopen(const char* filename, const char* mode);
extern FILE* freopen(const char* filename, const char* mode, FILE* stream);
extern int fflush(FILE* stream);
extern int fclose(FILE* stream);
extern int remove(const char* filename);
extern int rename(const char* oldname, const char* newname);
extern FILE* tmpfile();
extern char* tmpname(char s[L_tmpnam]);
extern int setvbuf(FILE* stream, char* buf, int mode, size_t size);
extern void setbuf(FILE* stream, char* buf);
extern int fprintf(FILE* stream, const char* format, ...);
extern int printf(const char* format, ...);
extern int sprintf(char* s, const char* format, ...);
extern int vfprintf(FILE* stream, const char* format, va_list arg);
extern int vprintf(const char* format, va_list arg);
extern int vsprintf(char* s, const char* format, va_list arg);
extern int fscanf(FILE* stream, const char* format, ...);
extern int scanf(const char* format, ...);
extern int sscanf(char* s, const char* format, ...);
extern int fgetc(FILE* stream);
extern char* fgets(char* s, int n, FILE* stream);
extern int fputc(int c, FILE* stream);
extern char* fputs(const char* s, FILE* stream);
extern int getc(FILE* stream);
extern int getchar(void);
extern char* gets(char* s);
extern int putc(int c, FILE* stream);
*/
/** $alias="putchar"
    \note: uses a wrapper because of an apparent macro conflict when compiling
    on (SourceForge Compile Farm server) x86-solaris1 */
extern int putchar_wrapper(int c);
extern int puts(const cstring* s);
/*
extern int ungetc(int c, FILE* stream);
extern size_t fread(void* ptr, size_t size, size_t nobj, FILE* stream);
extern size_t fwrite(const void* ptr, size_t size, size_t nobj, FILE* stream);
extern int fseek(FILE* stream, long offset, int origin);
extern long ftell(FILE* stream);
extern void rewind(FILE* stream);
extern int fgetpos(FILE* stream, fpos_t* ptr);
extern int fsetpos(FILE* stream, const fpos_t* ptr);
extern void clearerr(FILE* stream);
extern int feof(FILE* stream);
extern int ferror(FILE* stream);
extern void perror(const char* s);
*/
