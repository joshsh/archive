#include <settings.h>
#include "cstring.h"


cstring *
cstring__decode( char *buffer )
{
    return STRDUP( buffer );
}


void
cstring__encode( cstring *p, char *buffer )
{
    sprintf( buffer, p );
}
