#include "cstring.h"
#include <string.h>  /* strdup */
#include <stdio.h>  /* sprintf */


cstring *cstring__decode( char *buffer )
{
    return strdup( buffer );
}


void cstring__encode( cstring *p, char *buffer )
{
    sprintf( buffer, "\"%s\"", p );
}
