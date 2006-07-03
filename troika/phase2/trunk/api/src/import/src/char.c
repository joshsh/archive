#include <settings.h>
#include "char.h"


char *
char__decode( cstring *buffer )
{
    char *c = new( char );
    *c = *buffer;
    return c;
}


void
char__encode( char *c, cstring *buffer )
{
    sprintf( buffer, "%c", *c );
}


int
char__compare_to( char c1, char c2 )
{
    return ( c1 == c2 ) ? 0 : c1 < c2 ? -1 : 1;
}


#include <ctype.h>

/* The "0 !=" encourages these functions to return 1, and only 1, for TRUE. */
int char__isalnum( char c ) { return 0 != isalnum( ( int ) c ); }
int char__isalpha( char c ) { return 0 != isalpha( ( int ) c ); }
int char__iscntrl( char c ) { return 0 != iscntrl( ( int ) c ); }
int char__isdigit( char c ) { return 0 != isdigit( ( int ) c ); }
int char__isgraph( char c ) { return 0 != isgraph( ( int ) c ); }
int char__islower( char c ) { return 0 != islower( ( int ) c ); }
int char__isprint( char c ) { return 0 != isprint( ( int ) c ); }
int char__ispunct( char c ) { return 0 != ispunct( ( int ) c ); }
int char__isspace( char c ) { return 0 != isspace( ( int ) c ); }
int char__isupper( char c ) { return 0 != isupper( ( int ) c ); }
int char__isxdigit( char c ) { return 0 != isxdigit( ( int ) c ); }

char char__tolower( char c ) { return ( char ) tolower( ( int ) c ); }
char char__toupper( char c ) { return ( char ) toupper( ( int ) c ); }
