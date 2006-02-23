#include "cstring.h"
#include <string.h>  /* strdup */
#include <stdio.h>  /* sprintf */


void cstring__encode( char *p, char *buffer )
{
    sprintf(buffer, p);
}


char *cstring__decode( char *buffer )
{
    return strdup(buffer);
}

