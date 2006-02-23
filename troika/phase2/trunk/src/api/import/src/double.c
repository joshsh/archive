#include "double.h"
#include <stdio.h>  /* atof */


void double__encode( double p, char *buffer )
{
    sprintf( buffer, "%g", p );
}


double double__decode( char *buffer )
{
    return atof( buffer );
}

