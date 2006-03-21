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

#include "Namespace.h"


Namespace *
namespace__new()
{
    Namespace *ns;

    if ( !( ns = new( Namespace ) ) )
    {
        ERROR( "namespace__new: allocation failed" );
        return 0;
    }

    if ( !( ns->children = dictionary__new() ) )
    {
        free( ns );
        return 0;
    }

    #if DEBUG__NAMESPACE
    printf( "[%#x] namespace__new\n", ( int ) ns );
    #endif

    return ns;
}


void
namespace__delete( Namespace *ns )
{
    #if DEBUG__SAFE
        if ( !ns )
        {
            ERROR( "namespace__delete: null namespace" );
            return;
        }
    #endif

    #if DEBUG__NAMESPACE
    printf( "namespace__delete(%#x)\n", ( int ) ns ); fflush( stdout );
    #endif

    dictionary__delete( ns->children );

    free( ns );
}


Object *
namespace__add( Namespace_o *ns_obj, Name *name, Object *o )
{
    Namespace *ns = ( Namespace* ) ns_obj->value;

    Object *child_ns_obj, *displaced_object;
    char *key;

    #if DEBUG__SAFE
    if ( !ns )
    {
        ERROR( "namespace__add: null namespace" );
        return 0;
    }
    if ( !name )
    {
        ERROR( "namespace__add: null name" );
        return 0;
    }
    if ( !array__size( name ) )
    {
        ERROR( "namespace__add: empty name" );
        return 0;
    }
    #endif

    if ( array__size( name ) == 1 )
    {
        if ( ns_obj->flags & OBJECT__IMMUTABLE )
        {
            ERROR( "namespace__add: namespace is write-protected" );
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
            ERROR( "namespace__add: not a namespace" );
            displaced_object = 0;
        }

        else
        {
            displaced_object = namespace__add(
                child_ns_obj, name, o );
        }

        array__push( name, key );
    }

    return displaced_object;
}


Object *
namespace__add_simple( Namespace *ns, const char *name, Object *o )
{
    #if DEBUG__SAFE
    if ( !ns )
    {
        ERROR( "namespace__add_simple: null namespace" );
        return 0;
    }
    if ( !name )
    {
        ERROR( "namespace__add_simple: null name" );
        return 0;
    }
    #endif

    return ( Object* ) dictionary__add
        ( ns->children, name, o );
}


Object *
namespace__lookup( Namespace_o *ns_obj, Name *name )
{
    Namespace *ns = ( Namespace* ) ns_obj->value;

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
    printf( "[...] namespace__lookup(%#x, %#x)\n", ( int ) ns, ( int ) name );
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
            o = namespace__lookup( o, name );
            array__push( name, key );
        }
    }

    return o;
}


Object *
namespace__lookup_simple( Namespace *ns, const char *name )
{
    #if DEBUG__SAFE
    if ( !ns | !name )
    {
        ERROR( "namespace__lookup_simple: null argument" );
        return 0;
    }
    #endif

    return ( Object* ) dictionary__lookup( ns->children, name );
}


Object *
namespace__remove( Namespace_o *ns_obj, Name *name )
{
    Namespace *ns = ( Namespace* ) ns_obj->value;

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
            ERROR( "namespace__remove: namespace is write-protected" );
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
            displaced_object = namespace__remove( child_ns_obj, name );

        array__push( name, key );
    }
printf( "--- ns rm 3---\n" ); fflush( stdout );

    return displaced_object;
}


Object *
namespace__remove_simple( Namespace *ns, char *name )
{
    #if DEBUG__SAFE
    if ( !ns | !name)
    {
        ERROR( "namespace__remove_simple: null argument" );
        return 0;
    }
    #endif

    return ( Object* ) dictionary__remove( ns->children, name );
}


/******************************************************************************/


void
namespace__distribute( Namespace *ns, p2_procedure *p )
{
    dictionary__distribute( ns->children, p );
}


/******************************************************************************/


static p2_action *
lookup_and_print( char *key, Dictionary *dict )
{
    Object *o = ( Object* ) dictionary__lookup( dict, key );
    printf( "    %#x '%s' : %s\n", ( int ) o, key, o->type->name );
    return 0;
}


void
namespace__show_children( Namespace_o *ns_obj )
{
    int size = ( ( Namespace* ) ns_obj->value )->children->size;
    Array *a;
    p2_procedure p;

    printf( "%#x : namespace", ( int ) ns_obj );

    if ( size )
    {
        printf( "\n{\n" );

        /* Get alphabetized dictionary keys. */
        a = dictionary__keys(
            ( ( Namespace* ) ns_obj->value )->children );

        /* Print children. */
        p.execute = ( procedure ) lookup_and_print;
        p.state = ( ( Namespace* ) ns_obj->value )->children;
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
            lookup_table__distribute( o->outbound_edges, state->edge_p );
        }
        #endif
    }

    return 0;
}
*/


Name *
namespace__find( Namespace_o *ns_obj, Object *o )
{
    Name *name = name__new();

    /* ... */

    return name;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
