#include "double.h"
#include <stdlib.h>  /* atof */
#include <stdio.h>  /* sprintf */


double *double__decode( cstring *buffer )
{
    double *d = new( double );
    *d = atof( buffer );
    return d;
}


void double__encode( double *p, cstring *buffer )
{
    sprintf( buffer, "%g", *p );
}
