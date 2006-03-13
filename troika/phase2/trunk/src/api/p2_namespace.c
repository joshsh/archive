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


p2_namespace *p2_namespace__new()
{
    p2_namespace *ns;

    if ( !( ns = new( p2_namespace ) ) )
    {
        ERROR( "p2_namespace__new: allocation failed" );
        return 0;
    }

    if ( !( ns->children = p2_dictionary__new() ) )
    {
        free( ns );
        return 0;
    }

    /* Writable by default. */
    ns->constant = 0;

    #if DEBUG__NAMESPACE
    printf( "[%#x] p2_namespace__new\n", ( int ) ns );
    #endif

    return ns;
}


void p2_namespace__delete( p2_namespace *ns )
{
    #if DEBUG__SAFE
        if ( !ns )
        {
            ERROR( "p2_namespace__delete: null namespace" );
            return;
        }
    #endif

    #if DEBUG__NAMESPACE
    printf( "p2_namespace__delete(%#x)\n", ( int ) ns ); fflush( stdout );
    #endif

    p2_dictionary__delete( ns->children );

    free( ns );
}


p2_object *p2_namespace__add
    ( p2_namespace_o *ns_obj, p2_name *name, p2_object *o )
{
    p2_namespace *ns = ( p2_namespace* ) ns_obj->value;

    p2_object *child_ns_obj, *displaced_object;
    char *key;

    #if DEBUG__SAFE
    if ( !ns )
    {
        ERROR( "p2_namespace__add: null namespace" );
        return 0;
    }
    if ( !name )
    {
        ERROR( "p2_namespace__add: null name" );
        return 0;
    }
    if ( !name->size )
    {
        ERROR( "p2_namespace__add: empty name" );
        return 0;
    }
    #endif

    if ( name->size == 1 )
    {
        if ( ns->constant )
        {
            ERROR( "p2_namespace__add: namespace is write-protected" );
            return 0;
        }

        displaced_object = ( p2_object* ) p2_dictionary__add(
            ns->children, ( char* ) p2_array__peek( name ), o );
    }

    else
    {
        key = ( char* ) p2_array__pop( name );

        child_ns_obj = ( p2_object* ) p2_dictionary__lookup( ns->children, key );

        if ( child_ns_obj->type != ns_obj->type )
        {
            ERROR( "p2_namespace__add: not a namespace" );
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


p2_object *p2_namespace__add_simple
    ( p2_namespace_o *ns_obj, const char *name, p2_object *o )
{
    p2_namespace *ns = ( p2_namespace* ) ns_obj->value;

    #if DEBUG__SAFE
    if ( !ns )
    {
        ERROR( "p2_namespace__add_simple: null namespace" );
        return 0;
    }
    if ( !name )
    {
        ERROR( "p2_namespace__add_simple: null name" );
        return 0;
    }
    #endif

    if ( ns->constant )
    {
        ERROR( "p2_namespace__add_simple: namespace is write-protected" );
        return 0;
    }

    return ( p2_object* ) p2_dictionary__add
        ( ns->children, name, o );
}


p2_object *p2_namespace__lookup( p2_namespace_o *ns_obj, p2_name *name )
{
    p2_namespace *ns = ( p2_namespace* ) ns_obj->value;

    p2_object *o;
    char *key;

    #if DEBUG__SAFE
    if ( !ns )
    {
        ERROR( "null namespace" );
        return 0;
    }
    #endif

    #if DEBUG__NAMESPACE
    printf( "[...] p2_namespace__lookup(%#x, %#x)\n", ( int ) ns, ( int ) name );
fflush( stdout );
    #endif

    if ( !name || !name->size )
        return ns_obj;

printf( "p2_namespace__lookup: p2_array__peek( name ) = %#x\n", ( int ) p2_array__peek( name ) ); fflush( stdout );
    o = ( p2_object* ) p2_dictionary__lookup( ns->children, p2_array__peek( name ) );

    /* Look for the object in a nested namespace. */
    if ( name->size > 1 )
    {
        /* Always check for this error, as namespace references may come directly
           from the user. */
        if ( o->type != ns_obj->type )
        {
            ERROR( "not a namespace" );
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
    p2_namespace_o *ns_obj, const char *name )
{
    #if DEBUG__SAFE
    if ( !ns_obj )
    {
        ERROR( "p2_namespace__lookup_simple: null namespace" );
        return 0;
    }
    #endif

    if ( !name )
        return ns_obj;
    else
        return ( p2_object* ) p2_dictionary__lookup(
            ( ( p2_namespace* ) ns_obj->value )->children, name );
}


p2_object *p2_namespace__remove( p2_namespace_o *ns_obj, p2_name *name )
{
    p2_namespace *ns = ( p2_namespace* ) ns_obj->value;

    p2_object *child_ns_obj, *displaced_object;
    char *key;

    #if DEBUG__SAFE
    if ( !ns )
    {
        ERROR( "null namespace" );
        return 0;
    }
    if ( !name )
    {
        ERROR( "null name" );
        return 0;
    }
    if ( !name->size )
    {
        ERROR( "empty name" );
        return 0;
    }
    #endif

    if ( name->size == 1 )
    {
        if ( ns->constant )
        {
            ERROR( "p2_namespace__remove: namespace is write-protected" );
            return 0;
        }

        displaced_object = ( p2_object* ) p2_dictionary__remove(
            ns->children, ( char* ) p2_array__peek( name ) );
    }

    else
    {
        key = ( char* ) p2_array__pop( name );

        child_ns_obj = ( p2_object* ) p2_dictionary__lookup( ns->children, key );

        if ( child_ns_obj->type != ns_obj->type )
        {
            ERROR( "not a namespace" );
            displaced_object = 0;
        }

        else
            displaced_object = p2_namespace__remove( child_ns_obj, name );

        p2_array__push( name, key );
    }

    return displaced_object;
}


/******************************************************************************/


void p2_namespace__distribute( p2_namespace *ns, p2_procedure *p )
{
    p2_dictionary__distribute( ns->children, p );
}


/******************************************************************************/


static p2_action * lookup_and_print( char *key, p2_dictionary *dict )
{
    p2_object *o = ( p2_object* ) p2_dictionary__lookup( dict, key );
    printf( "    %#x '%s' : %s\n", ( int ) o, key, o->type->name );
    return 0;
}


void p2_namespace__show_children( p2_namespace_o *ns_obj )
{
    int size = ( ( p2_namespace* ) ns_obj->value )->children->size;
    p2_array *a;
    p2_procedure p;

    printf( "%#x : namespace", ( int ) ns_obj );

    if ( size )
    {
        printf( "\n{\n" );

        /* Get alphabetized dictionary keys. */
        a = p2_dictionary__keys(
            ( ( p2_namespace* ) ns_obj->value )->children );

        /* Print children. */
        p.execute = ( procedure ) lookup_and_print;
        p.state = ( ( p2_namespace* ) ns_obj->value )->children;
        p2_array__distribute( a, &p );

        p2_array__delete( a );

        printf( "}" );
    }

    else
        printf( " { }" );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
