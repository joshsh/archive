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
#include "../Object-impl.h"
#include "../Primitive-impl.h"


/* Kxy --> x
   [term size] [2]{K} [x_size]{x} [y_size]{y} ...
       --> [term size - y_size - 2] [x_size]{x} ... */
static Term *
k_reduce( Term *term )
{
    void **x, **y, **aux;
    unsigned int x_size, y_size;

    /* Skip the 'K' to reach the head of the 'x' sub-term. */
    x = term->head + 3;
    x_size = ( unsigned int ) *x;

    /* Skip the 'x' and to reach the head of the 'y' sub-term. */
    y = x + x_size;
    y_size = ( unsigned int ) *y;

    /* Copy the 'x' subterm to the target position. */
    aux = malloc( x_size * sizeof( void* ) );
    memcpy( aux, x, x_size * sizeof( void* ) );
    term->head = y + ( y_size - x_size );
    memcpy( term->head, aux, x_size * sizeof( void* ) );
    free( aux );

    /* Reset the term head. */
    if ( ( unsigned int ) *term->head == 2
      && ( term->buffer + term->buffer_size - term->head > 2 ) )
        term->head--;
    *( term->head ) = ( void * ) ( term->buffer + term->buffer_size - term->head );

    return term;
}


/* Sxyz --> xz(yz)
   [term size] [2]{S} [x_size]{x} [y_size]{y} [z_size]{z} ...
       --> [term size + z_size - 1] [x_size]{x} [z_size]{z} [y_size + z_size + 1] [y_size]{y} [z_size]{z} ... */
static Term *
s_reduce( Term *term )
{
    void **x, **y, **z, **aux;
    unsigned int x_size, y_size, z_size, newsize, temp;

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
        term = term__expand( term, newsize );

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
    temp = y_size + z_size;
    term->head = y;
    if ( ( unsigned int ) *term->head == 2 )
    {
        term->head--;
        temp++;
    }
    *( term->head ) = ( void* ) temp;

    /* Prepend a duplicate 'z'. */
    term->head -= z_size;
    memcpy( term->head, z, z_size * sizeof( void* ) );

    /* Prepend an 'x'. */
    term->head -= x_size;
    memcpy( term->head, aux, x_size * sizeof( void* ) );
    free( aux );

    /* Prepend a term head. */
    if ( ( unsigned int ) *term->head == 2
      && ( term->buffer + term->buffer_size - term->head > 2 ) )
        term->head--;
    *( term->head ) = ( void* ) ( term->buffer + term->buffer_size - term->head );

    return term;
}


/*
static void **
prim_args( Primitive *prim, void **args )
{

}
*/

static Object *
reduce_arg( Term *term,
    Memory_Manager *m,
    Type *term_type,
    Type *primitive_type,
    Type *combinator_type )
{
    term = sk_reduce( term, m, term_type, primitive_type, combinator_type );

    if ( term && term__length( term ) == 1 )
        return *( term->head + 1 );
    else
        return 0;
}


/* Assumes left-associative form.
   Note: it's probably worth trying to find a way to consolidate the type
   checking and garbage collection of arguments. */
static Term *
prim_reduce( Term *term,
    Memory_Manager *m,
    Type *term_type,
    Type *primitive_type,
    Type *combinator_type )
{
    unsigned int i;
    Object *o, *arg;
    void *result, **args, **cur = term->head + 2;
    Primitive *prim = ( ( Object* ) *cur )->value;
    Type *param_type;

    if ( PRIM__ALLOW_NOARG_FUNCTIONS )
    {
        args = ( prim->arity )
            ? malloc( prim->arity * sizeof( void* ) )
            : 0;
    }

    else
    {
        if ( DEBUG__SAFE && !prim->arity )
        {
            ERROR( "prim_reduce: no parameters" );
            term__delete( term );
            return 0;
        }

        args = malloc( prim->arity * sizeof( void* ) );
    }

    cur++;

    /* Load arguments into the array. */
    for ( i = 0; i < prim->arity; i++ )
    {
        if ( ( unsigned int ) *cur != 2 )
        {
            arg = reduce_arg( term__subterm_at( term, i + 1 ), m, term_type, primitive_type, combinator_type );

            /*#if SK__CHECKS__APPLY_TO_NONATOM*/

            if ( !arg )
            {
                ERROR( "prim_reduce: primitive applied to non-reducing term" );

                if ( args )
                    free( args );
                term__delete( term );

                return 0;
            }

            /*#endif*/

            cur += ( unsigned int ) *cur;
        }

        else
        {
            cur++;
            arg = *cur;
            cur++;

            if ( DEBUG__SAFE && !arg )
            {
                ERROR( "prim_reduce: null argument" );

                if ( args )
                    free( args );
                term__delete( term );

                return 0;
            }
        }

        /* Note: it's more efficient to do this here than in Primitive.c */
        if ( PRIM__CHECKS__PARAM_TYPE )
        {
            param_type = prim->parameters[i].type;
            if ( param_type != arg->type && param_type != any_type )
            {
                ERROR( "prim_reduce: argument type mismatch" );

                if ( args )
                    free( args );
                term__delete( term );

                return 0;
            }
        }

        /* ~ inefficient */
        if ( prim->parameters[i].type != any_type )
            args[i] = arg->value;
        else
            args[i] = arg;
    }

    /* Apply the primitive. */
    result = prim->cstub( args );

    if ( args )
        free( args );

    if ( !PRIM__ALLOW_VOID_FUNCTIONS && !result )
    {
        ERROR( "prim_reduce: null return value from primitive" );
        term__delete( term );
        return 0;
    }

    if ( prim->return_type != any_type )
    {
        o = object__new( prim->return_type, result, 0 );

        /* Caution: the object's value must be a BRAND NEW value. */
        memory_manager__add( m, o );
    }

    else
    {
        o = result;
    }

    /* Replace the primitive reference and its arguments with the return value. */
    cur--;
    *cur = o;
    cur--;
    *cur = ( void* ) 2;
    if ( term->buffer + term->buffer_size - cur > 2 )
        term->head = cur - 1;
    else
        term->head = cur;

    *( term->head ) = ( void* ) ( term->buffer + term->buffer_size - term->head );

    return term;
}


/* Expand the subterm at the head of another term. */
static Term *
term_reduce( Term *term )
{
    unsigned int size, newsize;
    Term *head_term;

    if ( ( unsigned int ) *( term->head ) == 2 )
    {
        head_term = ( ( Object* ) *( term->head + 1 ) )->value;
        term__delete( term );
        return term__copy( head_term );
    }

    else
    {
        head_term = ( ( Object* ) *( term->head + 2 ) )->value;

        size = ( unsigned int ) *head_term->head;

        newsize = ( unsigned int ) *term->head + size - 2;
        if ( newsize > term->buffer_size )
            term = term__expand( term, newsize );

        /*term->head = term->buffer;*/
        /*term->head = term->buffer + term->buffer_size - ( unsigned int ) *term->head + 3 - size;*/
        term->head += 3 - size;

        memcpy( term->head, head_term->head, size * sizeof ( void* ) );

        if ( ( unsigned int ) *term->head == 2 )
            term->head--;
        *term->head = ( void* ) ( term->buffer + term->buffer_size - term->head );

        return term;
    }
}


#if DEBUG__SK
static void
print_term( Term *t )
{
    void **cur = t->head, **lim = cur + ( unsigned int ) *cur;
    unsigned int i;

    while ( cur < lim )
    {
        i = ( unsigned int ) *cur;
        if ( i < 0x1000 )
            printf( " %i", i );
        else
            printf( " %#x", i );
        cur++;
    }

    printf( "\n" );
}
#endif


Term *
sk_reduce(
    Term *term,
    Memory_Manager *m,
    Type *term_type,
    Type *primitive_type,
    Type *combinator_type )
{
    #if SK__CHECKS__MAX_REDUX_ITERATIONS > 0
    int iter = 0;
    #endif

    Object *head;
    Type *head_type;

    #if DEBUG__SAFE
    if ( !term || !m || !primitive_type || !combinator_type )
    {
        ERROR( "sk_reduce: null argument" );
        if ( term )
            term__delete( term );
        return 0;
    }
    #endif

    #if DEBUG__SK
    printf( "%i:\t", iter );
    print_term( term );
    #endif

    /* Iterate until the resulting term is in head-normal form. */
    for (;;)
    {
        #if DEBUG__SAFE
        if ( !term )
        {
            ERROR( "sk_reduce: null term" );
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
            ERROR( "sk_reduce: reduction aborted (term might expand indefinitely)" );
            term__delete( term );
            return 0;
        }
        #endif

        /* Get the object at the head of the term.
           Caution: the term MUST be in left-associative form. */
        if ( ( unsigned int ) *( term->head ) == 2 )
            /* Singleton term. */
            head = *( term->head + 1 );
        else
            /* Left-associative sequence. */
            head = *( term->head + 2 );

        #if DEBUG__SAFE
        if ( !head )
        {
            ERROR( "sk_reduce: null encountered at head of term" );
            term__delete( term );
            return 0;
        }
        #endif

        head_type = head->type;

        /* If the head object is a primitive, apply it. */
        if ( head_type == primitive_type )
        {
            if ( term__length( term ) <= ( ( Primitive* ) head->value )->arity )
                return term;

            else
            {
                term = prim_reduce( term, m, term_type, primitive_type, combinator_type );

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
            switch ( *( ( Combinator* ) head->value ) )
            {
                /* Sxyz... --> xz(yz)... */
                case S_combinator:

                    if ( term__length( term ) < 4 )
                        return term;
                    else
                        term = s_reduce( term );
                    break;

                /* Kxy... --> x... */
                case K_combinator:

                    if ( term__length( term ) < 3 )
                        return term;
                    else
                        term = k_reduce( term );
                    break;
            }
        }

        /* If there is a term object at the head of the term being reduced,
           expand its value. */
        else if ( head_type == term_type )
        {
            term = term_reduce( term );
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

            ERROR( "sk_reduce: non-redex objects not permitted at the head of a term" );

            /* Garbage-collect whatever is left of the term. */
            term__delete( term );

            /* Fail. */
            return 0;

            #endif
        }

        #if SK__CHECKS__MAX_REDUX_ITERATIONS > 0

        iter++;

        #if DEBUG__SK
        printf( "%i:\t", iter );
        print_term( term );
        #endif

        if ( iter > SK__CHECKS__MAX_REDUX_ITERATIONS )
        {
            ERROR( "sk_reduce: reduction aborted (possible infinite loop)" );
            term__delete( term );
            return 0;
        }
        #endif
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
