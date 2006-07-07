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
