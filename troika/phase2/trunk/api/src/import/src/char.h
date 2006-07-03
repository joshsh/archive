#ifndef CHAR_H
#define CHAR_H

#include <common.h>


/** $decoder */
extern char *
char__decode( cstring *buffer );

/** $encoder */
extern void
char__encode( char *c, cstring *buffer );


extern int
char__compare_to( char c1, char c2 );

extern int char__isalnum( char c );
extern int char__isalpha( char c );
extern int char__iscntrl( char c );
extern int char__isdigit( char c );
extern int char__isgraph( char c );
extern int char__islower( char c );
extern int char__isprint( char c );
extern int char__ispunct( char c );
extern int char__isspace( char c );
extern int char__isupper( char c );
extern int char__isxdigit( char c );
extern char char__tolower( char c );
extern char char__toupper( char c );


#endif  /* CHAR_H */
