#include "cstring.h"

#include <p2.h>


cstring *cstring__decode( char *buffer )
{
printf( "---cstring: buffer = \"%s\"", buffer );
    return STRDUP( buffer );
}


void cstring__encode( cstring *p, char *buffer )
{
    sprintf( buffer, p );
}
