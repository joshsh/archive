#include <settings.h>
#include "combinator-impl.h"


Combinator *
combinator__decode( cstring *buffer )
{
    Combinator *c = NEW( Combinator );
    if ( !c )
        return 0;

    switch( *buffer )
    {
        case 'B':
            *c = B_combinator;
            break;
        case 'C':
            *c = C_combinator;
            break;
        case 'I':
            *c = I_combinator;
            break;
        case 'K':
            *c = K_combinator;
            break;
        case 'L':
            *c = L_combinator;
            break;
        case 'O':
            *c = O_combinator;
            break;
        case 'R':
            *c = R_combinator;
            break;
        case 'S':
            *c = S_combinator;
            break;
        case 'T':
            *c = T_combinator;
            break;
        case 'U':
            *c = U_combinator;
            break;
        case 'V':
            *c = V_combinator;
            break;
        case 'W':
            *c = W_combinator;
            break;
        case 'w':
            *c = w_combinator;
            break;
        case 'Y':
            *c = Y_combinator;
            break;
        default:
            ABORT;
    }

    return c;
}


void
combinator__encode( Combinator *c, cstring *buffer )
{
    switch ( *c )
    {
        case B_combinator:
            sprintf( buffer, "B" );
            break;
        case C_combinator:
            sprintf( buffer, "C" );
            break;
        case I_combinator:
            sprintf( buffer, "I" );
            break;
        case K_combinator:
            sprintf( buffer, "K" );
            break;
        case L_combinator:
            sprintf( buffer, "L" );
            break;
        case O_combinator:
            sprintf( buffer, "O" );
            break;
        case R_combinator:
            sprintf( buffer, "R" );
            break;
        case S_combinator:
            sprintf( buffer, "S" );
            break;
        case T_combinator:
            sprintf( buffer, "T" );
            break;
        case U_combinator:
            sprintf( buffer, "U" );
            break;
        case V_combinator:
            sprintf( buffer, "V" );
            break;
        case W_combinator:
            sprintf( buffer, "W" );
            break;
        case w_combinator:
            sprintf( buffer, "w" );
            break;
        case Y_combinator:
            sprintf( buffer, "Y" );
            break;
        default:
            ABORT;
    }
}
