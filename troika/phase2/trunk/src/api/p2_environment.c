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

#include "p2_environment.h"
#include "p2_name.h"


/* Adds an object to a namespace using a simple name. */
static void *ns__add( p2_namespace *ns, p2_object *o, const char *s );
{
    p2_name *name;

    if ( !o || !( name = p2_array__new( 0, 0 ) ) )
        return 0;

    else if ( !p2_array__enqueue( name, s ) )
        o = 0;

    else
        p2_namespace__add( ns, o, name );

    p2_array__delete( name );
    return o;
}


/* Look up a namespace item using a simple name. */
static p2_object *ns__lookup( p2_namespace *ns, const char *s )
{
    p2_name *name;

    if ( !o || !( name = p2_array__new( 0, 0 ) ) )
        return 0;

    else if ( !p2_array__enqueue( name, s ) )
        o = 0;

    else
        o = p2_namespace__lookup( ns, name );

    p2_array__delete( name );
    return o;
}


/* Adds an object to a memory manager. */
static p2_object *mm__add( p2_memory_manager *m, void *p, p2_type *type )
{
    p2_object *o = p2_object__new( type, p );

    if ( !o )
        return 0;

    if ( !p2_memory_manager__add( m, o ) )
    {
        p2_object__delete( o );
        return 0;
    }

    return o;
}


p2_environment *p2_environment__new()
{
    p2_environment *env = 0;
    p2_object *root_obj = 0;
    p2_name *name;

    p2_type *ns__type;

    /* Create the "namespace" data type. */
    if ( !( ns__type = p2_type__new(
        STRDUP( "namespace" ), 0, 0,
        DESTROY_T p2_namespace__delete, 0, 0,
        FOR_ALL_T p2_namespace__for_all ) ) )
        return 0;

    if ( !( env = new( p2_environment ) ) )
        goto abort;

    env->data = env->primitives = env->root = env->types = 0;
    env->manager = 0;

    /* Create root namespace and children. */
    if ( !( env->data = p2_namespace__new() )
      || !( env->primitives = p2_namespace__new() )
      || !( env->root = p2_namespace__new() )
      || !( env->types = p2_namespace__new() ) )
        goto abort;

    /* Create memory manager and root namespace object. */
    if ( !( root_obj = p2_object__new( ns__type, root ) )
      || !( env->manager = p2_memory_manager__new( root_obj ) ) )
        goto abort;

    /* Create child namespace objects and add them to root. */
    if ( !ns__add( root, mm__add( env->manager, data, ns__type ), "data" )
      || !ns__add( root, mm__add( env->manager, primitives, ns__type ), "primitives" )
      || !ns__add( root, mm__add( env->manager, types, ns__type ), "types" ) )
        goto abort;

    env->data->constant = 0;
    env->primitives->constant = 1;
    env->root->constant = 1;
    env->types->constant = 1;

    /* ############################## Add other types here... */

    return env;

abort:

    p2_type__delete( ns__type );

    if ( env )
    {
        if ( env->manager )
        {

        }

        else
        {
            if ( env->data )
                p2_namespace__delete( env->data );
            if ( env->primitives )
                p2_namespace__delete( env->primitives );
            if ( env->root )
                p2_namespace__delete( env->root );
            if ( env->types )
                p2_namespace__delete( env->types );
        }

        free( env );
    }

    return 0;
}


void p2_environment__delete( p2_environment *env )
{
    ...
}


p2_object *p2_environment__register_primitive(
    p2_environment *env,
    p2_primitive *prim,
    int flags )
{
    return mm__add( env->manager, prim, env->prim__type );
}


p2_object *p2_environment__register_type(
    p2_environment *env,
    p2_type *type )
{
    return mm__add( env->manager, type, env->type__type );
}


p2_type *p2_environment__resolve_type(
    p2_environment *env,
    const char *name )
{
    p2_object *o = ns__lookup( env->types, name );

    if ( !o )
        return 0;

    #if DEBUG__SAFE
    if ( o->type != env->ns__type )
    {
        PRINTERR( "p2_environment__resolve_type: type mismatch" );
        return 0;
    }
    #endif

    return ( p2_type* ) o->value;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
