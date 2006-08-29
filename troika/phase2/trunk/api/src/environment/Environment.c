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

#include <collection/Name.h>
#include <collection/Term.h>

#include "Environment-impl.h"


static Namespace_o *
ns__new( Manager *m, Type *ns_t )
{
    Object *o;
    Namespace *ns;

    if ( !( ns = namespace__new() ) )
        return 0;

    if ( !( o = memory_manager__object( m, ns_t, ns, NOFLAGS ) ) )
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

    if ( !( env = NEW( Environment ) ) )
        return 0;

    env->type_t = 0;
    env->combinators = env->data = env->primitives = env->root = env->types = 0;
    env->manager = 0;

    /* Create the basic data types. */
    if ( !( ( ns_t = namespace__create_type( NAMEOF( NAMESPACE ), TYPE__IS_OBJ_COLL ) )
         && ( prim_t = primitive__create_type( NAMEOF( PRIMITIVE ) ) )
         && ( set_t = set__create_type( NAMEOF( SET ), TYPE__IS_OBJ_COLL ) )
         && ( env->type_t = type__create_type( NAMEOF( TYPE ) ) ) ) )
        goto abort;

    /* Create memory manager. */
    if ( !( env->manager = memory_manager__new() ) )
        goto abort;

    /* Create root namespace object and children. */
    if ( !( env->combinators = ns__new( env->manager, ns_t ) )
      || !( env->data = ns__new( env->manager, ns_t ) )
      || !( env->primitives = ns__new( env->manager, ns_t ) )
      || !( env->root = ns__new( env->manager, ns_t ) )
      || !( env->types = ns__new( env->manager, ns_t ) ) )
        goto abort;

    memory_manager__set_root( env->manager, env->root );

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
    environment__register_type( env, array__create_type( NAMEOF( ARRAY ), TYPE__IS_OBJ_COLL ) );
    environment__register_type( env, term__create_type( NAMEOF( TERM ), TYPE__IS_OBJ_COLL ) );

    /* Add primitives. */
    environment__import_primitives( env );

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

    if ( DEBUG__SAFE && !env )
        abort();

    /* Preserve only data type objects. */
    memory_manager__set_root( env->manager, env->types );
    memory_manager__collect( env->manager, TRUE, FALSE );

    /* Preserve only the 'type' type. */
    ns_t = *( env->types->type );

/* ~ */
    ns_t.name = STRDUP( ns_t.name );  /* Just so that the debugging output doesn't look weird. */

    env->types->type = &ns_t;
    memory_manager__set_root( env->manager,
        namespace__lookup_simple( env->types->value, NAMEOF( TYPE ) ) );
    memory_manager__collect( env->manager, TRUE, FALSE );

    memory_manager__delete( env->manager );

    free( ns_t.name );
    free( env );
}


Manager *
environment__manager( const Environment *env )
{
    if ( DEBUG__SAFE && !env )
        abort();

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
