#ifndef CHAR_H
#define CHAR_H

#include <defs.h>


/** $decoder */
char *char__decode( cstring *buffer );

/** $encoder */
void char__encode( char *c, cstring *buffer );


int char__compare_to( char c1, char c2 );

int char__isalnum( char c );
int char__isalpha( char c );
int char__iscntrl( char c );
int char__isdigit( char c );
int char__isgraph( char c );
int char__islower( char c );
int char__isprint( char c );
int char__ispunct( char c );
int char__isspace( char c );
int char__isupper( char c );
int char__isxdigit( char c );
char char__tolower( char c );
char char__toupper( char c );


#endif  /* CHAR_H */
