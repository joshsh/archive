#include "combinator.h"
#include <stdio.h>  /* sprintf */


combinator *combinator__decode( cstring *buffer )
{
    combinator *c = new( combinator );
    *c = ( *buffer == 'S' )
        ? S_combinator
        : K_combinator;
    return c;
}


void combinator__encode( combinator *c, cstring *buffer )
{
    if ( *c == S_combinator )
        sprintf( buffer, "S" );
    else
        sprintf( buffer, "K" );
}
