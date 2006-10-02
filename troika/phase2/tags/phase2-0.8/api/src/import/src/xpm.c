#include <settings.h>
#include "xpm.h"


xpm *
xpm__decode( char *buffer )
{
    return STRDUP( buffer );
}


void
xpm__encode( xpm *p, char *buffer )
{
    sprintf( buffer, p );
}


xpm *
cstring__to_xpm( cstring *s )
{
    return STRDUP( s );
}
