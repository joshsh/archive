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
static void *ns__add( p2_namespace__object *ns_obj, p2_object *o, char *s )
{
    p2_name *name;

    if ( !o || !( name = p2_array__new( 0, 0 ) ) )
        return 0;

    else if ( !p2_array__enqueue( name, s ) )
        o = 0;

    else
        p2_namespace__add( ns_obj, name, o );

    p2_array__delete( name );
    return o;
}


/* Look up a namespace item using a simple name. */
static p2_object *ns__lookup( p2_namespace__object *ns_obj, const char *s )
{
    char *s2;
    p2_name *name;
    p2_object *o;

    if ( !( s2 = STRDUP( s ) ) )
        return 0;

    if ( !ns_obj || !( name = p2_array__new( 0, 0 ) ) )
    {
        free( s2 );
        return 0;
    }

    else if ( !p2_array__enqueue( name, s2 ) )
        o = 0;

    else
        o = p2_namespace__lookup( ns_obj, name );

    free( s2 );
    p2_array__delete( name );
    return o;
}


/* Adds an object to a memory manager. */
/*
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
*/


static p2_namespace__object *ns__new( p2_type *ns__type )
{
    p2_object *o;
    p2_namespace *ns;

    if ( !( ns = p2_namespace__new() ) )
        return 0;

    if ( !( o = p2_object__new( ns__type, ns, OBJECT__IS_OBJ_COLL ) ) )
    {
        p2_namespace__delete( ns );
        return 0;
    }

    return o;
}


p2_environment *p2_environment__new()
{
    p2_environment *env;
printf( "---e 1---\n" ); fflush( stdout );

    if ( !( env = new( p2_environment ) ) )
        return 0;
printf( "---e 2---\n" ); fflush( stdout );

    env->prim__type = env->ns__type = env->type__type = 0;
    env->data = env->primitives = env->root = env->types = 0;
    env->manager = 0;
printf( "---e 3---\n" ); fflush( stdout );

    /* Create the basic data types. */
    if ( !( env->ns__type = p2_type__new(
        STRDUP( "namespace" ), 0, 0,
        ( DESTROY_T ) p2_namespace__delete, 0, 0,
        ( FOR_ALL_T ) p2_namespace__for_all ) )
      || !( env->prim__type = p2_type__new(
        STRDUP( "primitive" ), 0, 0,
        ( DESTROY_T ) p2_primitive__delete, 0, 0, 0 ) )
      || !( env->type__type = p2_type__new(
        STRDUP( "type" ), 0, 0,
        ( DESTROY_T ) p2_type__delete, 0, 0, 0 ) ) )
        goto abort;
printf( "---e 4---\n" ); fflush( stdout );

    /* Create root namespace object and children. */
    if ( !( env->data = ns__new( env->ns__type ) )
      || !( env->primitives = ns__new( env->ns__type ) )
      || !( env->root = ns__new( env->ns__type ) )
      || !( env->types = ns__new( env->ns__type ) ) )
        goto abort;
printf( "---e 5---\n" ); fflush( stdout );

    /* Create memory manager around root namespace object. */
    if ( !( env->manager = p2_memory_manager__new( env->root ) ) )
    {
        env->root = 0;
        goto abort;
    }
printf( "---e 6---\n" ); fflush( stdout );

    /* Add the other namespace objects to the manager. */
    if ( !( env->data = p2_memory_manager__add( env->manager, env->data ) )
      || !( env->primitives = p2_memory_manager__add( env->manager, env->primitives ) )
      || !( env->types = p2_memory_manager__add( env->manager, env->types ) ) )
        goto abort;
printf( "---e 7---\n" ); fflush( stdout );

    /* Nest child namespaces under root. */
    if ( !ns__add( env->root, env->data, "data" )
      || !ns__add( env->root, env->primitives, "primitives" )
      || !ns__add( env->root, env->types, "types" ) )
        goto abort;
printf( "---e 8---\n" ); fflush( stdout );

    /* Register the basic data types. */
    p2_environment__register_type( env, env->ns__type, 0 );
    p2_environment__register_type( env, env->prim__type, 0 );
    p2_environment__register_type( env, env->type__type, 0 );
printf( "---e 9---\n" ); fflush( stdout );

    /* ############################## Add other types here... */

    if ( !p2_environment__import_primitives( env ) )
        goto abort;
printf( "---e 10---\n" ); fflush( stdout );

    /* Set namespace permissions. */
    ( ( p2_namespace* ) env->data->value )->constant = 0;
    ( ( p2_namespace* ) env->primitives->value )->constant = 1;
    ( ( p2_namespace* ) env->root->value )->constant = 1;
    ( ( p2_namespace* ) env->types->value )->constant = 1;
printf( "---e 11---\n" ); fflush( stdout );

    return env;

abort:
printf( "---e abort---\n" ); fflush( stdout );

    if ( env->manager )
    {
        p2_memory_manager__delete( env->manager );
    }

    else
    {
        if ( env->ns__type )
            p2_type__delete( env->ns__type );

        if ( env->data )
            p2_object__delete( env->data );
        if ( env->primitives )
            p2_object__delete( env->primitives );
        if ( env->root )
            p2_object__delete( env->root );
        if ( env->types )
            p2_object__delete( env->types );
    }

    free( env );
    return 0;
}


void p2_environment__delete( p2_environment *env )
{
    #if DEBUG__SAFE
    if ( !env )
    {
        PRINTERR( "p2_environment__delete: null environment" );
        return;
    }
    #endif

    p2_memory_manager__delete( env->manager );
    free( env );
}


p2_object *p2_environment__register_primitive(
    p2_environment *env,
    p2_primitive *prim,
    int flags,
    void ( *src_f ) ( void ) )
{
    p2_object *o;

    if ( flags & PRIM__CONSTRUCTOR )
        PRINTERR( "p2_environment__register_primitive: PRIM__CONSTRUCTOR not in use" );
    if ( flags & PRIM__DECODER )
        prim->return_type->decode = ( DECODE_T ) src_f;
    if ( flags & PRIM__DESTRUCTOR )
        prim->return_type->destroy = ( DESTROY_T ) src_f;
    if ( flags & PRIM__ENCODER )
        prim->return_type->encode = ( ENCODE_T ) src_f;

    if ( !( o = p2_object__new( env->prim__type, prim, 0 ) ) )
    {
        p2_primitive__delete( prim );
        return 0;
    }

    if ( !p2_memory_manager__add( env->manager, o ) )
    {
        p2_object__delete( o );
        return 0;
    }

    return ns__add( env->primitives, o, prim->name );
}


p2_object *p2_environment__register_type(
    p2_environment *env,
    p2_type *type,
    int flags )
{
    p2_object *o = p2_object__new( env->type__type, type, flags );

    if ( !o )
        return 0;

    if ( !p2_memory_manager__add( env->manager, o ) )
    {
        p2_object__delete( o );
        return 0;
    }

    return ns__add( env->types, o, type->name );
}


p2_type *p2_environment__resolve_type(
    p2_environment *env,
    const char *name )
{
    p2_object *o = ns__lookup( env->types, name );
    p2_type *type;

    if ( !o )
    {
        /* If not found, create the type and hope for the best. */
        if ( !( type = p2_type__new( STRDUP( name ), 0, 0, 0, 0, 0, 0 ) ) )
            return 0;

        /* Note: all object collection types are registered explicitly. */
        if ( p2_environment__register_type( env, type, 0 ) )
            return type;

        else
        {
            p2_type__delete( type );
            return 0;
        }
    }

    #if DEBUG__SAFE
    if ( o->type != env->type__type )
    {
        PRINTERR( "p2_environment__resolve_type: type mismatch" );
printf( "    type = '%s' (%i) rather than '%s' (%i)\n",
    o->type->name, (int) o->type, env->ns__type->name, (int) env->ns__type );
        return 0;
    }
    #endif

    return ( p2_type* ) o->value;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
