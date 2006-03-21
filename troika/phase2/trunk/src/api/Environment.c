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
printf( "---e m 1---\n" ); fflush( stdout );

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
    Primitive *prim;

    return ( ( prim = primitive__new( env, "any_type", "^+", assoc_stub, 3 ) )
      && primitive__add_param( env, prim, "any_type", "subject", 0 )
      && primitive__add_param( env, prim, "any_type", "predicate", 1 )
      && primitive__add_param( env, prim, "any_type", "object", 1 )
      && primitive__register( env, prim, 0, 0 )

      && ( prim = primitive__new( env, "any_type", "^", mult_stub, 2 ) )
      && primitive__add_param( env, prim, "any_type", "subject", 1 )
      && primitive__add_param( env, prim, "any_type", "predicate", 1 )
      && primitive__register( env, prim, 0, 0 ) );
}

#endif


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

printf( "---e 1---\n" ); fflush( stdout );

    if ( !( env = new( Environment ) ) )
        return 0;
printf( "---e 2---\n" ); fflush( stdout );

    #if DEBUG__ENV
    printf( "[%#x] environment__new()\n", ( int ) env );
    #endif

    env->prim_t = env->ns_t = env->type_t = 0;
    env->combinators = env->data = env->primitives = env->root = env->types = 0;
    env->manager = 0;
printf( "---e 3---\n" ); fflush( stdout );

    /* Create the basic data types. */
    if ( !( env->ns_t = type__new( "namespace", TYPE__IS_OBJ_COLL ) )
      || !( env->prim_t = type__new( "primitive", 0 ) )
      || !( env->type_t = type__new( "type", 0 ) ) )
        goto abort;

    env->ns_t->destroy =    ( destructor )  namespace__delete;
    env->ns_t->distribute = ( distributor ) namespace__distribute;
    env->prim_t->destroy =  ( destructor )  primitive__delete;
    env->prim_t->encode = ( encoder ) primitive__encode;
    env->type_t->destroy =  ( destructor )  type__delete;

printf( "---e 4---\n" ); fflush( stdout );

    /* Create root namespace object and children. */
    if ( !( env->combinators = ns__new( env->ns_t ) )
      || !( env->data = ns__new( env->ns_t ) )
      || !( env->primitives = ns__new( env->ns_t ) )
      || !( env->root = ns__new( env->ns_t ) )
      || !( env->types = ns__new( env->ns_t ) ) )
        goto abort;
printf( "---e 5---\n" ); fflush( stdout );

    /* Create memory manager around root namespace object. */
    if ( !( env->manager = memory_manager__new( env->root ) ) )
    {
        env->root = 0;
        goto abort;
    }
printf( "---e 6---\n" ); fflush( stdout );

    /* Add the other namespace objects to the manager. */
    if ( !( env->combinators = memory_manager__add( env->manager, env->combinators ) )
      || !( env->data = memory_manager__add( env->manager, env->data ) )
      || !( env->primitives = memory_manager__add( env->manager, env->primitives ) )
      || !( env->types = memory_manager__add( env->manager, env->types ) ) )
        goto abort;
printf( "---e 7---\n" ); fflush( stdout );

    /* Nest child namespaces under root. */
    if ( namespace__add_simple( ( Namespace* ) env->root->value, "combinators", env->combinators )
      || namespace__add_simple( ( Namespace* ) env->root->value, "data", env->data )
      || namespace__add_simple( ( Namespace* ) env->root->value, "primitives", env->primitives )
      || namespace__add_simple( ( Namespace* ) env->root->value, "types", env->types ) )
        goto abort;
printf( "---e 8---\n" ); fflush( stdout );

    /* Register the basic data types. */
    environment__register_type( env, env->ns_t );
    environment__register_type( env, env->prim_t );
    environment__register_type( env, env->type_t );
printf( "---e 9---\n" ); fflush( stdout );

    /* Add other types here... */
    environment__register_type( env, array__type( "bag", TYPE__IS_OBJ_COLL ) );
    environment__register_type( env, term__type( "term", TYPE__IS_OBJ_COLL ) );

    /* Add primitives. */
    if ( !environment__import_primitives( env ) )
        goto abort;
printf( "---e 10---\n" ); fflush( stdout );

    #if TRIPLES__GLOBAL
    if ( !add_triples_prims( env ) )
    {
        ERROR( "environment__new: failed to add triples primitives" );
        goto abort;
    }
    #endif

    /* Add combinators. */
    env->combinator_t = environment__resolve_type( env, "Combinator" );
    add_combinators( env );

    lock_ns( env );

printf( "---e 11---\n" ); fflush( stdout );

    return env;

abort:
printf( "---e abort---\n" ); fflush( stdout );

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

printf( "---e d 1---\n" ); fflush( stdout );
    /* Preserve only data type objects. */
    memory_manager__set_root( env->manager, env->types );
printf( "---e d 5---\n" ); fflush( stdout );
    memory_manager__mark_and_sweep( env->manager );

printf( "---e d 6---\n" ); fflush( stdout );
    /* Preserve only the 'type' type. */
    ns_t = *env->ns_t;
    env->types->type = &ns_t;
    memory_manager__set_root( env->manager,
        namespace__lookup_simple( ( Namespace* ) env->types->value, "type" ) );
printf( "---e d 7---\n" ); fflush( stdout );
    memory_manager__mark_and_sweep( env->manager );

printf( "---e d 8---\n" ); fflush( stdout );
    memory_manager__delete( env->manager );
printf( "---e d 9---\n" ); fflush( stdout );

    free( env );
}


Object *
environment__register_primitive
    ( Environment *env, Primitive *prim, int flags, generic_f src_f )
{
    Object *o;

    Type *first_param = prim->parameters[0].type;

    if ( flags & PRIM__CONSTRUCTOR )
        ERROR( "environment__register_primitive: PRIM__CONSTRUCTOR not in use" );
    if ( flags & PRIM__DECODER )
        prim->return_type->decode = ( decoder ) src_f;
    if ( flags & PRIM__DESTRUCTOR )
        first_param->destroy = ( destructor ) src_f;
    if ( flags & PRIM__ENCODER )
        first_param->encode = ( encoder ) src_f;

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


Object *
environment__register_type( Environment *env, Type *type )
{
    Object *o = object__new( env->type_t, type, OBJECT__IMMUTABLE );

    #if DEBUG__ENV
    printf( "[%#x] environment__register_type(%#x, %#x)\n",
        ( int ) o, ( int ) env, ( int ) type );
    #endif

    if ( !o )
        return 0;

    if ( !memory_manager__add( env->manager, o ) )
    {
        object__delete( o );
        return 0;
    }

    if ( namespace__add_simple( ( Namespace* ) env->types->value, type->name, o ) )
        return 0;

    else
        return o;
}


Type *
environment__resolve_type( Environment *env, const char *name )
{
    Object *o;
    Type *type;

    if ( !strcmp( name, "any_type" ) )
        return any_type;

    if ( !(o = namespace__lookup_simple( ( Namespace* ) env->types->value, name ) ) )
    {
        /* If not found, create the type and hope for the best. */
        if ( !( type = type__new( name, 0 ) ) )
            return 0;

        /* Note: all object collection types are registered explicitly. */
        if ( environment__register_type( env, type ) )
            return type;

        else
        {
            type__delete( type );
            return 0;
        }
    }

    #if DEBUG__SAFE
    if ( o->type != env->type_t )
    {
        ERROR( "environment__resolve_type: type mismatch" );
        return 0;
    }
    #endif

    return ( Type* ) o->value;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
