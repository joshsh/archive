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
#include <util/Set.h>
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


/* Assumes left-associative form.
   Note: it's probably worth trying to find a way to consolidate the type
   checking and garbage collection of arguments. */
static Term *
prim_reduce(
    Term *term,
    Memory_Manager *m,
    Type *term_type,
    Type *primitive_type,
    Type *combinator_type,
    Type *set_type )
{
    Primitive *p;
    void **args, **cur, **max;

    Object *result = 0;
    Set *result_set = 0;

    Object *reduce_arg( Term *term )
    {
        Object *arg;
        term = sk_reduce( term, m, term_type, primitive_type, combinator_type, set_type );

        if ( term )
        {
            if ( term__length( term ) == 1 )
                arg = *( term->head + 1 );

            /* Fail with error message. */
            else
            {
                ERROR( "primitive applied to irreducible argument" );
                arg = 0;
            }

            term__delete( term );
        }

        /* Soft fail. */
        else
            arg = 0;

        return arg;
    }

    /* Apply the primitive. */
    void apply()
    {
        /* Caution: no exception handling at this level. */
        void *r = p->cstub( args );

        Object *robj;

        max = cur;

        if ( r )
        {
            if ( PRIM__ALLOW_GENERIC_RESULTS && p->return_type == any_type )
                /* Return value is an existing typed object. */
                robj = r;

            else
            {
                /* Return type is a raw data reference which needs to be bound to a type. */
                robj = object__new( p->return_type, r, NOPROPS );
                memory_manager__add( m, robj );
            }

            /* Multiple return values. */
            if ( result )
            {
                if ( !result_set )
                {
                    result_set = set__new();
                    set__add( result_set, result );
                    result = object__new( set_type, result_set, NOPROPS );
                    memory_manager__add( m, result );
                }

                set__add( result_set, robj );
            }

            /* Single (or first) return value. */
            else
                result = robj;
        }

        /* Soft fail if r is NULL. */
    }

    /* Map arguments to parameters. */
    void marshal( unsigned int i )
    {
        /* This may be one argument, or a set of arguments. */
        Object *arg;

        Type *param_type;
        void **temp;

        unsigned int j;

        /* Returns FALSE for success. */
        boolean curry( Object *a )
        {
            if ( PRIM__CHECKS__PARAM_TYPE )
            {
                if ( param_type != a->type
                  && ( !PRIM__ALLOW_GENERIC_PARAMS || param_type != any_type ) )
                {
                    ERROR( "prim_reduce: argument type mismatch" );
                    return TRUE;
                }
            }

            if ( PRIM__ALLOW_GENERIC_PARAMS && param_type == any_type )
                args[j] = a;

            else
                args[j] = a->value;


            return FALSE;
        }

        void *recurse( Object **argp )
        {
            if ( curry( *argp ) )
                return walker__break;

            else
            {
                temp = cur;
                marshal( j + 1 );
                cur = temp;

                return 0;
            }
        }

        /* Iterate. */
        for ( j = i; j < p->arity; j++ )
        {
            if ( PRIM__CHECKS__PARAM_TYPE || PRIM__ALLOW_GENERIC_PARAMS )
                param_type = p->parameters[j].type;

            if ( ( unsigned int ) *cur == 2 )
            {
                cur++;
                arg = *cur;
                cur++;
            }

            else
            {
                /* FIXME: creating a new term for each argument is quite a waste */
                arg = reduce_arg( term__subterm_at( term, j + 1 ) );

                cur += ( unsigned int ) *cur;
            }

            /* Abandon application along this argument path. */
            if ( !arg )
                return;

            /* Map the arguments. */
            /* FIXME: as it is, term references will be iterated through, rather than reduced */
            if ( PRIM__IMPLICIT_SET_MAPPING && ( arg->type->flags & TYPE__IS_OBJ_COLL )
              && ( arg->type != param_type ) )
            {
                arg->type->walk( arg->value, ( Dist_f ) recurse );
                return;
            }

            /* Single argument maps to itself. */
            else if ( curry( arg ) )
                return;
        }

        apply();
    }

    /* Pick out the primitive to be applied. */
    cur = term->head + 2;
    p = ( ( Object* ) *cur )->value;

    if ( PRIM__ALLOW_NOARG_FUNCTIONS )
    {
        args = ( p->arity )
            ? malloc( p->arity * sizeof( void* ) )
            : 0;
    }

    else
    {
        if ( DEBUG__SAFE && !p->arity )
            abort();

        args = malloc( p->arity * sizeof( void* ) );
    }

    /* Advance to the first argument. */
    cur++;

    marshal( 0 );

    if ( !result )
        goto fail;

    /* Note: cur is now just beyond the position of the last argument. */
    cur = max;

    /* Replace the primitive reference and its arguments with the return value. */
    cur--;
    *cur = result;
    cur--;
    *cur = ( void* ) 2;

    if ( term->buffer + term->buffer_size - cur > 2 )
        term->head = cur - 1;

    else
        term->head = cur;

    *( term->head ) = ( void* ) ( term->buffer + term->buffer_size - term->head );

    goto finish;

fail:

    term__delete( term );
    term = 0;

finish:

    if ( args )
        free( args );

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


Term *
sk_reduce(
    Term *term,
    Memory_Manager *m,
    Type *term_type,
    Type *primitive_type,
    Type *combinator_type,
    Type *set_type )
{
    unsigned int iter;

    if ( SK__CHECKS__MAX_REDUX_ITERATIONS > 0 )
        iter = 0;

    Object *head;
    Type *head_type;

    if ( DEBUG__SAFE && ( !term || !m || !term_type || !primitive_type || !combinator_type || !set_type ) )
        abort();

    if ( DEBUG__SK )
    {
        printf( "%i:\t", iter );
        print_term( term );
    }

    /* Iterate until the resulting term is in head-normal form. */
    for (;;)
    {
        if ( DEBUG__SAFE && !term )
            abort();

/*
cur = term->head; sup = term->buffer + term->buffer_size;
while ( cur < sup ) {
printf( " %x", ( int ) *cur ); cur++; }
printf( "\n" );  fflush( stdout );
*/
        /* Give up if the term becomes too large. */
        if ( SK__CHECKS__MAX_TERM_SIZE > 0
          && ( unsigned int ) *( term->head ) > SK__CHECKS__MAX_TERM_SIZE )
        {
            ERROR( "sk_reduce: abandoned (term might expand indefinitely)" );
            goto fail;
        }

        /* Singleton term. */
        if ( ( unsigned int ) *( term->head ) == 2 )
            head = *( term->head + 1 );

        /* Left-associative sequence. */
        else
            head = *( term->head + 2 );

        /* There should be no way for nulls to appear at the head of a term. */
        if ( DEBUG__SAFE && !head )
            abort();

        head_type = head->type;

        /* TODO: put these types in a hash table for more flexibility on what is reduced and how */

        /* If the head object is a primitive, apply it. */
        if ( head_type == primitive_type )
        {
            if ( term__length( term ) <= ( ( Primitive* ) head->value )->arity )
                goto finish;

            else
            {
                term = prim_reduce( term, m, term_type, primitive_type, combinator_type, set_type );

                /* Unless the application of a primitive is allowed to yield
                   another primitive (or an S or K combinator), the resulting
                   term cannot be further reduced. */
                if ( !PRIM__ALLOW_HIGHER_ORDER )
                    goto finish;
            }
        }

        else if ( head_type == combinator_type )
        {
            switch ( *( ( Combinator* ) head->value ) )
            {
                /* Sxyz... --> xz(yz)... */
                case S_combinator:

                    if ( term__length( term ) < 4 )
                        goto finish;

                    else
                        term = s_reduce( term );

                    break;

                /* Kxy... --> x... */
                case K_combinator:

                    if ( term__length( term ) < 3 )
                        goto finish;

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

#ifdef NOTDEF
        /* TODO: Distribute... */
        else if ( head_type == set_type )
        {

        }
#endif

        /* Any object which is not an S or K combinator, a term or a primitive
           is considered a non-redex object. */
        else
        {
            /* Simply return the term as-is, without attempting to reduce it
               further. */
            if ( SK__ALLOW_NONREDUX )
                goto finish;

            else
            {
                ERROR( "sk_reduce: non-redex objects not permitted at the head of a term" );
                goto fail;
            }
        }

        /* Give up if reduction takes too long. */
        if ( SK__CHECKS__MAX_REDUX_ITERATIONS > 0 )
        {
            iter++;

            if ( DEBUG__SK )
            {
                printf( "%i:\t", iter );
                print_term( term );
            }

            if ( iter > SK__CHECKS__MAX_REDUX_ITERATIONS )
            {
                ERROR( "sk_reduce: abandoned (possible infinite loop)" );
                goto fail;
            }
        }
    }

fail:

    term__delete( term );
    term = 0;

finish:

    return term;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
