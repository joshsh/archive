#include "double.h"
#include <stdlib.h>  /* atof */
#include <stdio.h>  /* sprintf */


double double__decode( char *buffer )
{
    return atof( buffer );
}


void double__encode( double *p, char *buffer )
{
    sprintf( buffer, "%g", *p );
}
