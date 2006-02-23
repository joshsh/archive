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

extern p2_type *p2_namespace__type;


/******************************************************************************/


p2_namespace *p2_namespace__new()
{
    return p2_hash_table__new( 0, 0, 0, STRING_DEFAULTS );
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
    return ( void* ) 1;
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

    /* The namespace hashing keys, but it does not own its targets. */
    p2_hash_table__for_all_keys( ns, p2_free );

    p2_hash_table__delete( ns );
}


p2_object *p2_namespace__add( p2_namespace *ns, p2_name *name, p2_object *o )
{
    p2_object *child_ns_object, *displaced_object;
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

    if ( name->size == 1 )
    {
        key = strdup( ( char* ) p2_array__peek( name ) );
        displaced_pair = p2_hash_table__add( ns, key, o );

        if ( displaced_pair.key )
            free( displaced_pair.key );

        displaced_object = ( p2_object* ) displaced_pair.target;
    }

    else
    {
        key = ( char* ) p2_array__pop( name );

        child_ns_object = ( p2_object* ) p2_hash_table__lookup( ns, key );

        if ( child_ns_object->type != p2_namespace__type )
        {
            PRINTERR( "not a namespace" );
            displaced_object = 0;
        }

        else
        {
            displaced_object = p2_namespace__add(
                ( p2_namespace* ) child_ns_object->value, name, o );
        }

        p2_array__push( name, key );
    }

    return displaced_object;
}


p2_object *p2_namespace__lookup( p2_namespace *ns, p2_name *name )
{
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

    o = ( p2_object* ) p2_hash_table__lookup( ns, p2_array__peek( name ) );

    /* Look for the object in a nested namespace. */
    if ( name->size > 1 )
    {
        /* Always check for this error, as namespace references may come directly
           from the user. */
        if ( o->type != p2_namespace__type )
        {
            PRINTERR( "not a namespace" );
            o = 0;
        }

        else
        {
            key = ( char* ) p2_array__pop( name );
            o = p2_namespace__lookup( ( p2_namespace* ) o->value, name );
            p2_array__push( name, key );
        }
    }

    return o;
}


p2_object *p2_namespace__remove( p2_namespace *ns, p2_name *name )
{
    p2_object *child_ns_object, *displaced_object;
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

    if ( name->size == 1 )
    {
        key = ( char* ) p2_array__peek( name );
        displaced_pair = p2_hash_table__remove( ns, key );

        if ( displaced_pair.key )
            free( displaced_pair.key );

        displaced_object = ( p2_object* ) displaced_pair.target;
    }

    else
    {
        key = ( char* ) p2_array__pop( name );

        child_ns_object = ( p2_object* ) p2_hash_table__lookup( ns, key );

        if ( child_ns_object->type != p2_namespace__type )
        {
            PRINTERR( "not a namespace" );
            displaced_object = 0;
        }

        else
        {
            displaced_object = p2_namespace__remove(
                ( p2_namespace* ) child_ns_object->value, name );
        }

        p2_array__push( name, key );
    }

    return displaced_object;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
