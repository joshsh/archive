#include "combinator.h"
#include <stdio.h>  /* sprintf */


combinator combinator__decode( cstring *buffer )
{
    if ( *buffer == 'S' )
        return S_combinator;
    else
        return K_combinator;
}


void combinator__encode( combinator *c, cstring *buffer )
{
    if ( *c == S_combinator )
        sprintf( buffer, "S" );
    else
        sprintf( buffer, "K" );
}
