#include "combinator.h"
#include <stdio.h>  /* sprintf */


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
    if ( *c == S_combinator )
        sprintf( buffer, "S" );
    else
        sprintf( buffer, "K" );
}
