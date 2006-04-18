#include <settings.h>
#include "toys.h"


int system_call( cstring *s )
{
    return system( s );
}


double double__inverse( double a )
{
    return -a;
}


double double_double__add( double a, double b )
{
    return a + b;
}


double double_double__subtract( double a, double b )
{
    return a - b;
}


double double_double__multiply( double a, double b )
{
    return a * b;
}


double double_double__divide( double a, double b )
{
    if ( !b )
    {
        ERROR( "double_double__divide: division by zero" );
        return 0;
    }

    else
        return a / b;
}


int int__inverse( int a )
{
    return -a;
}


int int_int__add( int a, int b )
{
    return a + b;
}


int int_int__subtract( int a, int b )
{
    return a - b;
}


int int_int__multiply( int a, int b )
{
    return a * b;
}


int int_int__divide( int a, int b )
{
    if ( !b )
    {
        ERROR( "int_int__divide: division by zero" );
        return 0;
    }

    else
        return a / b;
}


int int_int__modulus( int a, int b )
{
    if ( !b )
    {
        ERROR( "int_int__modulus: mod zero" );
        return 0;
    }

    else
        return a % b;
}
