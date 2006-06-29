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

#include <Combinator.h>
#include "../object/Object-impl.h"
#include "../primitive/Primitive-impl.h"
#include "graph.h"


#define BOXED_TYPE  any_type

/** Accessor macros for Apply objects. */
#define FUNCTION( o )           ( ( Apply* ) ( o )->value )->function
#define OPERAND( o )            ( ( Apply* ) ( o )->value )->operand

/*
#define FUNCTION( o )   ( o ) ? ( ( Apply* ) ( o )->value )->function : 0
#define OPERAND( o )    ( o ) ? ( ( Apply* ) ( o )->value )->operand : 0
*/
#define SET_FUNCTION( o, f )    ( ( Apply* ) ( o )->value )->function = ( f )
#define SET_OPERAND( o, op )    ( ( Apply* ) ( o )->value )->operand = ( op )


/******************************************************************************/


/** \note  The indirection type has a trivial destructor, as an indirection node
   does not own the object it points to. */
static void
indirection__delete( void *p )
{
    p = 0;
}


static void
indirection__encode( Object *o, char *buffer )
{
    object__encode( o, buffer );
}


/* FIXME: beware of trying to mutate this reference (it won't have any effect). */
static void
indirection__walk( void *p, Dist_f f )
{
    f( &p );
}


/******************************************************************************/


Type *apply_type = 0, *indirection_type = 0;

/* FIXME: use of managed objects as global variables is not thread-safe. */
Type *combinator_type, *primitive_type;


void
graph_init( Type *combinator_t, Type *primitive_t )
{
    combinator_type = combinator_t;
    primitive_type = primitive_t;

    if ( !apply_type || !indirection_type )
    {
        apply_type          = type__new( APPLY__NAME, TYPE__IS_OBJ_COLL );
        indirection_type    = type__new( INDIRECTION__NAME, TYPE__IS_OBJ_COLL );

        if ( !apply_type || !indirection_type )
            abort();

        apply_type->destroy = ( Destructor ) apply__delete;
        apply_type->encode = ( Encoder ) apply__encode;
        apply_type->walk = ( Walker ) apply__walk;

        indirection_type->destroy = ( Destructor ) indirection__delete;
        indirection_type->encode = ( Encoder ) indirection__encode;
        indirection_type->walk = ( Walker ) indirection__walk;
    }
}


void
graph_end()
{
    type__delete( apply_type );
    type__delete( indirection_type );
}


/******************************************************************************/


/* Shorts out indirection nodes when they are encountered. */
Object *
dereference( Object **opp )
{
    if ( DEBUG__SAFE && !opp )
        abort();

    /* Allow for indirection-to-null. */
    if ( FIRST_CLASS_NULL && !*opp )
        return 0;

    /* Assumes that chains of indirection nodes are possible. */
    while ( EAGER_REDIRECTION && ( *opp )->type == indirection_type )
    {
        *opp = ( *opp )->value;

        /* Allow for indirection-to-null. */
        if ( !*opp )
            break;
    }

    return *opp;
}


static Object *
skip_indirection( Object *o )
{
    /* Accept a NULL argument. */
    if ( !o )
        return 0;

    while ( o->type == indirection_type )
    {
        o = o->value;

        /* Take indirection nodes to NULL into account. */
        if ( !o )
            return 0;
    }

    return o;
}


/** Make the argument into an indirection node to the target object.
    \param o  the object to replace
    \param target  the object to point to (may be NULL) */
static void
substitute_boxed( Object *o, Object *target )
{
    if ( DEBUG__SAFE && !o )
        abort();

    o->type = indirection_type;
    o->value = target;
}


/* Make the argument into an object with the specified type and value. */
static void
substitute_unboxed( Object *o, Type *type, void *value )
{
    if ( DEBUG__SAFE && ( !o || !type || !value ) )
        abort();

    o->type = type;
    o->value = value;
}


/******************************************************************************/


/** Bxyz --> x(yz) */
static Object *
apply_B( Array *spine, unsigned int nargs, Memory_Manager *m )
{
    Object *a1, *a2, *a3;

    if ( nargs >= 3 )
    {
        a1 = array__pop( spine );
        a2 = array__pop( spine );
        a3 = array__pop( spine );

        /* Replace the function of the Apply with x. */
        SET_FUNCTION( a3, OPERAND( a1 ) );

        /* Replace the operand of the Apply with new object @yz. */
        SET_OPERAND( a3,
            memory_manager__object( m, apply_type,
                apply__new( OPERAND( a2 ), OPERAND( a3 ) ), NOFLAGS ) );

        return a3;
    }

    else
        return 0;
}


/** Cxyz --> xzy */
static Object *
apply_C( Array *spine, unsigned int nargs, Memory_Manager *m )
{
    Object *a1, *a2, *a3;

    if ( nargs >= 3 )
    {
        a1 = array__pop( spine );
        a2 = array__pop( spine );
        a3 = array__pop( spine );

        /* Replace the function of the Apply with new object @xz. */
        SET_FUNCTION( a3,
            memory_manager__object( m, apply_type,
                apply__new( OPERAND( a1 ), OPERAND( a3 ) ), NOFLAGS ) );

        /* Replace the operand of the Apply y. */
        SET_OPERAND( a3, OPERAND( a2 ) );

        return a3;
    }

    else
        return 0;
}


/** Ix --> x */
static Object *
apply_I( Array *spine, unsigned int nargs )
{
    Object *a1;

    if ( nargs >= 1 )
    {
        a1 = array__pop( spine );

        /* Replace the Apply with an indirection node to x. */
        substitute_boxed( a1, OPERAND( a1 ) );

        return a1;
    }

    else
        return 0;
}


/** Kxy --> x */
static Object *
apply_K( Array *spine, unsigned int nargs )
{
    Object *a1, *a2;

    if ( nargs >= 2 )
    {
        a1 = array__pop( spine );
        a2 = array__pop( spine );

        /* Replace the top-level Apply with an indirection node to x. */
        substitute_boxed( a2, OPERAND( a1 ) );

        return a2;
    }

    else
        return 0;
}


/** Lxy --> x(yy) */
static Object *
apply_L( Array *spine, unsigned int nargs, Memory_Manager *m )
{
    Object *a1, *a2;

    if ( nargs >= 2 )
    {
        a1 = array__pop( spine );
        a2 = array__pop( spine );

        /* Replace the function of the Apply with x. */
        SET_FUNCTION( a2, OPERAND( a1 ) );

        /* Replace the operand of the Apply with new object @yy. */
        SET_OPERAND( a2,
            memory_manager__object( m, apply_type,
                apply__new( OPERAND( a2 ), OPERAND( a2 ) ), NOFLAGS ) );

        return a2;
    }

    else
        return 0;
}


/** Oxy --> y(xy) */
static Object *
apply_O( Array *spine, unsigned int nargs, Memory_Manager *m )
{
    Object *a1, *a2;

    if ( nargs >= 2 )
    {
        a1 = array__pop( spine );
        a2 = array__pop( spine );

        /* Replace the function of the Apply with y. */
        SET_FUNCTION( a2, OPERAND( a2 ) );

        /* Replace the operand of the Apply with new object @xy. */
        SET_OPERAND( a2,
            memory_manager__object( m, apply_type,
                apply__new( OPERAND( a1 ), OPERAND( a2 ) ), NOFLAGS ) );

        return a2;
    }

    else
        return 0;
}


/** Rxyz --> yzx */
static Object *
apply_R( Array *spine, unsigned int nargs, Memory_Manager *m )
{
    Object *a1, *a2, *a3;

    if ( nargs >= 3 )
    {
        a1 = array__pop( spine );
        a2 = array__pop( spine );
        a3 = array__pop( spine );

        /* Replace the function of the Apply with new object @yz. */
        SET_FUNCTION( a3,
            memory_manager__object( m, apply_type,
                apply__new( OPERAND( a2 ), OPERAND( a3 ) ), NOFLAGS ) );

        /* Replace the operand of the Apply with x. */
        SET_OPERAND( a3, OPERAND( a1 ) );

        return a3;
    }

    else
        return 0;
}


/** Sxyz --> xz(yz) */
static Object *
apply_S( Array *spine, unsigned int nargs, Memory_Manager *m )
{
    Object *a1, *a2, *a3;

    if ( nargs >= 3 )
    {
        a1 = array__pop( spine );
        a2 = array__pop( spine );
        a3 = array__pop( spine );

        /* Replace the function of the Apply with new object @xz. */
        SET_FUNCTION( a3,
            memory_manager__object( m, apply_type,
                apply__new( OPERAND( a1 ), OPERAND( a3 ) ), NOFLAGS ) );

        /* Replace the operand of the Apply with new object @yz. */
        SET_OPERAND( a3,
            memory_manager__object( m, apply_type,
                apply__new( OPERAND( a2 ), OPERAND( a3 ) ), NOFLAGS ) );

        return a3;
    }

    else
        return 0;
}


/** Txy --> yx */
static Object *
apply_T( Array *spine, unsigned int nargs )
{
    Object *a1, *a2;

    if ( nargs >= 2 )
    {
        a1 = array__pop( spine );
        a2 = array__pop( spine );

        /* Replace the function of the Apply with y. */
        SET_FUNCTION( a2, OPERAND( a2 ) );

        /* Replace the operand of the Apply with x. */
        SET_OPERAND( a2, OPERAND( a1 ) );

        return a2;
    }

    else
        return 0;
}


/** Uxy --> y(xxy) */
static Object *
apply_U( Array *spine, unsigned int nargs, Memory_Manager *m )
{
    Object *a1, *a2;

    if ( nargs >= 2 )
    {
        a1 = array__pop( spine );
        a2 = array__pop( spine );

        /* Replace the function of the Apply with y. */
        SET_FUNCTION( a2, OPERAND( a2 ) );

        /* Replace the operand of the Apply with new object @(@xx)y. */
        SET_OPERAND( a2,
            memory_manager__object( m, apply_type,
                apply__new(
                    memory_manager__object( m, apply_type,
                        apply__new( OPERAND( a1 ), OPERAND( a1 ) ), NOFLAGS ),
                    OPERAND( a2 ) ), NOFLAGS ) );

        return a2;
    }

    else
        return 0;
}


/** Vxyz --> zxy */
static Object *
apply_V( Array *spine, unsigned int nargs, Memory_Manager *m )
{
    Object *a1, *a2, *a3;

    if ( nargs >= 3 )
    {
        a1 = array__pop( spine );
        a2 = array__pop( spine );
        a3 = array__pop( spine );

        /* Replace the function of the Apply with new object @zx. */
        SET_FUNCTION( a3,
            memory_manager__object( m, apply_type,
                apply__new( OPERAND( a3 ), OPERAND( a1 ) ), NOFLAGS ) );

        /* Replace the operand of the Apply with y. */
        SET_OPERAND( a3, OPERAND( a2 ) );

        return a3;
    }

    else
        return 0;
}


/** Wxy --> xyy */
static Object *
apply_W( Array *spine, unsigned int nargs, Memory_Manager *m )
{
    Object *a1, *a2;

    if ( nargs >= 2 )
    {
        a1 = array__pop( spine );
        a2 = array__pop( spine );

        /* Replace the function of the Apply with new object @xy. */
        SET_FUNCTION( a2,
            memory_manager__object( m, apply_type,
                apply__new( OPERAND( a1 ), OPERAND( a2 ) ), NOFLAGS ) );

        /* Replace the operand of the Apply with new object y. */
        SET_OPERAND( a2, OPERAND( a2 ) );

        array__push( spine, a2 );

        return a2;
    }

    else
        return 0;
}


/** wx --> xx */
static Object *
apply_w( Array *spine, unsigned int nargs )
{
    Object *a1;

    if ( nargs >= 1 )
    {
        a1 = array__pop( spine );

        /* Replace the function of the Apply with x. */
        SET_FUNCTION( a1, OPERAND( a1 ) );

        return a1;
    }

    else
        return 0;
}


/** Yf --> f(Yf) */
static Object *
apply_Y( Array *spine, unsigned int nargs, Memory_Manager *m )
{
    Object *a1, *f;

    if ( nargs >= 1 )
    {
        a1 = array__pop( spine );

        f = OPERAND( a1 );

        /* Replace the operand of the Apply with new object @Yf. */
        SET_OPERAND( a1,
             memory_manager__object( m, apply_type,
                 apply__new( FUNCTION( a1 ), f ), NOFLAGS ) );

        /* Replace the function of the Apply with f. */
        SET_FUNCTION( a1, f );

        return a1;
    }

    else
        return 0;
}


/** \note  There should be no possibility of program-level errors in the
    application of the combinator functions, which simply rearrange nodes. */
static Object *
apply_combinator( Combinator *c, Array *spine, unsigned int nargs, Memory_Manager *m )
{
    if ( DEBUG__SAFE && ( !c || !spine || !m ) )
        abort();

    /* TODO: how about a lookup table instead of a switch statement? */
    switch ( *c )
    {
        case B_combinator:

            return apply_B( spine, nargs, m );

        case C_combinator:

            return apply_C( spine, nargs, m );

        case I_combinator:

            return apply_I( spine, nargs );

        case K_combinator:

            return apply_K( spine, nargs );

        case L_combinator:

            return apply_L( spine, nargs, m );

        case O_combinator:

            return apply_O( spine, nargs, m );

        case R_combinator:

            return apply_R( spine, nargs, m );

        case S_combinator:

            return apply_S( spine, nargs, m );

        case T_combinator:

            return apply_T( spine, nargs );

        case U_combinator:

            return apply_U( spine, nargs, m );

        case V_combinator:

            return apply_V( spine, nargs, m );

        case W_combinator:

            return apply_W( spine, nargs, m );

        case w_combinator:

            return apply_w( spine, nargs );

        case Y_combinator:

            return apply_Y( spine, nargs, m );

        default:

            abort();
    }
}


/******************************************************************************/


static Object *
apply_primitive( Primitive *p, Array *spine, unsigned int nargs, Memory_Manager *m )
{
    unsigned int i;
    Object *ap, *arg;
    void *args[PRIM__MAX_PARAMS];
    void *result;
    Type *t;

    if ( DEBUG__SAFE && ( !p || !spine || !m || p->arity > PRIM__MAX_PARAMS ) )
        abort();

    /* Apply the primitive, provided that there are enough arguments, and that
       they reduce to objects of the expected types. */
    if ( nargs >= p->arity )
    {
        /* For each argument... */
        for ( i = 0; i < p->arity; i++ )
        {
            ap = array__pop( spine );
            arg = reduce__graph_lazy( OPERAND( ap ), spine, m );

            /* Argument reduced to a non-null object. */
            if ( arg )
            {
                t = p->parameters[i].type;

                /* Check for type agreement. */
                if ( PRIM__CHECKS__PARAM_TYPE )
                {
                    if ( t != arg->type
                    && ( !PRIM__ALLOW_GENERIC_PARAMS || t != BOXED_TYPE ) )
                    {
                        ERROR( "prim_reduce: argument type mismatch" );

                        goto bad_arg;
                    }
                }

                /* Argument type is boxed. */
                if ( PRIM__ALLOW_GENERIC_PARAMS && t == BOXED_TYPE )
                    args[i] = arg;

                /* Argument type is unboxed. */
                else
                    args[i] = arg->value;
            }

            /* Argument reduced to null. */
            else
                goto bad_arg;
        }

        /* Caution: there is no exception handling at this level. */
        result = p->cstub( args );

        /* Return value is an existing typed object. */
        if ( PRIM__ALLOW_GENERIC_RESULTS && p->return_type == BOXED_TYPE )
            substitute_boxed( ap, result );

        /* Return type is a raw data reference which needs to be bound to a type ("boxed"). */
        else
            substitute_unboxed( ap, p->return_type, result );

        return ap;
    }

    else
        return 0;

bad_arg:

    /* FIXME: not used */
    if ( SK__ALLOW_NONREDUX )
        ap->flags |= OBJECT__IRREDUCIBLE;

    /* f 0 --> 0 */
    substitute_boxed( ap, 0 );

    return ap;
}


/******************************************************************************/

/*
[ba] first-class algorithms ^ reduction consists a side-effect to the expression reduced.  Reducing an expression which is meant to be used as data may destroy its usefulness
*/

/** \param spine  a stack of Apply objects (and ONLY Apply objects)
    \return  An immediate object (i.e. not an indirection node) if successful,
    or 0 if unsuccessful. */
Object *
reduce__graph_lazy( Object *o, Array *spine, Memory_Manager *m )
{
    Type *t;

    /* Start at the root object. */
    Object *cur = o;

    /* Original size of the stack. */
    unsigned int len = array__size( spine );

#if SK__CHECKS__MAX_REDUX_ITERATIONS > 0
    int iter = 0;
#endif

    /* Remove all objects added to the stack since invocation. */
    void unwind( boolean failure )
    {
        unsigned int i;

        for ( i = array__size( spine ); i > len; i-- )
        {
            cur = array__pop( spine );

            if ( failure && EAGER_PRUNING )
                substitute_boxed( cur, 0 );
        }
    }

    /* Iterate until either the expression is in head-normal form or an error is
       encountered. */
    for (;;)
    {
        /* 0 x --> 0 */
        if ( !cur )
        {
            unwind( TRUE );
            break;
        }

        else
        {
            /* FIXME: possible race condition.  If ap is initially an Apply but
               is overwritten (in another thread) with an indirection node or a
               boxed value, a segfault is likely. */
            t = cur->type;
/*PRINT( "type->name = %s\n", cur->type->name );*/

            if ( t == indirection_type )
                cur = cur->value;

            /* Traverse down the spine so long as only apply and indirection
               nodes are encountered. */
            else if ( t == apply_type )
            {
                array__push( spine, cur );

                if ( EAGER_REDIRECTION )
                    cur = DEREF( &FUNCTION( cur ) );
                else
                    cur = FUNCTION( cur );
            }

            /* Object at the tip of the spine is a combinator. */
            else if ( t == combinator_type )
            {
                cur = apply_combinator( cur->value, spine, array__size( spine ) - len, m );

                if ( !cur )
                {
                    unwind( FALSE );
                    break;
                }
            }

            /* Object at the tip of the spine is a primitive function. */
            else if ( t == primitive_type )
            {
                cur = apply_primitive( cur->value, spine, array__size( spine ) - len, m );

                if ( !cur )
                {
                    unwind( FALSE );
                    break;
                }
            }

            /* Object at tip of spine is a literal value. */
            else
            {
                if ( ( array__size( spine ) - len ) && !SK__ALLOW_NONREDUX )
                {
                    ERROR( "can't apply a non-redex object to an argument" );

                    /* error --> 0 */
                    unwind( !SK__ALLOW_NONREDUX );
                }

                break;
            }
        }

        /* Give up if reduction takes too long. */
        if ( SK__CHECKS__MAX_REDUX_ITERATIONS > 0 )
        {
            iter++;

            if ( iter > SK__CHECKS__MAX_REDUX_ITERATIONS )
            {
                ERROR( "reduction abandoned (possible non-termination)" );

                /* Caution: the danger of NOT pruning this expression is that
                   the program will try to reduce it again and again, leading to
                   bad performance and/or memory problems. */
                unwind( FALSE );
                break;
            }
        }
    }

    if ( EAGER_REDIRECTION )
        return skip_indirection( o );
    else
        return o;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
