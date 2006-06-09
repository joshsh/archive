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

#include <sk/Combinator.h>
#include "../Object-impl.h"
#include "../Primitive-impl.h"
#include "graph.h"


#define BOXED_TYPE  any_type

/* Accessor macros for Apply objects. */
#define FUNCTION( o )   ( ( Apply* ) ( o )->value )->function
#define OPERAND( o )    ( ( Apply* ) ( o )->value )->operand
#define SET_FUNCTION( o, f )    ( ( Apply* ) ( o )->value )->function = ( f )
#define SET_OPERAND( o, op )    ( ( Apply* ) ( o )->value )->operand = ( op )


/******************************************************************************/


/* Note: the indirection type has a trivial destructor, as it does not own
   the object it points to. */
static void
indirection__delete( void *p )
{
    p = 0;
}


static void
indirection__encode( Object *o, char *buffer )
{
    object__type( o )->encode( object__value( o ), buffer );
}


/* FIXME: beware of trying to mutate this reference (it won't have any effect). */
static void
indirection__walk( void *p, Dist_f f )
{
    f( &p );
}


/******************************************************************************/


Type *apply_type, *indirection_type;

/* FIXME: not thread-safe. */
Type *combinator_type, *primitive_type;


void
graph_init( Type *combinator_t, Type *primitive_t )
{
    combinator_type = combinator_t;
    primitive_type = primitive_t;

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


void
graph_end()
{
    type__delete( apply_type );
    type__delete( indirection_type );
}


/******************************************************************************/


/* Shorts out indirection nodes when they are encountered. */
static Object *
dereference( Object **opp )
{
    if ( DEBUG__SAFE && !opp )
        abort();

    /* Assumes that chains of indirection nodes are possible. */
    while ( ( *opp )->type == indirection_type )
    {
        *opp = ( *opp )->value;
        if ( ! *opp )
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


/* Make the argument into an indirection node to the target object. */
static void
substitute_boxed( Object *o, Object *target )
{
    o->type = indirection_type;
    o->value = target;
}


/* Make the argument into an object with the specified type and value. */
static void
substitute_unboxed( Object *o, Type *type, void *value )
{
    o->type = type;
    o->value = value;
}


/******************************************************************************/


/* Bxyz --> x(yz) */
static boolean
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
                apply__new( OPERAND( a2 ), OPERAND( a3 ) ) ) );

        return TRUE;
    }

    else
        return FALSE;
}


/* Cxyz --> xzy */
static boolean
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
                apply__new( OPERAND( a1 ), OPERAND( a3 ) ) ) );

        /* Replace the operand of the Apply y. */
        SET_OPERAND( a3, OPERAND( a2 ) );

        return TRUE;
    }

    else
        return FALSE;
}


/* Ix --> x */
static boolean
apply_I( Array *spine, unsigned int nargs )
{
    Object *a1;

    if ( nargs >= 1 )
    {
        a1 = array__pop( spine );

        /* Replace the Apply with an indirection node to x. */
        substitute_boxed( a1, OPERAND( a1 ) );

        return TRUE;
    }

    else
        return FALSE;
}


/* Kxy --> x */
static boolean
apply_K( Array *spine, unsigned int nargs )
{
    Object *a1, *a2;

    if ( nargs >= 2 )
    {
        a1 = array__pop( spine );
        a2 = array__pop( spine );

        /* Replace the top-level Apply with an indirection node to x. */
        substitute_boxed( a2, OPERAND( a1 ) );

        return TRUE;
    }

    else
        return FALSE;
}


/* Lxy --> x(yy) */
static boolean
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
                apply__new( OPERAND( a2 ), OPERAND( a2 ) ) ) );

        return TRUE;
    }

    else
        return FALSE;
}


/* Oxy --> y(xy) */
static boolean
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
                apply__new( OPERAND( a1 ), OPERAND( a2 ) ) ) );

        return TRUE;
    }

    else
        return FALSE;
}


/* Rxyz --> yzx */
static boolean
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
                apply__new( OPERAND( a2 ), OPERAND( a3 ) ) ) );

        /* Replace the operand of the Apply with x. */
        SET_OPERAND( a3, OPERAND( a1 ) );

        return TRUE;
    }

    else
        return FALSE;
}


/* Sxyz --> xz(yz) */
static boolean
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
                apply__new( OPERAND( a1 ), OPERAND( a3 ) ) ) );

        /* Replace the operand of the Apply with new object @yz. */
        SET_OPERAND( a3,
            memory_manager__object( m, apply_type,
                apply__new( OPERAND( a2 ), OPERAND( a3 ) ) ) );

        return TRUE;
    }

    else
        return FALSE;
}


/* Txy --> yx */
static boolean
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

        return TRUE;
    }

    else
        return FALSE;
}


/* Uxy --> y(xxy) */
static boolean
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
                        apply__new( OPERAND( a1 ), OPERAND( a1 ) ) ),
                    OPERAND( a2 ) ) ) );

        return TRUE;
    }

    else
        return FALSE;
}


/* Vxyz --> zxy */
static boolean
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
                apply__new( OPERAND( a3 ), OPERAND( a1 ) ) ) );

        /* Replace the operand of the Apply with y. */
        SET_OPERAND( a3, OPERAND( a2 ) );

        return TRUE;
    }

    else
        return FALSE;
}


/* Wxy --> xyy */
static boolean
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
                apply__new( OPERAND( a1 ), OPERAND( a2 ) ) ) );

        /* Replace the operand of the Apply with new object y. */
        SET_OPERAND( a2, OPERAND( a2 ) );

        return TRUE;
    }

    else
        return FALSE;
}


/* wx --> xx */
static boolean
apply_w( Array *spine, unsigned int nargs )
{
    Object *a1;

    if ( nargs >= 1 )
    {
        a1 = array__pop( spine );

        /* Replace the function of the Apply with x. */
        SET_FUNCTION( a1, OPERAND( a1 ) );

        return TRUE;
    }

    else
        return FALSE;
}


/* Yf --> f(Yf) */
static boolean
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
                 apply__new( FUNCTION( a1 ), f ) ) );

        /* Replace the function of the Apply with f. */
        SET_FUNCTION( a1, f );

        return TRUE;
    }

    else
        return FALSE;
}


/** \note  There should be no possibility of program-level errors in the
    application of the combinator functions, which simply rearrange nodes. */
static boolean
apply_combinator( Object *o, Array *spine, unsigned int nargs, Memory_Manager *m )
{
    if ( DEBUG__SAFE && ( !o || !spine || !m ) )
        abort();

    switch ( *( ( Combinator* ) o->value ) )
    {
        case B_combinator:

            return apply_B( spine, nargs, m );
            break;

        case C_combinator:

            return apply_C( spine, nargs, m );
            break;

        case I_combinator:

            return apply_I( spine, nargs );
            break;

        case K_combinator:

            return apply_K( spine, nargs );
            break;

        case L_combinator:

            return apply_L( spine, nargs, m );
            break;

        case O_combinator:

            return apply_O( spine, nargs, m );
            break;

        case R_combinator:

            return apply_R( spine, nargs, m );
            break;

        case S_combinator:

            return apply_S( spine, nargs, m );
            break;

        case T_combinator:

            return apply_T( spine, nargs );
            break;

        case U_combinator:

            return apply_U( spine, nargs, m );
            break;

        case V_combinator:

            return apply_V( spine, nargs, m );
            break;

        case W_combinator:

            return apply_W( spine, nargs, m );
            break;

        case w_combinator:

            return apply_w( spine, nargs );
            break;

        case Y_combinator:

            return apply_Y( spine, nargs, m );
            break;

        default:

            abort();
    }
}


/******************************************************************************/


static boolean
apply_primitive( Primitive *p, Array *spine, unsigned int nargs, Memory_Manager *m )
{
    unsigned int i;
    Object *ap, *arg;
    void *args[PRIM__MAX_PARAMS], *result;
    Type *t;

    if ( DEBUG__SAFE && ( !p || !spine || !m || p->arity > PRIM__MAX_PARAMS ) )
        abort();

    /* Apply the primitive, provided that there are enough arguments, and that
       they reduce to objects of the expected types. */
    if ( nargs >= p->arity )
    {
        for ( i = 0; i < p->arity; i++ )
        {
            ap = array__pop( spine );
            arg = reduce__graph_lazy( OPERAND( ap ), spine, m );

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
                        ap->flags |= OBJECT__IRREDUCIBLE;
                        return FALSE;
                    }
                }

                /* Argument type is boxed. */
                if ( PRIM__ALLOW_GENERIC_PARAMS && t == BOXED_TYPE )
                    args[i] = arg;

                /* Argument type is unboxed. */
                else
                    args[i] = arg->value;
            }

            else
            {
                ERROR( "primitive applied to irreducible argument" );
                ap->flags |= OBJECT__IRREDUCIBLE;
                return FALSE;
            }
        }

        /* Caution: there is no exception handling at this level. */
        result = p->cstub( args );

        /* Return value is an existing typed object. */
        if ( PRIM__ALLOW_GENERIC_RESULTS && p->return_type == BOXED_TYPE )
            substitute_boxed( ap, result );

        /* Return type is a raw data reference which needs to be bound to a type ("boxed"). */
        else
            substitute_unboxed( ap, p->return_type, result );

        return TRUE;
    }

    else
        return FALSE;
}


/******************************************************************************/


/** \return  An immediate object (i.e. not an indirection node) if successful,
    or 0 if unsuccessful. */
Object *
reduce__graph_lazy( Object *o, Array *spine, Memory_Manager *m )
{
    Object *ap;

    /* Note: because spine is passed recursively, array__size( spine ) is not
       necessarily the number of available arguments. */
    unsigned int nargs;

    /* Original size of the stack. */
    unsigned int len = array__size( spine );

    void rewind0()
    {
        unsigned int i;
        for ( i = array__size( spine ); i > len; i-- )
            array__pop( spine );
    }

    if ( DEBUG__SAFE && !o )
        abort();
if ( DEBUG__SK ) PRINT( "---gr 0---\n" );
    /* Break out when no more reduction is possible. */
    for (;;)
    {
if ( DEBUG__SK ) PRINT( "---gr 1---\n" );
        ap = o;
        nargs = 0;

        ap = skip_indirection( ap );

        if ( !ap )
            break;

        /* Traverse to the tip of the application spine. */
        while ( ap->type == apply_type )
        {
            array__push( spine, ap );
            nargs++;

            /* FIXME: possible race condition.  If ap is initially an Apply but
               is overwritten (by another thread) with an indirection node or a
               boxed value, a segfault is inevitable. */
            ap = dereference( &FUNCTION( ap ) );

/*
            if ( !ap )
                break;
*/
        }
if ( DEBUG__SK ) PRINT( "---gr 2---\n" );

        if ( ap->type == combinator_type )
        {
if ( DEBUG__SK ) PRINT( "---gr 3a---\n" );
            if ( !apply_combinator( ap, spine, nargs, m ) )
                break;
        }

        else if ( ap->type == primitive_type )
        {
if ( DEBUG__SK ) PRINT( "---gr 3b---\n" );
            if ( !apply_primitive( ap->value, spine, nargs, m ) )
                break;
        }

        /* Object at tip of spine is a literal value. */
        else
        {
if ( DEBUG__SK ) PRINT( "---gr 3c---\n" );
/*PRINT( "type->name = %s\n", ap->type->name );*/
            if ( nargs && !SK__ALLOW_NONREDUX )
                ERROR( "can't apply a non-redex object to an argument" );

            break;
        }
if ( DEBUG__SK ) PRINT( "---gr 4---\n" );

        rewind0();
    }
if ( DEBUG__SK ) PRINT( "---gr 5---\n" );

    rewind0();

    return skip_indirection( o );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
