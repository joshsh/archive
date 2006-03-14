#include "char.h"
#include <stdio.h>  /* sprintf */


char char__decode( cstring *buffer )
{
    return *buffer;
}


void char__encode( char *c, cstring *buffer )
{
    sprintf( buffer, "'%c'", *c );
}
