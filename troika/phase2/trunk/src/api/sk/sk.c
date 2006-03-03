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

#include "sk.h"
#include "../p2_primitive.h"

#include <stdlib.h>  /* malloc */
#include <string.h>  /* memcpy */





/* Pointer to a function which generates output as terms are reduced. */
void ( *debug_print_ )( p2_term* );





/* Kxy --> x
   [term size] [2]{K} [x_size]{x} [y_size]{y} ...
       --> [term size - y_size - 2] [x_size]{x} ... */
static p2_term *K_reduce( p2_term *term )
{
    void **x, **y;
    unsigned int x_size, y_size;

    /* Skip the 'K' to reach the head of the 'x' sub-term. */
    x = term->head + 3;
    x_size = ( unsigned int ) *x;

    /* Skip the 'x' and to reach the head of the 'y' sub-term. */
    y = x + x_size;
    y_size = ( unsigned int ) *y;

    /* Copy the 'x' subterm to the target position. */
    term->head = y + ( y_size - x_size );
    memcpy( term->head, x, x_size * sizeof( void* ) );

    /* Reset the term head. */
    term->head--;
    *( term->head ) = ( void * ) ( term->buffer + term->buffer_size - term->head );

    return term;
}


/* Sxyz --> xz(yz)
   [term size] [2]{S} [x_size]{x} [y_size]{y} [z_size]{z} ...
       --> [term size + z_size - 1] [x_size]{x} [z_size]{z} [y_size + z_size + 1] [y_size]{y} [z_size]{z} ... */
static p2_term *S_reduce( p2_term *term )
{
    void **x, **y, **z;
    unsigned int x_size, y_size, z_size, newsize;

    /* Locate the head of 'x', 'y' and 'z'. */
    x = term->head + 3;
    x_size = ( unsigned int ) *x;
    y = x + x_size;
    y_size = ( unsigned int ) *y;
    z = y + y_size;
    z_size = ( unsigned int ) *z;

    /* Predict the size of the resulting array.  If necessary, copy the term to a
       larger buffer and reduce that instead. */
    newsize = ( unsigned int ) *( term->head ) + z_size - 1;
    if ( newsize > term->buffer_size )
    {
        term = p2_term__expand( term, newsize );

        /* Re-locate the head of 'x', 'y' and 'z'. */
        x = term->head + 3;
        x_size = ( unsigned int ) *x;
        y = x + x_size;
        y_size = ( unsigned int ) *y;
        z = y + y_size;
        z_size = ( unsigned int ) *z;
    }

    /* Copy 'x' to an auxiliary array. */
    void **aux = ( void ** ) malloc( x_size * sizeof( void* ) );
    memcpy( aux, x, x_size * sizeof( void* ) );

    /* Prepend a term head for the new sub-term 'yz'.  The data of 'y' and 'z'
       remains where it is. */
    term->head = y - 1;
    *( term->head ) = ( void* ) ( y_size + z_size + 1 );

    /* Prepend a duplicate 'z'. */
    term->head -= z_size;
    memcpy( term->head, z, z_size * sizeof( void* ) );

    /* Prepend an 'x'. */
    term->head -= x_size;
    memcpy( term->head, aux, x_size * sizeof( void* ) );
    free( aux );

    /* Prepend a term head. */
    term->head--;
    *( term->head ) = ( void* ) ( term->buffer + term->buffer_size - term->head );

    return term;
}


/* Assumes left-associative form.
   Note: it's probably worth trying to find a way to consolidate the type
   checking and garbage collection of arguments. */
static p2_term *prim_reduce( p2_term *term, p2_memory_manager *m )
{
    int i;
    p2_object *o;
    void *result, **args, **cur = term->head + 2;
    p2_primitive *prim = ( p2_primitive* ) ( ( p2_object* ) *cur )->value;

    #if PRIM__ALLOW_NOARG_FUNCTIONS
    args = ( prim->arity )
        ? ( void** ) malloc( prim->arity * sizeof( void* ) )
        : 0;
    #else
    #if DEBUG__SAFE
    if ( !prim->arity )
    {
        PRINTERR( "prim_reduce: no parameters" );
        p2_term__delete( term );
        return 0;
    }
    #endif
        args = ( void** ) malloc( prim->arity * sizeof( void* ) );
    #endif

    /* Load arguments into the array. */
    for ( i = 0; i < prim->arity; i++ )
    {
        #if SK__CHECKS__APPLY_TO_NONATOM

        cur++;

        if ( ( unsigned int ) *cur != 2 )
        {
            PRINTERR( "prim_reduce: primitive applied to non-atom" );

            if ( args )
                free( args );
            p2_term__delete( term );

            return 0;
        }

        cur++;

        #else

        cur += 2;

        #endif  /* SK__CHECKS__APPLY_TO_NONATOM */

        #if DEBUG__SAFE
        if ( !(*cur) )
        {
            PRINTERR( "prim_reduce: null argument" );

            if ( args )
                free( args );
            p2_term__delete( term );

            return 0;
        }
        #endif

        /* Note: it's more efficient to do this here than in p2_primitive.c */
        #if PRIM__CHECKS__PARAM_TYPE

        if ( ( *( ( p2_object** ) cur ) )->type != prim->parameters[i].type )
        {
            PRINTERR( "prim_reduce: argument type mismatch" );

            if ( args )
                free( args );
            p2_term__delete( term );

            return 0;
        }

        #endif  /* PRIM__CHECKS__TYPE__DYNAMIC */

        args[i] = ( *( ( p2_object** ) cur ) )->value;
    }

    /* Apply the primitive. */
    result = prim->cstub( args );

    if ( args )
        free( args );

    #if !PRIM__ALLOW_VOID_FUNCTIONS
    if ( !result )
    {
        PRINTERR( "prim_reduce: null return value from primitive" );
        p2_term__delete( term );
        return 0;
    }
    #endif

    o = p2_object__new( prim->return_type, result, 0 );

    /* Caution: the object's value must be a BRAND NEW value. */
    p2_memory_manager__add( m, o );

    /* Replace the primitive reference and its arguments with the return value. */
    *cur = o;
    cur--;
    *cur = ( void* ) 2;
    term->head = cur - 1;
    *( term->head ) = ( void* ) ( term->buffer + term->buffer_size - term->head );

    return term;
}


p2_term *SK_reduce(
    p2_term *term,
    p2_memory_manager *m,
    p2_type *primitive_type,
    p2_type *combinator_type,
    void (*for_each_iteration)(p2_term*) )
{
    #if SK__CHECKS__MAX_REDUX_ITERATIONS > 0
        int iter = 0;
    #endif

    p2_object *head;
    p2_type *head_type;

    #if DEBUG__SAFE
    if ( !term || !m || !primitive_type || !combinator_type )
    {
        PRINTERR( "SK_reduce: null argument" );
        if ( term )
            p2_term__delete( term );
        return 0;
    }
    #endif

    /* Iterate until the resulting term is in head-normal form. */
    for (;;)
    {
        #if SK__CHECKS__MAX_TERM_SIZE > 0
        if ( ( unsigned int ) *( term->head ) > SK__CHECKS__MAX_TERM_SIZE )
        {
            PRINTERR( "SK_reduce: reduction aborted (term might expand indefinitely)" );
            p2_term__delete( term );
            return 0;
        }
        #endif

        #if DEBUG__SK
        if ( for_each_iteration )
            for_each_iteration( term );
        #endif

        /* Get the object at the head of the term.
           Caution: the term MUST be in left-associative form. */
        if ( ( unsigned int ) *( term->head ) == 2 )
            /* Singleton term. */
            head = ( p2_object* ) *( term->head + 1 );
        else
            /* Left-associative sequence. */
            head = ( p2_object* ) *( term->head + 2 );

        #if DEBUG__SAFE
        if ( !head )
        {
            PRINTERR( "SK_reduce: null encountered at head of term" );
            p2_term__delete( term );
            return 0;
        }
        #endif

        head_type = head->type;

        /* If the head object is a primitive, apply it. */
        if ( head_type == primitive_type )
        {
            if ( p2_term__length( term ) <= ( ( p2_primitive* ) head->value )->arity )
                return term;
            else
            {
                term = prim_reduce( term, m );

                /* Unless the application of a primitive is allowed to yield
                   another primitive (or an S or K combinator), the resulting
                   term cannot be further reduced. */
                #if !PRIM__ALLOW_HIGHER_ORDER
                return term;
                #endif
            }
        }

        else if ( head_type == combinator_type )
        {
            switch ( *( ( combinator* ) head->value ) )
            {
                /* Sxyz... --> xz(yz)... */
                case S_combinator:

                    if ( p2_term__length( term ) < 4 )
                        return term;
                    else
                        term = S_reduce( term );
                    break;

                /* Kxy... --> x... */
                case K_combinator:

                    if ( p2_term__length( term ) < 3 )
                        return term;
                    else
                        term = K_reduce( term );
                    break;
            }
        }

        /* Any object which is not an S or K combinator or a primitive is
           considered a non-redex object. */
        else
        {
            #if SK__ALLOW_NONREDUX

                /* Simply return the term as-is, without attempting to reduce it
                   further. */
                return term;

            #else

                PRINTERR( "SK_reduce: non-redex objects not permitted at the head of a term" );

                /* Garbage-collect whatever is left of the term. */
                p2_term__delete( term );

                /* Fail. */
                return 0;

            #endif
        }

        #if SK__CHECKS__MAX_REDUX_ITERATIONS > 0
        if ( ++iter > SK__CHECKS__MAX_REDUX_ITERATIONS )
        {
            PRINTERR( "SK_reduce: reduction aborted (possible infinite loop)" );
            p2_term__delete( term );
            return 0;
        }
        #endif

    };
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
