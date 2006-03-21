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

    if ( !( ns->children = dictionary__new() ) )
    {
        free( ns );
        return 0;
    }

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

    dictionary__delete( ns->children );

    free( ns );
}


Object *p2_namespace__add
    ( p2_namespace_o *ns_obj, p2_name *name, Object *o )
{
    p2_namespace *ns = ( p2_namespace* ) ns_obj->value;

    Object *child_ns_obj, *displaced_object;
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
    if ( !array__size( name ) )
    {
        ERROR( "p2_namespace__add: empty name" );
        return 0;
    }
    #endif

    if ( array__size( name ) == 1 )
    {
        if ( ns_obj->flags & OBJECT__IMMUTABLE )
        {
            ERROR( "p2_namespace__add: namespace is write-protected" );
            return 0;
        }

        displaced_object = ( Object* ) dictionary__add(
            ns->children, ( char* ) array__peek( name ), o );
    }

    else
    {
        key = ( char* ) array__pop( name );

        child_ns_obj = ( Object* ) dictionary__lookup( ns->children, key );

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

        array__push( name, key );
    }

    return displaced_object;
}


Object *p2_namespace__add_simple
    ( p2_namespace *ns, const char *name, Object *o )
{
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

    return ( Object* ) dictionary__add
        ( ns->children, name, o );
}


Object *p2_namespace__lookup( p2_namespace_o *ns_obj, p2_name *name )
{
    p2_namespace *ns = ( p2_namespace* ) ns_obj->value;

    Object *o;
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
    #endif

    if ( !name || !array__size( name ) )
        return ns_obj;

    o = ( Object* ) dictionary__lookup( ns->children, array__peek( name ) );

    /* Look for the object in a nested namespace. */
    if ( array__size( name ) > 1 )
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
            key = ( char* ) array__pop( name );
            o = p2_namespace__lookup( o, name );
            array__push( name, key );
        }
    }

    return o;
}


Object *p2_namespace__lookup_simple(
    p2_namespace *ns, const char *name )
{
    #if DEBUG__SAFE
    if ( !ns | !name )
    {
        ERROR( "p2_namespace__lookup_simple: null argument" );
        return 0;
    }
    #endif

    return ( Object* ) dictionary__lookup( ns->children, name );
}


Object *p2_namespace__remove( p2_namespace_o *ns_obj, p2_name *name )
{
    p2_namespace *ns = ( p2_namespace* ) ns_obj->value;

    Object *child_ns_obj, *displaced_object;
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
    if ( !array__size( name ) )
    {
        ERROR( "empty name" );
        return 0;
    }
    #endif
printf( "--- ns rm 1---\n" ); fflush( stdout );

    if ( array__size( name ) == 1 )
    {
printf( "--- ns rm 2a---\n" ); fflush( stdout );
        if ( ns_obj->flags & OBJECT__IMMUTABLE )
        {
            ERROR( "p2_namespace__remove: namespace is write-protected" );
            return 0;
        }

        displaced_object = ( Object* ) dictionary__remove(
            ns->children, ( char* ) array__peek( name ) );
    }

    else
    {
printf( "--- ns rm 2b---\n" ); fflush( stdout );
        key = ( char* ) array__pop( name );

        child_ns_obj = ( Object* ) dictionary__lookup( ns->children, key );

        if ( child_ns_obj->type != ns_obj->type )
        {
            ERROR( "not a namespace" );
            displaced_object = 0;
        }

        else
            displaced_object = p2_namespace__remove( child_ns_obj, name );

        array__push( name, key );
    }
printf( "--- ns rm 3---\n" ); fflush( stdout );

    return displaced_object;
}


Object *p2_namespace__remove_simple( p2_namespace *ns, char *name )
{
    #if DEBUG__SAFE
    if ( !ns | !name)
    {
        ERROR( "p2_namespace__remove_simple: null argument" );
        return 0;
    }
    #endif

    return ( Object* ) dictionary__remove( ns->children, name );
}


/******************************************************************************/


void p2_namespace__distribute( p2_namespace *ns, p2_procedure *p )
{
    dictionary__distribute( ns->children, p );
}


/******************************************************************************/


static p2_action * lookup_and_print( char *key, p2_dictionary *dict )
{
    Object *o = ( Object* ) dictionary__lookup( dict, key );
    printf( "    %#x '%s' : %s\n", ( int ) o, key, o->type->name );
    return 0;
}


void p2_namespace__show_children( p2_namespace_o *ns_obj )
{
    int size = ( ( p2_namespace* ) ns_obj->value )->children->size;
    Array *a;
    p2_procedure p;

    printf( "%#x : namespace", ( int ) ns_obj );

    if ( size )
    {
        printf( "\n{\n" );

        /* Get alphabetized dictionary keys. */
        a = dictionary__keys(
            ( ( p2_namespace* ) ns_obj->value )->children );

        /* Print children. */
        p.execute = ( procedure ) lookup_and_print;
        p.state = ( ( p2_namespace* ) ns_obj->value )->children;
        array__distribute( a, &p );

        array__delete( a );

        printf( "}" );
    }

    else
        printf( " { }" );
}


/******************************************************************************/

/*
static boolean equals( void *p1, void *p2 )
{
    if ( p1 == p2 )
        return boolean__true;
    else
        return boolean__false;
}


    Object *match = ( Object* )
        collection__first_match( ns_obj, ( criterion ) equals );



static p2_action * ns__trace_bfs( Object *o, trace_proc_st *state )
{
    p2_action *action;

    if ( !o )
    {
        #if DEBUG__SAFE
        WARNING( "ns__trace_bfs: null object" );
        #endif

        return 0;
    }

    if ( !( action = p2_procedure__execute( ( state->inner_p ), o ) ) )
    {
        if ( o->type->flags & TYPE__IS_OBJ_COLL )
        {
            o->type->distribute( o->value, state->outer_p );
        }

        #if TRIPLES__GLOBAL__OUT_EDGES
        if ( o->outbound_edges )
        {
            p2_lookup_table__distribute( o->outbound_edges, state->edge_p );
        }
        #endif
    }

    return 0;
}
*/


p2_name *p2_namespace__find( p2_namespace_o *ns_obj, Object *o )
{
    p2_name *name = p2_name__new();

    /* ... */

    return name;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
