#include "int.h"
#include <stdlib.h>  /* atoi */
#include <stdio.h>  /* sprintf */


int int__decode( char *buffer )
{
    return atoi( buffer );
}


void int__encode( int *p, char *buffer )
{
    sprintf( buffer, "%d", *p );
}
