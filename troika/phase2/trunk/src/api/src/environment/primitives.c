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

#include "Environment-impl.h"
    #include "../Object-impl.h"

#if TRIPLES__GLOBAL

#include <Primitive-import.h>
    #include "../Primitive-impl.h"


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
        t->walk( object__value( o ), ( Dist_f ) helper );

    return s;
}


static void *
predicates_stub( void **args )
{
    Object *o = args[0];
    Set *s = set__new();

    void *helper( Hash_Map__Entry **epp )
    {
        set__add( s, ( *epp )->key );
        return 0;
    }

    #if TRIPLES__GLOBAL__OUT_EDGES
    if ( o->outbound_edges )
        hash_map__walk( o->outbound_edges, ( Dist_f ) helper );
    #endif

    return s;
}


static void *
objects_stub( void **args )
{
    Object *o = args[0];
    Set *s = set__new();

    void *helper( Hash_Map__Entry **epp )
    {
        set__add( s, ( *epp )->target );
        return 0;
    }

    #if TRIPLES__GLOBAL__OUT_EDGES
    if ( o->outbound_edges )
        hash_map__walk( o->outbound_edges, ( Dist_f ) helper );
    #endif

    return s;
}


static int
add_triples_prims( Environment *env )
{
    Primitive *p;

    return ( ( p = primitive__new( env, ANY__NAME, "^=", assoc_stub, 3 ) )
      && primitive__add_param( env, p, ANY__NAME, "subject", REF_OPQ )
      && primitive__add_param( env, p, ANY__NAME, "predicate", REF_TRP )
      && primitive__add_param( env, p, ANY__NAME, "object", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, ANY__NAME, "^0", dissoc_stub, 2 ) )
      && primitive__add_param( env, p, ANY__NAME, "subject", REF_TRP )
      && primitive__add_param( env, p, ANY__NAME, "predicate", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, ANY__NAME, "^", mult_stub, 2 ) )
      && primitive__add_param( env, p, ANY__NAME, "subject", REF_TRP )
      && primitive__add_param( env, p, ANY__NAME, "predicate", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, ANY__NAME, "object_object__equals", equals_stub, 2 ) )
      && primitive__add_param( env, p, ANY__NAME, "objectA", REF_TRP )
      && primitive__add_param( env, p, ANY__NAME, "objectB", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, "Set", "object__triples_predicates", predicates_stub, 1 ) )
      && primitive__add_param( env, p, ANY__NAME, "objectA", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, "Set", "object__triples_objects", objects_stub, 1 ) )
      && primitive__add_param( env, p, ANY__NAME, "objectA", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, "Set", "object__children", children_stub, 1 ) )
      && primitive__add_param( env, p, ANY__NAME, "objectA", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 ) );
}

#endif


#include <util/Set.h>

static void *
set_add_stub( void **args )
{
    Object *o1 = args[0];
    Object *o2 = args[1];

    Set *s = object__value( o1 );

    /* ! No checking of return value. */
    set__add( s, o2 );

    return o1;
}


static void *
set_remove_stub( void **args )
{
    Object *o1 = args[0];
    Object *o2 = args[1];

    Set *s = ( Set* ) object__value( o1 );

    /* ! No checking of return value. */
    set__remove( s, o2 );

    return o1;
}


static int
add_set_prims( Environment *env )
{
    Primitive *p;

    return ( ( p = primitive__new( env, ANY__NAME, "set__add", set_add_stub, 2 ) )
      && primitive__add_param( env, p, "Set", "s", REF_OPQ )
      && primitive__add_param( env, p, ANY__NAME, "el", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )

      && ( p = primitive__new( env, ANY__NAME, "set__remove", set_remove_stub, 2 ) )
      && primitive__add_param( env, p, "Set", "s", REF_OPQ )
      && primitive__add_param( env, p, ANY__NAME, "el", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 ) );
}


/* ! */
static Environment *global_env;

/* Note: this is as pokey as it looks. */
static void *
type_of( void **args )
{
    Object *o = args[0];
    return namespace__lookup_simple( global_env->types->value, o->type->name );
}


int
add_meta_prims( Environment *env )
{
    Primitive *p;

    /* ! */
    global_env = env;

    return ( ( p = primitive__new( env, ANY__NAME, "type_of", type_of, 1 ) )
      && primitive__add_param( env, p, ANY__NAME, "self", REF_TRP )
      && primitive__register( env, p, NOPROPS, 0 )
    #if TRIPLES__GLOBAL
      && add_triples_prims( env )
    #endif
      && add_set_prims( env )
    );
}


/******************************************************************************/


Object *
environment__register_primitive
    ( Environment *env, Primitive *prim, int flags, Generic_f src_f )
{
    Object *o;

    Type *t = environment__resolve_type( env, PRIMITIVE__NAME )->value;

    Type *first_param = primitive__parameter_type( prim, 0 );

    if ( flags & PRIM__CONSTRUCTOR )
        ERROR( "environment__register_primitive: PRIM__CONSTRUCTOR not in use" );
    if ( flags & PRIM__DECODER )
        prim->return_type->decode = ( Decoder ) src_f;
    if ( flags & PRIM__DESTRUCTOR )
        first_param->destroy = ( Destructor ) src_f;
    if ( flags & PRIM__ENCODER )
        first_param->encode = ( Encoder ) src_f;

    if ( !( o = object__new( t, prim, OBJECT__IMMUTABLE ) ) )
    {
        primitive__delete( prim );
        return 0;
    }

    #if DEBUG__ENV
    printf( "[%#x] environment__register_primitive(%#x, %#x, %i, %#x)\n",
        ( int ) o, ( int ) env, ( int ) prim, flags, ( int ) src_f );
    #endif

    if ( !memory_manager__add( env->manager, o ) )
    {
        object__delete( o );
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
