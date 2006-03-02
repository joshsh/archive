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

#include "p2_namespace.h"

#include <stdio.h>  /* fprintf */
#include <string.h>  /* strdup */


/******************************************************************************/


p2_namespace *p2_namespace__new()
{
    p2_namespace *ns = new( p2_namespace );

    if ( ns )
    {
        if ( !( ns->children = p2_hash_table__new( 0, 0, 0, STRING_DEFAULTS ) ) )
        {
            free( ns );
            ns = 0;
        }
    }

    #if DEBUG__SAFE
    if ( !ns )
    {
        PRINTERR( "p2_namespace__new: allocation failed" );
        return 0;
    }
    #endif

    /* Writable by default. */
    ns->constant = 0;

    return ns;
}


static void *p2_free( void *p )
{
    #if DEBUG__SAFE
    if ( !p )
    {
        PRINTERR( "p2_free in p2_namespace.c: null pointer" );
        return 0;
    }
    #endif

    free( p );
    return p;
}


void p2_namespace__delete( p2_namespace *ns )
{
    #if DEBUG__SAFE
        if ( !ns )
        {
            PRINTERR( "p2_namespace__delete: null namespace" );
            return;
        }
    #endif

    /* The namespace owns its hashing keys, but it does not own its targets. */
    p2_hash_table__for_all_keys( ns->children, p2_free );

    p2_hash_table__delete( ns->children );

    free( ns );
}


p2_object *p2_namespace__add( p2_namespace__object *ns_obj, p2_name *name, p2_object *o )
{
    p2_namespace *ns = ( p2_namespace* ) ns_obj->value;

    p2_object *child_ns_obj, *displaced_object;
    p2_hashing_pair displaced_pair;
    char *key;

    #if DEBUG__SAFE

        if ( !ns )
        {
            PRINTERR( "null namespace" );
            return 0;
        }

        if ( !name )
        {
            PRINTERR( "null name" );
            return 0;
        }

        if ( !name->size )
        {
            PRINTERR( "empty name" );
            return 0;
        }

    #endif

    if ( ns->constant )
    {
        PRINTERR( "p2_namespace__add: namespace is write-protected" );
        return 0;
    }

    if ( name->size == 1 )
    {
        key = strdup( ( char* ) p2_array__peek( name ) );
        displaced_pair = p2_hash_table__add( ns->children, key, o );

        if ( displaced_pair.key )
            free( displaced_pair.key );

        displaced_object = ( p2_object* ) displaced_pair.target;
    }

    else
    {
        key = ( char* ) p2_array__pop( name );

        child_ns_obj = ( p2_object* ) p2_hash_table__lookup( ns->children, key );

        if ( child_ns_obj->type != ns_obj->type )
        {
            PRINTERR( "not a namespace" );
            displaced_object = 0;
        }

        else
        {
            displaced_object = p2_namespace__add(
                child_ns_obj, name, o );
        }

        p2_array__push( name, key );
    }

    return displaced_object;
}


p2_object *p2_namespace__lookup( p2_namespace__object *ns_obj, p2_name *name )
{
    p2_namespace *ns = ( p2_namespace* ) ns_obj->value;

    p2_object *o;
    char *key;

    #if DEBUG__SAFE

        if ( !ns )
        {
            PRINTERR( "null namespace" );
            return 0;
        }

        if ( !name )
        {
            PRINTERR( "null name" );
            return 0;
        }

        if ( !name->size )
        {
            PRINTERR( "empty name" );
            return 0;
        }

    #endif

    o = ( p2_object* ) p2_hash_table__lookup( ns->children, p2_array__peek( name ) );

    /* Look for the object in a nested namespace. */
    if ( name->size > 1 )
    {
        /* Always check for this error, as namespace references may come directly
           from the user. */
        if ( o->type != ns_obj->type )
        {
            PRINTERR( "not a namespace" );
            o = 0;
        }

        else
        {
            key = ( char* ) p2_array__pop( name );
            o = p2_namespace__lookup( o, name );
            p2_array__push( name, key );
        }
    }

    return o;
}


p2_object *p2_namespace__remove( p2_namespace__object *ns_obj, p2_name *name )
{
    p2_namespace *ns = ( p2_namespace* ) ns_obj->value;

    p2_object *child_ns_obj, *displaced_object;
    p2_hashing_pair displaced_pair;
    char *key;

    #if DEBUG__SAFE

        if ( !ns )
        {
            PRINTERR( "null namespace" );
            return 0;
        }

        if ( !name )
        {
            PRINTERR( "null name" );
            return 0;
        }

        if ( !name->size )
        {
            PRINTERR( "empty name" );
            return 0;
        }

    #endif

    if ( ns->constant )
    {
        PRINTERR( "p2_namespace__remove: namespace is write-protected" );
        return 0;
    }

    if ( name->size == 1 )
    {
        key = ( char* ) p2_array__peek( name );
        displaced_pair = p2_hash_table__remove( ns->children, key );

        if ( displaced_pair.key )
            free( displaced_pair.key );

        displaced_object = ( p2_object* ) displaced_pair.target;
    }

    else
    {
        key = ( char* ) p2_array__pop( name );

        child_ns_obj = ( p2_object* ) p2_hash_table__lookup( ns->children, key );

        if ( child_ns_obj->type != ns_obj->type )
        {
            PRINTERR( "not a namespace" );
            displaced_object = 0;
        }

        else
            displaced_object = p2_namespace__remove( child_ns_obj, name );

        p2_array__push( name, key );
    }

    return displaced_object;
}


static void *print_ns_item( char *name, p2_object *o )
{
    printf( "    0x%X %s (%s)\n", ( int ) o, name, o->type->name );
    return o;
}


void p2_namespace__show_children( p2_namespace__object *ns_obj )
{
    printf( "Namespace at 0x%X", ( int ) ns_obj );

    if ( ( ( p2_namespace* ) ns_obj->value )->children->size )
    {
        printf( "\n{\n" );
        p2_hash_table__for_all( ( ( p2_namespace* ) ns_obj->value )->children, ( void*(*)(void*, void*) ) print_ns_item );
        printf( "}" );
    }

    else
        printf( " { }" );
}


void *p2_namespace__for_all( p2_namespace *ns, void *(*func)(void *) )
{
    return p2_hash_table__for_all_targets( ns->children, func );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
