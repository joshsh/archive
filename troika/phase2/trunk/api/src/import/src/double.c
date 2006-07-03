#include <settings.h>
#include "double.h"


double *
double__decode( cstring *buffer )
{
    double *d = new( double );
    *d = atof( buffer );
    return d;
}


void
double__encode( double *p, cstring *buffer )
{
    sprintf( buffer, "%g", *p );
}


double
double__inverse( double a )
{
    return -a;
}


double
double_double__add( double a, double b )
{
    return a + b;
}


double
double_double__subtract( double a, double b )
{
    return a - b;
}


double
double_double__multiply( double a, double b )
{
    return a * b;
}


double
double_double__divide( double a, double b )
{
    if ( !b )
    {
        ERROR( "double_double__divide: division by zero" );
        return 0;
    }

    else
        return a / b;
}

