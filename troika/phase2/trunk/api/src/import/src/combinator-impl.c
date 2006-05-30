#include <settings.h>
#include "combinator-impl.h"


Combinator *combinator__decode( cstring *buffer )
{
    Combinator *c = new( Combinator );
    *c = ( *buffer == 'S' )
        ? S_combinator
        : K_combinator;
    return c;
}


void combinator__encode( Combinator *c, cstring *buffer )
{
    switch ( *c )
    {
        case K_combinator:
            sprintf( buffer, "K" );
            break;
        case S_combinator:
            sprintf( buffer, "S" );
            break;
        case Y_combinator:
            sprintf( buffer, "Y" );
            break;
        default:
            abort();
    }
}
