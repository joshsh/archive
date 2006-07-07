#include <settings.h>
#include "int.h"


int *
int__decode( cstring *buffer )
{
    int *i = NEW( int );
    *i = atoi( buffer );
    return i;
}


void
int__encode( int *p, cstring *buffer )
{
    sprintf( buffer, "%d", *p );
}
