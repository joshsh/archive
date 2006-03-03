/*******************************************************************************

Phase2 language API, Copyright (C) 2005 Joshua Shinavier.

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

#include "p2_ast.h"

#include <stdlib.h>  /* free */
#include <stdio.h>  /* fprintf, printf */


/** If defined, print terms as raw integer sequences, rather than as
    parenthesized expressions. */
/* #define PRINT_TERM_AS_ARRAY */


/* p2_ast__type ***************************************************************/


const char *p2_ast__type__names[7] =
{
    "BAG_T",
    "CHAR_T",
    "FLOAT_T",
    "INT_T",
    "NAME_T",
    "STRING_T",
    "TERM_T",
    "VOID_T"
};


const char *p2_ast__type__name( p2_ast__type type )
{
    return p2_ast__type__names[ type ];
}


/* Constructors, destructors **************************************************/


/** \note  Ownership of the passed value is conferred to the new node. */
static p2_ast *p2_ast__new( p2_ast__type type, void *value )
{
    p2_ast *node = ( p2_ast* ) malloc( sizeof( p2_ast ) );
    node->type = type;
    node->value = value;
    return node;
}


static void *p2_free( void *p )
{
    free( p );
    return ( void* ) 1;
}


static void *term_of_nodes__delete( p2_term *t )
{
    p2_term__for_all( t, ( void*(*)(void*) ) p2_ast__delete );
    p2_term__delete( t );
    return t;
}


p2_ast *p2_ast__bag( p2_array *bag )
{
    p2_ast *ast = p2_ast__new( BAG_T, bag );

    #if DEBUG__AST
    printf( "[%X] p2_ast__bag(%X)\n", ( int ) ast, ( int) bag );
    #endif

    return ast;
}


p2_ast *p2_ast__char( char c )
{
    char *p = ( char* ) malloc( sizeof( char ) );
    *p = c;
    p2_ast *ast = p2_ast__new( CHAR_T, p );

    #if DEBUG__AST
    printf( "[%X] p2_ast__char('%c')\n", ( int ) ast, c );
    #endif

    return ast;
}


p2_ast *p2_ast__float( double f )
{
    double *p = ( double* ) malloc( sizeof( double ) );
    *p = f;
    p2_ast *ast = p2_ast__new( FLOAT_T, p );

    #if DEBUG__AST
    printf( "[%X] p2_ast__float(%g)\n", ( int ) ast, f );
    #endif

    return ast;
}


p2_ast *p2_ast__int( int i )
{
    int *p = ( int* ) malloc( sizeof( int ) );
    *p = i;
    p2_ast *ast = p2_ast__new( INT_T, p );

    #if DEBUG__AST
    printf( "[%X] p2_ast__int(%X)\n", ( int ) ast, i );
    #endif

    return ast;
}


p2_ast *p2_ast__name( p2_name *name )
{
    p2_ast *ast = p2_ast__new( NAME_T, name );

    #if DEBUG__AST
    printf( "[%X] p2_ast__name(%X)\n", ( int ) ast, ( int) name );
    #endif

    return ast;
}


p2_ast *p2_ast__string( char *s )
{
    p2_ast *ast = p2_ast__new( STRING_T, s );

    #if DEBUG__AST
    printf( "[%X] p2_ast__string(%X)\n", ( int ) ast, ( int) s );
    #endif

    return ast;
}


p2_ast *p2_ast__term( p2_term *term )
{
    p2_ast *ast = p2_ast__new( TERM_T, term );

    #if DEBUG__AST
    printf( "[%X] p2_ast__term(%X)\n", ( int ) ast, ( int) term );
    #endif

    return ast;
}


p2_ast *p2_ast__void( void *p )
{
    p2_ast *ast = p2_ast__new( VOID_T, p );

    #if DEBUG__AST
    printf( "[%X] p2_ast__void(%X)\n", ( int ) ast, ( int ) p );
    #endif

    return ast;
}


int p2_ast__size( p2_ast *ast )
{
    switch ( ast->type )
    {
        case BAG_T:
            return ( ( p2_array* ) ast->value )->size;
        case TERM_T:
            return p2_term__length( ( p2_term* ) ast->value );
        default:
            return 1;
    }
}


void *p2_ast__delete( p2_ast *ast )
{
    #if DEBUG__AST
    printf( "[] p2_ast__delete(%X)\n", ( int ) ast );
    #endif

    switch( ast->type )
    {
        case BAG_T:

            p2_array__for_all(
                ( p2_array* ) ast->value,
                ( void*(*)(void*) ) term_of_nodes__delete );
            p2_array__delete( ( p2_array* ) ast->value );
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

            p2_array__for_all(
                ( p2_array* ) ast->value,
                p2_free );
            p2_array__delete( ( p2_array* ) ast->value );
            break;

        case STRING_T:

            free( ast->value );
            break;

        case TERM_T:

            p2_term__for_all(
                ( p2_term* ) ast->value,
                ( void*(*)(void*) ) p2_ast__delete );
            p2_term__delete( ( p2_term* ) ast->value );
            break;

        case VOID_T:

            /* Do nothing. */
            break;

        default:

            fprintf( stderr, "p2_ast__delete: bad AST type: %i\n", ast->type );
            return 0;
    }

    return p2_free( ast );
}


/* Printing *******************************************************************/


static void term__print( p2_term *term, int top_level )
{
    p2_term *subterm;

    #ifdef PRINT_TERM_AS_ARRAY
        void **cur = term->head, **sup = term->buffer + term->buffer_size;

        while ( cur < sup )
        {
            printf( "%d ", ( int ) *cur );
            cur++;
        }
    #else
        unsigned int i, length = p2_term__length( term );

        if ( length == 1 )
            p2_ast__print( ( p2_ast* ) *( term->head + 1 ) );
            /*printf((char *) *(term->head + 1)); */

        else
        {
            if ( !top_level )
                printf( "( " );
            /*else
                  printf( "[ " ); */

            for ( i = 0; i < length - 1; i++ )
            {
                subterm = p2_term__subterm_at( term, i );
                term__print( subterm, 0 );
                p2_term__delete( subterm );
                printf( " " );
            }

            subterm = p2_term__subterm_at( term, length - 1 );
            term__print( subterm, 0 );
            p2_term__delete( subterm );

            if ( !top_level )
                printf( " )" );
            /*else
                  printf( " ]" ); */
        }
    #endif  /* PRINT_TERM_AS_ARRAY */
}


static void bag__print( p2_array *a )
{
    int i, size = a->size;
    if ( !size )
        printf( "{null}" );
    else
    {
        if ( size > 1 )
            printf( "{ " );

        for ( i = 0; i < size; i++ )
        {
            if ( i )
                printf( ", " );
            term__print( ( p2_term* ) p2_array__get( a, i ), 1 );
        }

        if ( size > 1 )
            printf( " }" );
    }
}


void p2_ast__print( p2_ast *ast )
{
    char *s;

    switch( ast->type )
    {
        case BAG_T:

            bag__print( ( p2_array* ) ast->value );
            break;

        case CHAR_T:

            printf( "'%c'", *( ( char* ) ast->value ) );
            break;

        case FLOAT_T:

            printf( "%g", *( ( double* ) ast->value ) );
            break;

        case INT_T:

            printf( "%i", *( ( int* ) ast->value ) );
            break;

        case NAME_T:

            p2_name__print( ( p2_name* ) ast->value );
            break;

        case STRING_T:

            s = ( char* ) ast->value;
            printf( "\"" );
            while ( *s )
            {
                if ( *s == '"' || *s == '\\' )
                    printf( "\\" );

                printf( "%c", *s );
                s++;
            }
            printf( "\"" );
            break;

        case TERM_T:

            printf( "[ " );
            term__print( ( p2_term* ) ast->value, 1 );
            printf( " ]" );
            break;

        case VOID_T:

            printf( "0x%X", ( int ) ast->value );
            break;

        default:

            fprintf( stderr, "p2_ast__delete: bad AST type: %i\n", ast->type );
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
