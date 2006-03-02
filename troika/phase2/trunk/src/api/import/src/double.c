#include "double.h"
#include <stdlib.h>  /* atof */
#include <stdio.h>  /* sprintf */


void double__encode( double p, char *buffer )
{
    sprintf( buffer, "%g", p );
}


double double__decode( char *buffer )
{
    return atof( buffer );
}

