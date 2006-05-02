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

#include <util/Name.h>
#include <util/Term.h>

#include "Environment-impl.h"


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

    Type *ns_t = 0, *prim_t = 0, *set_t = 0;

    if ( !( env = new( Environment ) ) )
        return 0;

    #if DEBUG__ENV
    printf( "[%#x] environment__new()\n", ( int ) env );
    #endif

    env->type_t = 0;
    env->combinators = env->data = env->primitives = env->root = env->types = 0;
    env->manager = 0;

    /* Create the basic data types. */
    if ( !( ( ns_t = namespace__create_type( NAMESPACE__NAME, TYPE__IS_OBJ_COLL ) )
         && ( prim_t = primitive__create_type( PRIMITIVE__NAME ) )
         && ( set_t = set__create_type( SET__NAME, TYPE__IS_OBJ_COLL ) )
         && ( env->type_t = type__create_type( TYPE__NAME, 0 ) ) ) )
        goto abort;

    /* Create root namespace object and children. */
    if ( !( env->combinators = ns__new( ns_t ) )
      || !( env->data = ns__new( ns_t ) )
      || !( env->primitives = ns__new( ns_t ) )
      || !( env->root = ns__new( ns_t ) )
      || !( env->types = ns__new( ns_t ) ) )
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
    if ( !namespace__add_simple( env->root->value, "combinators", env->combinators )
      || !namespace__add_simple( env->root->value, "data", env->data )
      || !namespace__add_simple( env->root->value, "primitives", env->primitives )
      || !namespace__add_simple( env->root->value, "types", env->types ) )
        goto abort;

    /* Register the basic data types. */
    environment__register_type( env, ns_t );
    environment__register_type( env, prim_t );
    environment__register_type( env, set_t );
    environment__register_type( env, env->type_t );

    /* Add other types here... */
    environment__register_type( env, array__create_type( BAG__NAME, TYPE__IS_OBJ_COLL ) );
    environment__register_type( env, term__create_type( TERM__NAME, TYPE__IS_OBJ_COLL ) );

    /* Add primitives. */
    if ( !environment__import_primitives( env ) )
        goto abort;

    if ( !add_meta_prims( env ) )
    {
        ERROR( "environment__new: failed to add \"meta\" primitives" );
        goto abort;
    }

    /* Add combinators. */
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
        if ( ns_t )
            type__delete( ns_t );

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
        ERROR( "environment__delete: null argument" );
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
    ns_t = *( env->types->type );
    ns_t.name = STRDUP( ns_t.name );  /* Just so that the debugging output doesn't look weird. */
    env->types->type = &ns_t;
    memory_manager__set_root( env->manager,
        namespace__lookup_simple( env->types->value, TYPE__NAME ) );
    memory_manager__collect( env->manager );

    memory_manager__delete( env->manager );

    free( ns_t.name );
    free( env );
}


Memory_Manager *
environment__manager( const Environment *env )
{
    #if DEBUG__SAFE
    if ( !env )
    {
        ERROR( "environment__manager: null argument" );
        return 0;
    }
    #endif

    return env->manager;
}



Namespace_o *
environment__data( const Environment *env )
{
    return env->data;
}


Namespace_o *
environment__root( const Environment *env )
{
    return env->root;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
