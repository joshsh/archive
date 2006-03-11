#include "char.h"
#include <stdio.h>  /* sprintf */


char char__decode( char *buffer )
{
    return *buffer;
}


void char__encode( char *c, char *buffer )
{
    sprintf( buffer, "'%c'", *c );
}
