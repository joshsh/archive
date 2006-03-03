#include "cstring.h"
#include <string.h>  /* strdup */
#include <stdio.h>  /* sprintf */


void cstring__encode( cstring *p, char *buffer )
{
    sprintf(buffer, p);
}


cstring *cstring__decode( char *buffer )
{
    return strdup(buffer);
}

