/*******************************************************************************

Phase2 language API, Copyright (C) 2006 Joshua Shinavier.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

*******************************************************************************/

#include <Ast.h>
#include "settings.h"


/** If defined, print terms as raw integer sequences, rather than as
    parenthesized expressions. */
/* #define PRINT_TERM_AS_ARRAY */


/* Ast__Type ***************************************************************/


const char *Ast__Type__names[9] =
{
    "BAG_T",
    "CHAR_T",
    "FLOAT_T",
    "INT_T",
    "NAME_T",
    "NULL_T",
    "STRING_T",
    "TERM_T",
    "VOID_T"
};


const char *
Ast__Type__name( Ast__Type type )
{
    return Ast__Type__names[ type ];
}


/* Constructors, destructors **************************************************/


/** \note  Ownership of the passed value is conferred to the new node. */
static Ast *
ast__new( Ast__Type type, void *value )
{
    Ast *node = NEW( Ast );
    node->type = type;
    node->value = value;
    return node;
}


Ast *
ast__bag( Array *bag )
{
    Ast *ast = ast__new( BAG_T, bag );

    return ast;
}


Ast *
ast__char( char c )
{
    Ast *ast;
    char *p;

    p = NEW( char );
    *p = c;
    ast = ast__new( CHAR_T, p );

    return ast;
}


Ast *
ast__float( double f )
{
    Ast *ast;
    double *p;

    p = NEW( double );
    *p = f;
    ast = ast__new( FLOAT_T, p );

    return ast;
}


Ast *
ast__int( int i )
{
    Ast *ast;
    int *p;

    p = NEW( int );
    *p = i;
    ast = ast__new( INT_T, p );

    return ast;
}


Ast *
ast__name( Name *name )
{
    Ast *ast = ast__new( NAME_T, name );

    return ast;
}


Ast *
ast__null()
{
    Ast *ast = ast__new( NULL_T, 0 );

    return ast;
}


Ast *
ast__string( char *s )
{
    Ast *ast = ast__new( STRING_T, s );

    return ast;
}


Ast *
ast__term( Term *term )
{
    Ast *ast = ast__new( TERM_T, term );

    return ast;
}


Ast *
ast__void( void *p )
{
    Ast *ast = ast__new( VOID_T, p );

    return ast;
}


int
ast__size( Ast *ast )
{
    switch ( ast->type )
    {
        case BAG_T:
            return array__size( ( Array* ) ast->value );
        case TERM_T:
            return term__length( ( Term* ) ast->value );
        default:
            return 1;
    }
}


void
ast__delete( Ast *ast )
{
    void *helper( Ast **ast )
    {
        ast__delete( *ast );
        return 0;
    }

    switch( ast->type )
    {
        case BAG_T:

            array__walk( ast->value, ( Dist_f ) helper );
            array__delete( ( Array* ) ast->value );

            break;

        case CHAR_T:

            free( ast->value );
            break;

        case FLOAT_T:

            free( ast->value );
            break;

        case INT_T:

            free( ast->value );
            break;

        case NAME_T:

            name__delete( ( Name* ) ast->value );
            break;

        case NULL_T:

            break;

        case STRING_T:

            free( ast->value );
            break;

        case TERM_T:

            term__walk( ast->value, ( Dist_f ) helper );
            term__delete( ( Term* ) ast->value );

            break;

        case VOID_T:

            /* Do nothing. */
            break;

        default:

            fprintf( stderr, "ast__delete: bad AST type: %i\n", ast->type );
            return;
    }

    free( ast );
}


/* Printing *******************************************************************/


static void
term__print( Term *term, int top_level )
{
    Term *subterm;

    #ifdef PRINT_TERM_AS_ARRAY
        void **cur = term->head, **sup = term->buffer + term->buffer_size;

        while ( cur < sup )
        {
            PRINT( "%d ", ( int ) *cur );
            cur++;
        }
    #else
        unsigned int i, length = term__length( term );

        if ( length == 1 )
            ast__print( ( Ast* ) *( term->head + 1 ) );
            /*PRINT((char *) *(term->head + 1)); */

        else
        {
            if ( !top_level )
                PRINT( "(" );
            /*else
                  PRINT( "[ " ); */

            for ( i = 0; i < length - 1; i++ )
            {
                subterm = term__subterm_at( term, i );
                term__print( subterm, 0 );
                term__delete( subterm );
                PRINT( " " );
            }

            subterm = term__subterm_at( term, length - 1 );
            term__print( subterm, 0 );
            term__delete( subterm );

            if ( !top_level )
                PRINT( ")" );
            /*else
                  PRINT( " ]" ); */
        }
    #endif  /* PRINT_TERM_AS_ARRAY */
}


static void
bag__print( Array *a )
{
    int i, size = array__size( a );
    if ( !size )
        PRINT( "{}" );
    else
    {
        /*if ( size > 1 )*/
            PRINT( "{" );

        for ( i = 0; i < size; i++ )
        {
            if ( i )
                PRINT( ", " );
            ast__print( ( Ast* ) array__get( a, i ) );
        }

        /*if ( size > 1 )*/
            PRINT( "}" );
    }
}


void
ast__print( Ast *ast )
{
    char *s;

    switch( ast->type )
    {
        case BAG_T:

            bag__print( ( Array* ) ast->value );
            break;

        case CHAR_T:

            PRINT( "'%c'", *( ( char* ) ast->value ) );
            break;

        case FLOAT_T:

            PRINT( "%g", *( ( double* ) ast->value ) );
            break;

        case INT_T:

            PRINT( "%i", *( ( int* ) ast->value ) );
            break;

        case NAME_T:

            name__print( ( Name* ) ast->value );
            break;

        case NULL_T:

            PRINT( "()" );
            break;

        case STRING_T:

            s = ( char* ) ast->value;
            PRINT( "\"" );
            while ( *s )
            {
                if ( *s == '"' || *s == '\\' )
                    PRINT( "\\" );

                PRINT( "%c", *s );
                s++;
            }
            PRINT( "\"" );
            break;

        case TERM_T:

            PRINT( "[" );
            term__print( ( Term* ) ast->value, 1 );
            PRINT( "]" );
            break;

        case VOID_T:

            PRINT( "[void]" );
            /*PRINT( "%#x", ( int ) ast->value );*/
            break;

        default:

            fprintf( stderr, "ast__delete: bad AST type: %i\n", ast->type );
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
