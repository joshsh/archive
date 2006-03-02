#include "combinator.h"
#include <stdio.h>  /* sprintf */


combinator combinator__decode( char *buffer )
{
    if ( *buffer == 'S' )
        return S_combinator;
    else
        return K_combinator;
}


void combinator__encode( combinator c, char *buffer )
{
    if ( c == S_combinator )
        sprintf( buffer, "S" );
    else
        sprintf( buffer, "K" );
}
