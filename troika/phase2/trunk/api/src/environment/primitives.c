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

#include "Environment-impl.h"
    #include "../object/Object-impl.h"

#if TRIPLES__GLOBAL

#include <compiler.h>
#include "../compiler/Apply.h"

#include <Primitive-import.h>
    #include "../primitive/Primitive-impl.h"


/* FIXME */
static Environment *global_env;


/******************************************************************************/


static void *
assoc_stub( void **args )
{
    return object__associate( args[0], args[1], args[2] );
}

static void *
dissoc_stub( void **args )
{
    return object__dissociate( args[0], args[1] );
}

static void *
mult_stub( void **args )
{
    return object__multiply( args[0], args[1] );
}

static void *
equals_stub( void **args )
{
    if ( args[0] == args[1] )
        return args[0];
    else
        return 0;
}

static void *
children_stub( void **args )
{
    Object *o = args[0];
    Set *s = set__new();
    Type *t = object__type( o );

    void *helper( void **opp )
    {
        set__add( s, *opp );
        return 0;
    }

    if ( t->flags & TYPE__IS_OBJ_COLL )
        t->walk( object__value( o ), ( Visitor ) helper );

    return s;
}


/*
static void *
objects_inv_stub( void **args )
{
    Set *s = args[0];
    Set *matches = set__new();

    ACTION helper( Object **opp )
    {
        Object *o = *opp;

        void *helper2( Hash_Map__Entry **epp )
        {
            Object *target = ( *epp )->target;

            if ( set__contains( s, target ) )
            {
                set__add( matches, o );
                return BREAK;
            }

            else
                return CONTINUE;
        }

        if ( TRIPLES__GLOBAL__OUT_EDGES && ( o->outbound_edges ) )
            hash_map__walk( o->outbound_edges, ( Visitor ) helper );

        return 0;
    }

    memory_manager__walk( m, 0, ( Visitor ) helper, FALSE, TRUE );

    return matches;
}
*/


static void *
predicates_stub( void **args )
{
    Object *o = args[0];
    Set *s = set__new();

    ACTION helper( Object **pred, Object **obj )
    {
        set__add( s, DEREF( pred ) );
        obj = 0;
        return CONTINUE;
    }

    if ( TRIPLES__GLOBAL__OUT_EDGES && ( o->outbound_edges ) )
        hash_map__walk2( o->outbound_edges, ( Visitor2 ) helper );

    return s;
}


static void *
objects_stub( void **args )
{
    Object *o = args[0];
    Set *s = set__new();

    ACTION helper( Object **pred, Object **obj )
    {
        set__add( s, DEREF( obj ) );
        pred = 0;
        return CONTINUE;
    }

    if ( TRIPLES__GLOBAL__OUT_EDGES && ( o->outbound_edges ) )
        hash_map__walk2( o->outbound_edges, ( Visitor2 ) helper );

    return s;
}


static int
add_triples_prims( Environment *env )
{
    Primitive *p;

    return ( ( p = primitive__new( env, NAMEOF( ANY ), "^=", assoc_stub, 3 ) )
      && primitive__add_param( env, p, NAMEOF( ANY ), "subject", REF_OPQ )
      && primitive__add_param( env, p, NAMEOF( ANY ), "predicate", REF_TRP )
      && primitive__add_param( env, p, NAMEOF( ANY ), "object", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, NAMEOF( ANY ), "^0", dissoc_stub, 2 ) )
      && primitive__add_param( env, p, NAMEOF( ANY ), "subject", REF_TRP )
      && primitive__add_param( env, p, NAMEOF( ANY ), "predicate", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, NAMEOF( ANY ), "^", mult_stub, 2 ) )
      && primitive__add_param( env, p, NAMEOF( ANY ), "subject", REF_TRP )
      && primitive__add_param( env, p, NAMEOF( ANY ), "predicate", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, NAMEOF( ANY ), "object_object__equals", equals_stub, 2 ) )
      && primitive__add_param( env, p, NAMEOF( ANY ), "objectA", REF_TRP )
      && primitive__add_param( env, p, NAMEOF( ANY ), "objectB", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, "Set", "object__triples_predicates", predicates_stub, 1 ) )
      && primitive__add_param( env, p, NAMEOF( ANY ), "objectA", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, "Set", "object__triples_objects", objects_stub, 1 ) )
      && primitive__add_param( env, p, NAMEOF( ANY ), "objectA", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, "Set", "object__children", children_stub, 1 ) )
      && primitive__add_param( env, p, NAMEOF( ANY ), "objectA", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 ) );
}

#endif  /* TRIPLES_GLOBAL */


/******************************************************************************/


#include <collection/Set.h>


static void *
set_add_stub( void **args )
{
    Set *s = args[0];
    Object *o2 = args[1];

    /* ! No checking of return value. */
    set__add( s, o2 );

    return 0;
}


static void *
set_remove_stub( void **args )
{
    Set *s = args[0];
    Object *o2 = args[1];

    /* ! No checking of return value. */
    set__remove( s, o2 );

    return 0;
}


static void *
set_contains_stub( void **args )
{
    int *i = NEW( int );
    *i = set__contains( args[0], args[1] );
    return i;
}


static void *
set_size_stub( void **args )
{
    int *i = NEW( int );
    *i = ( int ) set__size( args[0] );
    return i;
}


static void *
set_singleton_stub( void **args )
{
    Object *o1 = args[0];

    Set *s = set__new();
    set__add( s, o1 );

    return s;
}


static int
add_set_prims( Environment *env )
{
    Primitive *p;

    return ( ( p = primitive__new( env, NAMEOF( ANY ), "set__add", set_add_stub, 2 ) )
      && primitive__add_param( env, p, "Set", "s", REF_OPQ )
      && primitive__add_param( env, p, NAMEOF( ANY ), "el", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, NAMEOF( ANY ), "set__remove", set_remove_stub, 2 ) )
      && primitive__add_param( env, p, "Set", "s", REF_OPQ )
      && primitive__add_param( env, p, NAMEOF( ANY ), "el", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, "int", "set__size", set_size_stub, 1 ) )
      && primitive__add_param( env, p, "Set", "s", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, "int", "set__contains", set_contains_stub, 2 ) )
      && primitive__add_param( env, p, "Set", "s", REF_TRP )
      && primitive__add_param( env, p, NAMEOF( ANY ), "el", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, "Set", "set__singleton", set_singleton_stub, 1 ) )
      && primitive__add_param( env, p, NAMEOF( ANY ), "el", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 ) );
}


/******************************************************************************/


static void *
int_nonzero_stub( void **args )
{
    int *i = args[0];
    Object *o;

    if ( *i )
    {
        o = environment__resolve_combinator( global_env, "K" );
    }

    else
    {
        o = memory_manager__object( global_env->manager, apply_type, apply__new(
            environment__resolve_combinator( global_env, "S" ),
            environment__resolve_combinator( global_env, "K" ) ), NOFLAGS );
    }

    return o;
}


static int
add_sk_prims( Environment *env )
{
    Primitive *p;

    return ( ( p = primitive__new( env, NAMEOF( ANY ), "int__nonzero", int_nonzero_stub, 1 ) )
      && primitive__add_param( env, p, "int", "i", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 ) );
}


/******************************************************************************/


/* Note: this is as pokey as it looks. */
static void *
type_of( void **args )
{
    Object *o = args[0];
    return namespace__lookup_simple( global_env->types->value, o->type->name );
}


/* A trivial function to force the reduction of its argument. */
static void *
object__identity( void **args )
{
    return args[0];
}


int
add_meta_prims( Environment *env )
{
    Primitive *p;

    /* ! */
    global_env = env;

    return (
         ( p = primitive__new( env, NAMEOF( ANY ), "type_of", type_of, 1 ) )
      && primitive__add_param( env, p, NAMEOF( ANY ), "self", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, NAMEOF( ANY ), "object__identity", object__identity, 1 ) )
      && primitive__add_param( env, p, NAMEOF( ANY ), "o", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

#if TRIPLES__GLOBAL
      && add_triples_prims( env )
#endif
      && add_set_prims( env )
      && add_sk_prims( env )
    );
}


/******************************************************************************/


Object *
environment__register_primitive
    ( Environment *env, Primitive *prim, int flags, Generic_f src_f )
{
    Object *o;

    Type *t = environment__resolve_type( env, NAMEOF( PRIMITIVE ) )->value;

    Type *first_param = primitive__parameter_type( prim, 0 );

    if ( flags & PRIM__CONSTRUCTOR )
        ERROR( "environment__register_primitive: PRIM__CONSTRUCTOR is not in use" );
    if ( flags & PRIM__DECODER )
        prim->return_type->decode = ( Decoder ) src_f;
    if ( flags & PRIM__DESTRUCTOR )
        first_param->destroy = ( Destructor ) src_f;
    if ( flags & PRIM__ENCODER )
        first_param->encode = ( Encoder ) src_f;

    if ( !( o = memory_manager__object( env->manager, t, prim, OBJECT__IMMUTABLE ) ) )
    {
        primitive__delete( prim );
        return 0;
    }

    if ( namespace__add_simple( env->primitives->value, prim->name, o ) )
        return 0;

    else
        return o;
}


Primitive_o *
environment__resolve_primitive( Environment *env, const char *name )
{
    return namespace__lookup_simple( env->primitives->value, name );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
