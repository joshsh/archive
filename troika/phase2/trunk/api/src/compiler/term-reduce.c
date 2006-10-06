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

#include <phase2/compiler.h>
#include <phase2/Set.h>
#include "../object/Object-impl.h"
#include "../primitive/Primitive-impl.h"
#include "../collection/Term-impl.h"


/* Kxy --> x
   [term size] [2]{K} [x_size]{x} [y_size]{y} ...
       --> [term size - y_size - 2] [x_size]{x} ... */
static Term *
k_reduce( Term *term )
{
    term_cell *x, *y, *aux;
    unsigned int x_size, y_size;

    /* Skip the 'K' to reach the head of the 'x' sub-term. */
    x = term->head + 3;
    x_size = x->intval;

    /* Skip the 'x' and to reach the head of the 'y' sub-term. */
    y = x + x_size;
    y_size = y->intval;

    /* Copy the 'x' subterm to the target position. */
    aux = malloc( x_size * sizeof( term_cell ) );
    memcpy( aux, x, x_size * sizeof( term_cell ) );
    term->head = y + ( y_size - x_size );
    memcpy( term->head, aux, x_size * sizeof( term_cell ) );
    free( aux );

    /* Reset the term head. */
    if ( term->head->intval == 2
      && ( term->buffer + term->buffer_size - term->head > 2 ) )
        term->head--;
    term->head->intval = term->buffer + term->buffer_size - term->head;

    return term;
}


/* Sxyz --> xz(yz)
   [term size] [2]{S} [x_size]{x} [y_size]{y} [z_size]{z} ...
       --> [term size + z_size - 1] [x_size]{x} [z_size]{z} [y_size + z_size + 1] [y_size]{y} [z_size]{z} ... */
static Term *
s_reduce( Term *term )
{
    term_cell *x, *y, *z, *aux;
    unsigned int x_size, y_size, z_size, newsize, temp;

    /* Locate the head of 'x', 'y' and 'z'. */
    x = term->head + 3;
    x_size = x->intval;
    y = x + x_size;
    y_size = y->intval;
    z = y + y_size;
    z_size = z->intval;

    /* Predict the size of the resulting array.  If necessary, copy the term to a
       larger buffer and reduce that instead. */
    newsize = term->head->intval + z_size - 1;
    if ( newsize > term->buffer_size )
    {
        term = term__expand( term, newsize );

        /* Re-locate the head of 'x', 'y' and 'z'. */
        x = term->head + 3;
        x_size = x->intval;
        y = x + x_size;
        y_size = y->intval;
        z = y + y_size;
        z_size = z->intval;
    }

    /* Copy 'x' to an auxiliary array. */
    aux = malloc( x_size * sizeof( term_cell ) );
    memcpy( aux, x, x_size * sizeof( term_cell ) );

    /* Prepend a term head for the new sub-term 'yz'.  The data of 'y' and 'z'
       remains where it is. */
    temp = y_size + z_size;
    term->head = y;
    if ( term->head->intval == 2 )
    {
        term->head--;
        temp++;
    }
    term->head->intval = temp;

    /* Prepend a duplicate 'z'. */
    term->head -= z_size;
    memcpy( term->head, z, z_size * sizeof( term_cell ) );

    /* Prepend an 'x'. */
    term->head -= x_size;
    memcpy( term->head, aux, x_size * sizeof( term_cell ) );
    free( aux );

    /* Prepend a term head. */
    if ( term->head->intval == 2
      && ( term->buffer + term->buffer_size - term->head > 2 ) )
        term->head--;
    term->head->intval = term->buffer + term->buffer_size - term->head;

    return term;
}


/* Assumes left-associative form.
   NOTE: it's probably worth trying to find a way to consolidate the type
   checking and garbage collection of arguments. */
static Term *
prim_reduce(
    Term *term,
    Manager *m,
    Type *term_type,
    Type *primitive_type,
    Type *combinator_type,
    Type *set_type )
{
    Primitive *p;
    void **args;
    term_cell *cur, *max;

    Object *result = 0;
    Set *result_set = 0;

    Object *reduce_arg( Term *t )
    {
        Object *arg;
        t = term__reduce( t, m, term_type, primitive_type, combinator_type, set_type );

        if ( term )
        {
            if ( term__length( t ) == 1 )
                arg = ( t->head + 1 )->pointerval;

            /* Fail with error message. */
            else
            {
                ERROR( "primitive applied to irreducible argument" );
                arg = 0;
            }

            term__free( t );
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

        ACTION add_to_result( void **refp )
        {
            set__add( result_set, *refp );
            return CONTINUE;
        }

        max = cur;

        if ( r )
        {
            if ( PRIM__ALLOW_GENERIC_RESULTS && p->return_type == any_type )
                /* Return value is an existing typed object. */
                robj = r;

            else
            {
                /* Return type is a raw data reference which needs to be bound to a type. */
                robj = manager__object( m, p->return_type, r, NOPROPS );
            }

            /* Multiple return values. */
            if ( result )
            {
                if ( !result_set )
                {
                    result_set = set__new();

/* FIXME: this is cheating */
                    if ( result->type == set_type )
                        set__walk( result->value, ( Visitor ) add_to_result );

                    else
                        set__add( result_set, result );

                    result = manager__object( m, set_type, result_set, NOPROPS );
                }

/* FIXME: this is cheating */
                if ( robj->type == set_type )
                    set__walk( robj->value, ( Visitor ) add_to_result );

                else
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
        term_cell *temp;

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

        ACTION recurse( Object **argp )
        {
            if ( curry( *argp ) )
                return BREAK;

            else
            {
                temp = cur;
                marshal( j + 1 );
                cur = temp;

                return CONTINUE;
            }
        }

        /* Iterate. */
        for ( j = i; j < p->arity; j++ )
        {
            if ( PRIM__CHECKS__PARAM_TYPE || PRIM__ALLOW_GENERIC_PARAMS )
                param_type = p->parameters[j].type;

            if ( cur->intval == 2 )
            {
                cur++;
                arg = cur->pointerval;
                cur++;
            }

            else
            {
/* FIXME: creating a new term for each argument is wasty */
                arg = reduce_arg( term__subterm_at( term, j + 1 ) );

                cur += cur->intval;
            }

            /* Abandon application along this argument path. */
            if ( !arg )
                return;

            /* Map the arguments. */
/* FIXME: as it is, term references will be iterated through, rather than reduced */
            if ( PRIM__IMPLICIT_SET_MAPPING && ( arg->type->flags & TYPE__IS_OBJ_COLL )
              && ( arg->type != param_type ) )
            {
                arg->type->walk( arg->value, ( Visitor ) recurse );
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
    p = ( ( Object* ) cur->pointerval )->value;

    if ( PRIM__ALLOW_NULLARY )
    {
        args = ( p->arity )
            ? malloc( p->arity * sizeof( term_cell ) )
            : 0;
    }

    else
    {
        if ( DEBUG__SAFE && !p->arity )
            ABORT;

        args = malloc( p->arity * sizeof( term_cell ) );
    }

    /* Advance to the first argument. */
    cur++;

    marshal( 0 );

    if ( !result )
        goto fail;

    /* NOTE: cur is now just beyond the position of the last argument. */
    cur = max;

    /* Replace the primitive reference and its arguments with the return value. */
    cur--;
    cur->pointerval = result;
    cur--;
    cur->intval = 2;

    if ( term->buffer + term->buffer_size - cur > 2 )
        term->head = cur - 1;

    else
        term->head = cur;

    term->head->intval = term->buffer + term->buffer_size - term->head;

    goto finish;

fail:

    term__free( term );
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

    if ( term->head->intval == 2 )
    {
        head_term = ( ( Object* ) ( term->head + 1 )->pointerval )->value;
        term__free( term );
        return term__copy( head_term );
    }

    else
    {
        head_term = ( ( Object* ) ( term->head + 2 )->pointerval )->value;

        size = head_term->head->intval;

        newsize = term->head->intval + size - 2;
        if ( newsize > term->buffer_size )
            term = term__expand( term, newsize );

        /*term->head = term->buffer;*/
        /*term->head = term->buffer + term->buffer_size - term->head->intval + 3 - size;*/
        term->head += 3 - size;

        memcpy( term->head, head_term->head, size * sizeof ( term_cell ) );

        if ( term->head->intval == 2 )
            term->head--;
        term->head->intval = term->buffer + term->buffer_size - term->head;

        return term;
    }
}


static void
print_term( Term *t )
{
    term_cell *cur = t->head, *lim = cur + cur->intval;

    while ( cur < lim )
    {
        if ( cur->intval < 0x1000 )
            PRINT( " %i", cur->intval );
        else
            PRINT( " %p", cur->pointerval );
        cur++;
    }

    PRINT( "\n" );
}


Term *
term__reduce(
    Term *term,
    Manager *m,
    Type *term_type,
    Type *primitive_type,
    Type *combinator_type,
    Type *set_type )
{
    Object *head;
    Type *head_type;

    unsigned int iter;

    if ( COMPILER__REDUX_TIMEOUT > 0 )
        iter = 0;

    if ( DEBUG__SAFE && ( !term || !m || !term_type || !primitive_type || !combinator_type || !set_type ) )
        ABORT;

#if DEBUG__SK
     PRINT( "%i:\t", iter );
     print_term( term );
#endif

    /* Iterate until the resulting term is in head-normal form. */
    for (;;)
    {
        if ( DEBUG__SAFE && !term )
            ABORT;

/*
cur = term->head; sup = term->buffer + term->buffer_size;
while ( cur < sup ) {
PRINT( " %x", ( int ) *cur ); cur++; }
PRINT( "\n" );  fflush( stdout );
*/
        /* Give up if the term becomes too large. */
        if ( COMPILER__MAX_TERM_SIZE > 0
          && ( unsigned int ) term->head->intval > COMPILER__MAX_TERM_SIZE )
        {
            ERROR( "term__reduce: abandoned (term might expand indefinitely)" );
            goto fail;
        }

        /* Singleton term. */
        if ( ( unsigned int ) term->head->intval == 2 )
            head = ( term->head + 1 )->pointerval;

        /* Left-associative sequence. */
        else
            head = ( term->head + 2 )->pointerval;

        /* There should be no way for nulls to appear at the head of a term. */
        if ( DEBUG__SAFE && !head )
            ABORT;

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

                default:

                    goto finish;
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
            if ( COMPILER__ALLOW_NONREDUX )
                goto finish;

            else
            {
                ERROR( "term__reduce: non-redex objects not permitted at the head of a term" );
                goto fail;
            }
        }

        /* Give up if reduction takes too long. */
        if ( COMPILER__REDUX_TIMEOUT > 0 )
        {
            iter++;

#if DEBUG__SK
            PRINT( "%i:\t", iter );
            print_term( term );
#endif

            if ( iter > COMPILER__REDUX_TIMEOUT )
            {
                ERROR( "term__reduce: abandoned (possible infinite loop)" );
                goto fail;
            }
        }
/*
        if ( !term )
            return 0;
*/
    }

fail:

    term__free( term );
    term = 0;

finish:

    return term;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
