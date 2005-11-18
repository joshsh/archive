#include "int.h"
#include <stdio.h>  // atoi, sprintf


void int__encode( int p, char *buffer )
{
    sprintf( buffer, "%d", p );
}


int int__decode( char *buffer )
{
    return atoi( buffer );
}

