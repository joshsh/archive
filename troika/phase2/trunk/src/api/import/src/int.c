#include "int.h"
#include <stdlib.h>  /* atoi */
#include <stdio.h>  /* sprintf */


int int__decode( cstring *buffer )
{
    return atoi( buffer );
}


void int__encode( int *p, cstring *buffer )
{
    sprintf( buffer, "%d", *p );
}
