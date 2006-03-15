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
#include "util/p2_name.h"
#include "util/p2_term.h"


static p2_namespace_o *ns__new( p2_type *ns_t )
{
    p2_object *o;
    p2_namespace *ns;

    if ( !( ns = p2_namespace__new() ) )
        return 0;

    if ( !( o = p2_object__new( ns_t, ns, OBJECT__IS_OBJ_COLL ) ) )
    {
        p2_namespace__delete( ns );
        return 0;
    }

    return o;
}


/* To be moved... *************************************************************/

/*
static void sequence_item__print

typedef struct _print_format
{
    char *prefix, *infix, *suffix;

} print_format;


struct

static char *print
void p2_object__print( p2_object *o, char *buffer )
{
    if ( o & OBJECT__IS_OBJ_COLL )
    {
        o->type->for_all( o->value, 
    }

    else
        o->type->encode( o->value, buffer );
}
*/

/******************************************************************************/


p2_environment *p2_environment__new()
{
    p2_environment *env;

printf( "---e 1---\n" ); fflush( stdout );

    if ( !( env = new( p2_environment ) ) )
        return 0;
printf( "---e 2---\n" ); fflush( stdout );

    env->prim_t = env->ns_t = env->type_t = 0;
    env->data = env->primitives = env->root = env->types = 0;
    env->manager = 0;
printf( "---e 3---\n" ); fflush( stdout );

    /* Create the basic data types. */
    if ( !( env->ns_t = p2_type__new( "namespace", 0 ) )
      || !( env->prim_t = p2_type__new( "primitive", 0 ) )
      || !( env->type_t = p2_type__new( "type", 0 ) ) )
        goto abort;

    env->ns_t->destroy =    ( destructor )  p2_namespace__delete;
    env->ns_t->distribute = ( distributor ) p2_namespace__distribute;
    env->prim_t->destroy =  ( destructor )  p2_primitive__delete;
    env->prim_t->encode = ( encoder ) p2_primitive__encode;
    env->type_t->destroy =  ( destructor )  p2_type__delete;
/*
    if ( !( env->ns_t = p2_type__new(
        STRDUP( "namespace" ), 0, 0,
        ( destructor ) p2_namespace__delete, 0, 0,
        ( FOR_ALL_T ) p2_namespace__for_all, 0 ) )
      || !( env->prim_t = p2_type__new(
        STRDUP( "primitive" ), 0, 0,
        ( destructor ) p2_primitive__delete, 0, 0, 0, 0 ) )
      || !( env->type_t = p2_type__new(
        STRDUP( "type" ), 0, 0,
        ( destructor ) p2_type__delete, 0, 0, 0, 0 ) ) )
        goto abort;
*/
printf( "---e 4---\n" ); fflush( stdout );

    /* Create root namespace object and children. */
    if ( !( env->data = ns__new( env->ns_t ) )
      || !( env->primitives = ns__new( env->ns_t ) )
      || !( env->root = ns__new( env->ns_t ) )
      || !( env->types = ns__new( env->ns_t ) ) )
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
    if ( p2_namespace__add_simple( env->root, "data", env->data )
      || p2_namespace__add_simple( env->root, "primitives", env->primitives )
      || p2_namespace__add_simple( env->root, "types", env->types ) )
        goto abort;
printf( "---e 8---\n" ); fflush( stdout );

    /* Register the basic data types. */
    p2_environment__register_type( env, env->ns_t, 0 );
    p2_environment__register_type( env, env->prim_t, 0 );
    p2_environment__register_type( env, env->type_t, 0 );
printf( "---e 9---\n" ); fflush( stdout );

    /* Add other types here... */
    p2_environment__register_type( env, p2_array__type( "bag" ), 0 );
    p2_environment__register_type( env, p2_term__type( "term" ), 0 );

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
        if ( env->ns_t )
            p2_type__delete( env->ns_t );

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
        ERROR( "p2_environment__delete: null environment" );
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

    p2_type *first_param = prim->parameters[0].type;

    if ( flags & PRIM__CONSTRUCTOR )
        ERROR( "p2_environment__register_primitive: PRIM__CONSTRUCTOR not in use" );
    if ( flags & PRIM__DECODER )
        first_param->decode = ( decoder ) src_f;
    if ( flags & PRIM__DESTRUCTOR )
        first_param->destroy = ( destructor ) src_f;
    if ( flags & PRIM__ENCODER )
        first_param->encode = ( encoder ) src_f;

    if ( !( o = p2_object__new( env->prim_t, prim, 0 ) ) )
    {
        p2_primitive__delete( prim );
        return 0;
    }

    if ( !p2_memory_manager__add( env->manager, o ) )
    {
        p2_object__delete( o );
        return 0;
    }

    if ( p2_namespace__add_simple( env->primitives, prim->name, o ) )
        return 0;

    else
        return o;
}


p2_object *p2_environment__register_type(
    p2_environment *env,
    p2_type *type,
    int flags )
{
    p2_object *o = p2_object__new( env->type_t, type, flags );

    if ( !o )
        return 0;

    if ( !p2_memory_manager__add( env->manager, o ) )
    {
        p2_object__delete( o );
        return 0;
    }

    if ( p2_namespace__add_simple( env->types, type->name, o ) )
        return 0;

    else
        return o;
}


p2_type *p2_environment__resolve_type(
    p2_environment *env,
    const char *name )
{
    p2_object *o = p2_namespace__lookup_simple( env->types, name );
    p2_type *type;

    if ( !o )
    {
        /* If not found, create the type and hope for the best. */
        if ( !( type = p2_type__new( name, 0 ) ) )
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
    if ( o->type != env->type_t )
    {
        ERROR( "p2_environment__resolve_type: type mismatch" );
printf( "    type = '%s' (%i) rather than '%s' (%i)\n",
o->type->name, (int) o->type, env->ns_t->name, (int) env->ns_t );
        return 0;
    }
    #endif

    return ( p2_type* ) o->value;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
