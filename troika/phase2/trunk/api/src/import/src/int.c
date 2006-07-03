#include <settings.h>
#include "int.h"


int *
int__decode( cstring *buffer )
{
    int *i = new( int );
    *i = atoi( buffer );
    return i;
}


void
int__encode( int *p, cstring *buffer )
{
    sprintf( buffer, "%d", *p );
}


int
int__inverse( int a )
{
    return -a;
}


int
int_int__add( int a, int b )
{
    return a + b;
}


int
int_int__subtract( int a, int b )
{
    return a - b;
}


int
int_int__multiply( int a, int b )
{
    return a * b;
}


int
int_int__divide( int a, int b )
{
    if ( !b )
    {
        ERROR( "int_int__divide: division by zero" );
        return 0;
    }

    else
        return a / b;
}


int
int_int__modulus( int a, int b )
{
    if ( !b )
    {
        ERROR( "int_int__modulus: mod zero" );
        return 0;
    }

    else
        return a % b;
}

