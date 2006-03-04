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
    #endif

    if ( !name || !name->size )
        return ns_obj;

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


p2_object *p2_namespace__lookup_simple(
    p2_namespace__object *ns_obj, const char *s )
{
    #if DEBUG__SAFE
    if ( !ns_obj )
    {
        PRINTERR( "p2_namespace__lookup_simple: null namespace" );
        return 0;
    }
    #endif

    if ( !s )
        return ns_obj;
    else
        return ( p2_object* ) p2_hash_table__lookup(
            ( ( p2_namespace* ) ns_obj->value )->children, s );
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
    printf( "    0x%X '%s' : %s\n", ( int ) o, name, o->type->name );
    return o;
}

/* ! */
static p2_array *glob_array;
static p2_hash_table *glob_hash_table;
static void *add_to_array( void *p )
{
    return p2_array__enqueue( glob_array, p );
}
static void *lookup_and_print( char *s )
{
    return print_ns_item( s,
        ( p2_object* ) p2_hash_table__lookup( glob_hash_table, s ) );
}


void p2_namespace__show_children( p2_namespace__object *ns_obj )
{
    int size = ( ( p2_namespace* ) ns_obj->value )->children->size;

    printf( "0x%X : namespace", ( int ) ns_obj );

    if ( size )
    {
        glob_array = p2_array__new( size, 0 );
        p2_hash_table__for_all_keys(
            ( ( p2_namespace* ) ns_obj->value )->children,
            add_to_array );
        p2_array__mergesort( glob_array, ( int(*)(void*,void*))strcmp );

        printf( "\n{\n" );
        glob_hash_table = ( ( p2_namespace* ) ns_obj->value )->children;
        p2_array__for_all( glob_array, ( void*(*)(void*) ) lookup_and_print );
/*
        p2_hash_table__for_all( ( ( p2_namespace* ) ns_obj->value )->children, ( void*(*)(void*, void*) ) print_ns_item );
*/
        printf( "}" );

        p2_array__delete( glob_array );
    }

    else
        printf( " { }" );
}


void *p2_namespace__for_all( p2_namespace *ns, void *(*func)(void *) )
{
    return p2_hash_table__for_all_targets( ns->children, func );
}


/* Procedure which points the argument procedure to the target value of a
   hashing pair. */
static p2_procedure__effect apply_to_target( void **addr, p2_procedure *p )
{
    addr++;
    return p2_procedure__execute( p, addr );
}


void p2_namespace__distribute( p2_namespace *ns, p2_procedure *p )
{
    p2_procedure p_alt;

    p_alt.execute = ( procedure ) apply_to_target;
    p_alt.state = p;

    p2_hash_table__distribute( ns->children, &p_alt );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
