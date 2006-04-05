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

#include <Environment.h>
#include <util/Name.h>
#include <util/Term.h>
#include <sk/sk.h>


static Namespace_o *
ns__new( Type *ns_t )
{
    Object *o;
    Namespace *ns;

    if ( !( ns = namespace__new() ) )
        return 0;

    if ( !( o = object__new( ns_t, ns, 0 ) ) )
    {
        namespace__delete( ns );
        return 0;
    }

    return o;
}


/******************************************************************************/


#if TRIPLES__GLOBAL

#include <Primitive-import.h>

static void *
assoc_stub( void **args )
{
    return object__associate( args[0], args[1], args[2] );
}

static void *
mult_stub( void **args )
{
    return object__multiply( args[0], args[1] );
}

static int
add_triples_prims( Environment *env )
{
    Primitive *p;

    return ( ( p = primitive__new( env, ANY__NAME, "^+", assoc_stub, 3 ) )
      && primitive__add_param( env, p, ANY__NAME, "subject", 0 )
      && primitive__add_param( env, p, ANY__NAME, "predicate", 1 )
      && primitive__add_param( env, p, ANY__NAME, "object", 1 )
      && primitive__register( env, p, 0, 0 )

      && ( p = primitive__new( env, ANY__NAME, "^", mult_stub, 2 ) )
      && primitive__add_param( env, p, ANY__NAME, "subject", 1 )
      && primitive__add_param( env, p, ANY__NAME, "predicate", 1 )
      && primitive__register( env, p, 0, 0 ) );
}

#endif


static Environment *global_env;

/* Note: this is a slow as it looks. */
static void *
type_of( void **args )
{
    Object *o = args[0];
    return namespace__lookup_simple( global_env->types->value, o->type->name );
}


static int
add_meta_prims( Environment *env )
{
    Primitive *p;

    global_env = env;

    return ( ( p = primitive__new( env, ANY__NAME, "type_of", type_of, 1 ) )
      && primitive__add_param( env, p, ANY__NAME, "self", 1 )
      && primitive__register( env, p, 0, 0 ) );
}


/******************************************************************************/


static void
add_combinators( Environment *env )
{
    Object *o;
    Combinator *sk_s, *sk_k;
    Memory_Manager *m = env->manager;

    sk_s = new( Combinator );
    sk_k = new( Combinator );

    *sk_s = S_combinator;
    *sk_k = K_combinator;

    o = object__new( env->combinator_t, sk_s, OBJECT__IMMUTABLE );
    memory_manager__add( m, o );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "S", o );

    o = object__new( env->combinator_t, sk_k, OBJECT__IMMUTABLE );
    memory_manager__add( m, o );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "K", o );
}


/* Make all namespaces apart from the 'data' namespace read-only. */
static void
lock_ns( Environment *env )
{
    env->combinators->flags |= OBJECT__IMMUTABLE;
    env->primitives->flags |= OBJECT__IMMUTABLE;
    env->root->flags |= OBJECT__IMMUTABLE;
    env->types->flags |= OBJECT__IMMUTABLE;
}


/* Remove
static void unlock_ns( Environment *env )
{

}*/


/******************************************************************************/


Environment *
environment__new()
{
    Environment *env;

    if ( !( env = new( Environment ) ) )
        return 0;

    #if DEBUG__ENV
    printf( "[%#x] environment__new()\n", ( int ) env );
    #endif

    env->prim_t = env->ns_t = env->type_t = 0;
    env->combinators = env->data = env->primitives = env->root = env->types = 0;
    env->manager = 0;

    /* Create the basic data types. */
    if ( !( env->ns_t = namespace__create_type( NAMESPACE__NAME, TYPE__IS_OBJ_COLL ) )
      || !( env->prim_t = primitive__create_type( PRIMITIVE__NAME ) )
      || !( env->set_t = set__create_type( SET__NAME, TYPE__IS_OBJ_COLL ) )
      || !( env->type_t = type__create_type( TYPE__NAME, 0 ) ) )
        goto abort;

    /* Create root namespace object and children. */
    if ( !( env->combinators = ns__new( env->ns_t ) )
      || !( env->data = ns__new( env->ns_t ) )
      || !( env->primitives = ns__new( env->ns_t ) )
      || !( env->root = ns__new( env->ns_t ) )
      || !( env->types = ns__new( env->ns_t ) ) )
        goto abort;

    /* Create memory manager around root namespace object. */
    if ( !( env->manager = memory_manager__new( env->root ) ) )
    {
        env->root = 0;
        goto abort;
    }

    /* Add the other namespace objects to the manager. */
    if ( !memory_manager__add( env->manager, env->combinators )
      || !memory_manager__add( env->manager, env->data )
      || !memory_manager__add( env->manager, env->primitives )
      || !memory_manager__add( env->manager, env->types ) )
        goto abort;

    /* Nest child namespaces under root. */
    if ( namespace__add_simple( ( Namespace* ) env->root->value, "combinators", env->combinators )
      || namespace__add_simple( ( Namespace* ) env->root->value, "data", env->data )
      || namespace__add_simple( ( Namespace* ) env->root->value, "primitives", env->primitives )
      || namespace__add_simple( ( Namespace* ) env->root->value, "types", env->types ) )
        goto abort;

    /* Register the basic data types. */
    environment__register_type( env, env->ns_t );
    environment__register_type( env, env->prim_t );
    environment__register_type( env, env->set_t );
    environment__register_type( env, env->type_t );

    /* Add other types here... */
    environment__register_type( env, array__create_type( BAG__NAME, TYPE__IS_OBJ_COLL ) );
    environment__register_type( env, term__create_type( TERM__NAME, TYPE__IS_OBJ_COLL ) );

    /* Add primitives. */
    if ( !environment__import_primitives( env ) )
        goto abort;

    #if TRIPLES__GLOBAL
    if ( !add_triples_prims( env ) )
    {
        ERROR( "environment__new: failed to add triples primitives" );
        goto abort;
    }
    #endif

    if ( !add_meta_prims( env ) )
    {
        ERROR( "environment__new: failed to add \"meta\" primitives" );
        goto abort;
    }

    env->bag_t = environment__resolve_type( env, BAG__NAME );
    env->char_t = environment__resolve_type( env, CHAR__NAME );
    env->float_t = environment__resolve_type( env, DOUBLE__NAME );
    env->int_t = environment__resolve_type( env, INT__NAME );
    env->string_t = environment__resolve_type( env, STRING__NAME );
    env->term_t = environment__resolve_type( env, TERM__NAME );

    /* Add combinators. */
    env->combinator_t = environment__resolve_type( env, COMBINATOR__NAME );
    add_combinators( env );

    lock_ns( env );


    return env;

abort:

    if ( env->manager )
    {
        memory_manager__delete( env->manager );
    }

    else
    {
        if ( env->ns_t )
            type__delete( env->ns_t );

        if ( env->data )
            object__delete( env->data );
        if ( env->primitives )
            object__delete( env->primitives );
        if ( env->root )
            object__delete( env->root );
        if ( env->types )
            object__delete( env->types );
    }

    free( env );
    return 0;
}


void
environment__delete( Environment *env )
{
    Type ns_t;

    #if DEBUG__SAFE
    if ( !env )
    {
        ERROR( "environment__delete: null environment" );
        return;
    }
    #endif

    #if DEBUG__ENV
    printf( "[] environment__delete(%#x)\n", ( int ) env );
    #endif

    /* Preserve only data type objects. */
    memory_manager__set_root( env->manager, env->types );
    memory_manager__collect( env->manager );

    /* Preserve only the 'type' type. */
    ns_t = *env->ns_t;
    ns_t.name = STRDUP( ns_t.name );  /* Just so that the debugging output doesn't look weird. */
    env->types->type = &ns_t;
    memory_manager__set_root( env->manager,
        namespace__lookup_simple( ( Namespace* ) env->types->value, TYPE__NAME ) );
    memory_manager__collect( env->manager );

    memory_manager__delete( env->manager );

    free( ns_t.name );
    free( env );
}


Object *
environment__register_primitive
    ( Environment *env, Primitive *prim, int flags, Generic_f src_f )
{
    Object *o;

    Type *first_param = prim->parameters[0].type;

    if ( flags & PRIM__CONSTRUCTOR )
        ERROR( "environment__register_primitive: PRIM__CONSTRUCTOR not in use" );
    if ( flags & PRIM__DECODER )
        prim->return_type->decode = ( Decoder ) src_f;
    if ( flags & PRIM__DESTRUCTOR )
        first_param->destroy = ( Destructor ) src_f;
    if ( flags & PRIM__ENCODER )
        first_param->encode = ( Encoder ) src_f;

    if ( !( o = object__new( env->prim_t, prim, OBJECT__IMMUTABLE ) ) )
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

    if ( namespace__add_simple( ( Namespace* ) env->primitives->value, prim->name, o ) )
        return 0;

    else
        return o;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
