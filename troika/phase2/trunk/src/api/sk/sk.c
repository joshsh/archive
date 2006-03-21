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

#include <sk/sk.h>
#include <p2_primitive.h>


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
    if ( ( unsigned int ) *term->head == 2 )
        term->head--;
    *( term->head ) = ( void * ) ( term->buffer + term->buffer_size - term->head );

    return term;
}


/* Sxyz --> xz(yz)
   [term size] [2]{S} [x_size]{x} [y_size]{y} [z_size]{z} ...
       --> [term size + z_size - 1] [x_size]{x} [z_size]{z} [y_size + z_size + 1] [y_size]{y} [z_size]{z} ... */
static p2_term *S_reduce( p2_term *term )
{
    void **x, **y, **z, **aux;
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
    aux = malloc( x_size * sizeof( void* ) );
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
    if ( ( unsigned int ) *term->head == 2 )
        term->head--;
    *( term->head ) = ( void* ) ( term->buffer + term->buffer_size - term->head );

    return term;
}


/* Assumes left-associative form.
   Note: it's probably worth trying to find a way to consolidate the type
   checking and garbage collection of arguments. */
static p2_term *prim_reduce( p2_term *term, p2_memory_manager *m )
{
    unsigned int i;
    Object *o;
    void *result, **args, **cur = term->head + 2;
    p2_primitive *prim = ( p2_primitive* ) ( ( Object* ) *cur )->value;
    Type *param_type;
printf( "---sk pr -2---\n" ); fflush( stdout );

    #if PRIM__ALLOW_NOARG_FUNCTIONS
    args = ( prim->arity )
        ? malloc( prim->arity * sizeof( void* ) )
        : 0;
    #else
    #if DEBUG__SAFE
    if ( !prim->arity )
    {
        ERROR( "prim_reduce: no parameters" );
        p2_term__delete( term );
        return 0;
    }
    #endif
        args = malloc( prim->arity * sizeof( void* ) );
    #endif
printf( "---sk pr -1---\n" ); fflush( stdout );

    /* Load arguments into the array. */
    for ( i = 0; i < prim->arity; i++ )
    {
printf( "---sk pr 0---\n" ); fflush( stdout );

        cur++;

        if ( ( unsigned int ) *cur != 2 )
        {
            #if SK__CHECKS__APPLY_TO_NONATOM

            ERROR( "prim_reduce: primitive applied to non-atom" );

            if ( args )
                free( args );
            p2_term__delete( term );

            return 0;

            #endif

            /*cur += ( unsigned int ) *cur;*/
        }

        else
            cur++;


        #if DEBUG__SAFE
        if ( !(*cur) )
        {
            ERROR( "prim_reduce: null argument" );

            if ( args )
                free( args );
            p2_term__delete( term );

            return 0;
        }
        #endif

        /* Note: it's more efficient to do this here than in p2_primitive.c */
        #if PRIM__CHECKS__PARAM_TYPE

        param_type = prim->parameters[i].type;
        if ( param_type != ( *( ( Object** ) cur ) )->type
          && param_type != any_type )
        {
            ERROR( "prim_reduce: argument type mismatch" );

            if ( args )
                free( args );
            p2_term__delete( term );

            return 0;
        }

        #endif  /* PRIM__CHECKS__PARAM_TYPE */

        /* ~ inefficient */
        if ( prim->parameters[i].type != any_type )
            args[i] = ( *( ( Object** ) cur ) )->value;
        else
            args[i] = *cur;
    }
printf( "---sk pr 0.1---\n" ); fflush( stdout );

    /* Apply the primitive. */
    result = prim->cstub( args );
printf( "---sk pr 0.2---\n" ); fflush( stdout );

    if ( args )
        free( args );

    #if !PRIM__ALLOW_VOID_FUNCTIONS
    if ( !result )
    {
        ERROR( "prim_reduce: null return value from primitive" );
        p2_term__delete( term );
        return 0;
    }
    #endif
printf( "---sk pr 1---\n" ); fflush( stdout );
printf( "prim->return_type = %#x\n", ( int ) prim->return_type );

    if ( prim->return_type != any_type )
    {
printf( "---sk pr 2a---\n" ); fflush( stdout );
        o = object__new( prim->return_type, result, 0 );

        /* Caution: the object's value must be a BRAND NEW value. */
        p2_memory_manager__add( m, o );
    }

    else
    {
printf( "---sk pr 2b---\n" ); fflush( stdout );
        o = ( Object* ) result;
    }
printf( "---sk pr 3---\n" ); fflush( stdout );

    /* Replace the primitive reference and its arguments with the return value. */
    *cur = o;
    cur--;
    *cur = ( void* ) 2;
    term->head = cur - 1;
    *( term->head ) = ( void* ) ( term->buffer + term->buffer_size - term->head );
printf( "---sk pr 4---\n" ); fflush( stdout );

    return term;
}


/* Expand the subterm at the head of another term. */
static p2_term *term_reduce( p2_term *term )
{
    unsigned int size, newsize;
    p2_term *head_term;

    if ( ( unsigned int ) *( term->head ) == 2 )
    {
        head_term = ( p2_term* ) ( ( Object* ) *( term->head + 1 ) )->value;
        p2_term__delete( term );
        return p2_term__copy( head_term );
    }

    else
    {
        head_term = ( p2_term* ) ( ( Object* ) *( term->head + 2 ) )->value;

        size = ( unsigned int ) *head_term->head;

        newsize = ( unsigned int ) *term->head + size - 2;
        if ( newsize > term->buffer_size )
            term = p2_term__expand( term, newsize );

        term->head = term->buffer + term->buffer_size - ( unsigned int ) *term->head + 3 - size;
        memcpy( term->head, head_term->buffer, size * sizeof( void* ) );

        if ( ( unsigned int ) *term->head == 2 )
            term->head--;
        *term->head = ( void* ) ( term->buffer + term->buffer_size - term->head );

        return term;
    }
}


p2_term *SK_reduce(
    p2_term *term,
    p2_memory_manager *m,
    Type *term_type,
    Type *primitive_type,
    Type *combinator_type,
    void (*for_each_iteration)(p2_term*) )
{
    #if SK__CHECKS__MAX_REDUX_ITERATIONS > 0
    int iter = 0;
    #endif

    Object *head;
    Type *head_type;

    #if DEBUG__SAFE
    if ( !term || !m || !primitive_type || !combinator_type )
    {
        ERROR( "SK_reduce: null argument" );
        if ( term )
            p2_term__delete( term );
        return 0;
    }
    #endif

    /* Iterate until the resulting term is in head-normal form. */
    for (;;)
    {
        #if DEBUG__SAFE
        if ( !term )
        {
            ERROR( "SK_reduce: null term" );
            return 0;
        }
        #endif

/*
cur = term->head; sup = term->buffer + term->buffer_size;
while ( cur < sup ) {
printf( " %x", ( int ) *cur ); cur++; }
printf( "\n" );  fflush( stdout );
*/
        #if SK__CHECKS__MAX_TERM_SIZE > 0
        if ( ( unsigned int ) *( term->head ) > SK__CHECKS__MAX_TERM_SIZE )
        {
            ERROR( "SK_reduce: reduction aborted (term might expand indefinitely)" );
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
            head = ( Object* ) *( term->head + 1 );
        else
            /* Left-associative sequence. */
            head = ( Object* ) *( term->head + 2 );

        #if DEBUG__SAFE
        if ( !head )
        {
            ERROR( "SK_reduce: null encountered at head of term" );
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

        /* If there is a term object at the head of the term being reduced,
           expand its value. */
        else if ( head_type == term_type )
        {
            term = term_reduce( term );
/*
            head_term = p2_term__copy( ( p2_term* ) head->value );
printf( "---sk r x 2---\n" ); fflush( stdout );

            size = ( unsigned int ) *term->head;
printf( "size = %i\n", *term->head );
            if ( size == 2 )
            {
printf( "---sk r x 3a---\n" ); fflush( stdout );
                p2_term__delete( term );
                term = head_term;
            }

            else
            {
printf( "---sk r x 3b---\n" ); fflush( stdout );
                term->head += 3;
                if ( ( unsigned int ) *term->head == 2 )
                {
                    if ( size > 5 )
                    {
                        term->head--;
                        *term->head = ( void* ) ( size - 2 );
printf( "size - 2 = %i\n", *term->head );
                    }

                    term = p2_term__cat( head_term, term );
                }

                else
                    term = p2_term__merge_la( head_term, term );
            }
*/
printf( "---sk r x 4---\n" ); fflush( stdout );
        }

        /* Any object which is not an S or K combinator, a term or a primitive
           is considered a non-redex object. */
        else
        {
            #if SK__ALLOW_NONREDUX

            /* Simply return the term as-is, without attempting to reduce it
               further. */
            return term;

            #else

            ERROR( "SK_reduce: non-redex objects not permitted at the head of a term" );

            /* Garbage-collect whatever is left of the term. */
            p2_term__delete( term );

            /* Fail. */
            return 0;

            #endif
        }

        #if SK__CHECKS__MAX_REDUX_ITERATIONS > 0
printf( "iter = %i\n", iter );
        if ( ++iter > SK__CHECKS__MAX_REDUX_ITERATIONS )
        {
            ERROR( "SK_reduce: reduction aborted (possible infinite loop)" );
            p2_term__delete( term );
            return 0;
        }
        #endif
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
