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

#include "Namespace-impl.h"
#include <collection/Hash_Map.h>


/* Distribute f recursively (and in a breadth-first fashion) to any child
   namespaces. */
static void
ns_walk_bfs( OBJ( NAMESPACE ) *ns_o, Visitor f )
{
    Array *queue;
    Type *t;

    /* Distribute f over the children of the namespace which are themselves
       namespaces. */
    ACTION helper( Object **opp )
    {
        Object *o = DEREF( opp );

        /* If we've found a namespace... */
        if ( object__type( o ) == t )
        {
            /* Apply the target function, and quit the traversal if so instructed. */
            if ( f( ( void** ) opp ) == BREAK )
            {
                array__clear( queue );
                return BREAK;
            }

            /* Extend the traversal to this namespace's children. */
            else
            {
                array__enqueue( queue, o );
                return CONTINUE;
            }
        }

        /* Pass over objects which are not namespaces. */
        else
            return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !ns_o || !f ) )
        abort();

    t = object__type( ns_o );

    queue = array__new( 0, 0 );

    helper( &ns_o );
    /*array__push( queue, ns_o );*/

    /* Breaks out when queue is empty (search exhausted or aborted). */
    while ( array__size( queue ) )
    {
        namespace__walk(
            object__value( array__pop( queue ) ),
            ( Visitor ) helper );
    }

    array__delete( queue );
}


#ifdef NOT_FINISHED
struct edge
{
    char *name;
    OBJ( NAMESPACE ) *parent;
};


void
namespace__map_back( OBJ( NAMESPACE ) *haystack,
                     Set *needles,
                     Hash_Map *map,
                     Manager *m )
{
    if ( DEBUG__SAFE && ( !haystack || !needles || !map || !m ) )
        abort();

    /* Parent hierarchy. */
    Hash_Map *edges = hash_map__new();

    Type *t = object__type( haystack );
    Name *name;

    Name *get_name( struct edge *e )
    {
        name = name__new();

        while ( e )
        {
            array__push( name, STRDUP( e->name ) );
            e = ( e->parent )
                ? hash_map__lookup( edges, ;
        }

        do
        {
            key = dictionary__reverse_lookup( ( ( Namespace* ) object__value( parent ) )->children, o );
            array__push( name, key );
            o = parent;

        } while ( ( parent = hash_map__lookup( parents, o ) ) );
    }

    ACTION find( OBJ( NAMESPACE ) **nsopp )
    {
        ACTION helper( char *name, Object **opp )
        {
            Object *o = DEREF( opp );

            if ( set__contains( needles, o ) )
            {
                hash_map__add( map, 

                set__remove( needles, o );

                if ( !set__size( needles ) )
                    return BREAK;
            }

            return CONTINUE;
        }

        dictionary__walk_special(
            ( ( Namespace* ) object__value( *nsopp ) )->children,
            ( Dictionary_Visitor ) helper );
    }

    /* So that the original set is not affected by the algorithm. */
    needles = set__copy( needles );


    set__delete( needles );
}
#endif


Name *
namespace__find( OBJ( NAMESPACE ) *haystack, const Object *needle, Manager *m )
{
    Hash_Map *parents = hash_map__new();
    Name *name = name__new();
    Object *o, *parent = 0, *needle2;
    char *key;
    Type *t = object__type( haystack );

    ACTION find( OBJ( NAMESPACE ) **nsopp )
    {
        ACTION helper( Object **opp )
        {
            Object *o = DEREF( opp );

            if ( o == needle )
            {
                parent = *nsopp;
                needle2 = o;
                return BREAK;
            }

            /* Only add the object to the "parents" tree if it is a namespace,
               and is not already in the tree. */
            else if ( object__type( o ) == t && !hash_map__lookup( parents, o ) )
                hash_map__add( parents, o, *nsopp );

            return CONTINUE;
        }

        namespace__walk( object__value( *nsopp ), ( Visitor ) helper );

        if ( parent )
            return BREAK;

        else
            return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !haystack || !m ) )
        abort();

    /* The null element always receives special treatment (when it is allowed
       at all), so don't cover it up with a name. */
    if ( !needle )
    {
        if ( FIRST_CLASS_NULL )
        {
            name = 0;
            goto finish;
        }

        else if ( DEBUG__SAFE )
            abort();
    }

    /* Trace until 'needle' is found or all namespaces have been visited. */
    memory_manager__trace( m, haystack, ( Walker ) ns_walk_bfs, ( Visitor ) find );

    if ( parent )
    {
        name = name__new();
        o = needle2;

        do
        {
            key = dictionary__reverse_lookup( ( ( Namespace* ) object__value( parent ) )->children, o );
            array__push( name, key );
            o = parent;

        } while ( ( parent = hash_map__lookup( parents, o ) ) );
    }

    else
        name = 0;

finish:

    hash_map__delete( parents );
    return name;
}


Object *
namespace__resolve_simple( OBJ( NAMESPACE ) *ns_obj, char *key, Manager *m )
{
    Object *o = 0;

    ACTION test( OBJ( NAMESPACE ) **ns_opp )
    {
        o = namespace__lookup_simple( object__value( *ns_opp ), key );

        return o ? BREAK : CONTINUE;
    }

    if ( DEBUG__SAFE && ( !ns_obj || !key || !m ) )
        abort();

    memory_manager__trace( m, ns_obj, ( Walker ) ns_walk_bfs, ( Visitor ) test );

    return o;
}


Object *
namespace__resolve( OBJ( NAMESPACE ) *nso, Name *name, Manager *m )
{
    Object *o = nso;
    char *key;
    unsigned int i;

    if ( DEBUG__SAFE && ( !nso || !name || !m ) )
        abort();

    for ( i = 0; i < array__size( name ); i++ )
    {
        if ( object__type( o ) != object__type( nso ) )
        {
            o = 0;
            break;
        }

        key = array__get( name, i );

        if ( !( o = namespace__resolve_simple( o, key, m ) ) )
            break;
    }

    return o;
}


Object *
namespace__undefine( OBJ( NAMESPACE ) *nso, Name *name, Manager *m )
{
    Object *o = nso;
    unsigned int i;
    Namespace *parent;
    char *key = "";
    Type *t = object__type( nso );

    Object *
    resolve( OBJ( NAMESPACE ) *ns_obj, char *key, Manager *m )
    {
        Object *o = 0;

        ACTION test( OBJ( NAMESPACE ) **nsopp )
        {
            parent = object__value( *nsopp );
            o = namespace__lookup_simple( parent, key );

            return o ? BREAK : CONTINUE;
        }

        memory_manager__trace( m, ns_obj, ( Walker ) ns_walk_bfs, ( Visitor ) test );

        return o;
    }

    if ( DEBUG__SAFE && ( !nso || !name || !m || !array__size( name ) ) )
        abort();

    for ( i = 0; i < array__size( name ); i++ )
    {
        if ( object__type( o ) != t )
        {
            o = 0;
            break;
        }

        key = array__get( name, i );

        if ( !( o = resolve( o, key, m ) ) )
            break;
    }

    if ( o )
        namespace__remove_simple( parent, key );

    return o;
}


/******************************************************************************/


#define ERRBUFSIZ  1000


static void
error__not_defined( Name *name )
{
    char buff[ERRBUFSIZ];

    if ( DEBUG__SAFE && !name )
        abort();

    name__encode( name, buff );
    ERROR( "\"%s\" is not defined in this namespace", buff );
}

static void
error__not_a_namespace( Name *name )
{
    char buff[ERRBUFSIZ];

    if ( DEBUG__SAFE && !name )
        abort();

    name__encode( name, buff );
    ERROR( "\"%s\" is not a namespace", buff );
}


/*
Object *
namespace__copy( OBJ( NAMESPACE ) *nso, Name *src, Name *dest )
{

}
*/


/******************************************************************************/


Object *
namespace__define( OBJ( NAMESPACE ) *nso, Name *name, Object *o, Manager *m )
{
    void *key;
    OBJ( NAMESPACE ) *local;

    /* Check that arguments are sound. */
    if ( DEBUG__SAFE && ( !nso || !name || !o || !m || !array__size( name ) ) )
        abort();

    if ( COMPILER__NAME_INHERITANCE )
    {
        key = array__dequeue( name );
        local = namespace__resolve( nso, name, m );

        if ( !local )
        {
            error__not_defined( name );
            array__enqueue( name, key );
            o = 0;
        }

        else if ( object__type( local ) != object__type( nso ) )
        {
            error__not_a_namespace( name );
            array__enqueue( name, key );
            o = 0;
        }

        else
        {
            nso = local;

            if ( o )
            {
                o = namespace__add_simple( object__value( nso ), key, o );
                array__enqueue( name, key );
            }

            else
            {
                array__enqueue( name, key );
                o = namespace__undefine( nso, name, m );
            }
        }
    }

    else
        o = ( o )
            ? namespace__add( nso, name, o )
            : namespace__remove( nso, name );

    return o;
}

/*
Object *
namespace__move( OBJ( NAMESPACE ) *nso, Name *src, Name *dest )
{

}
*/


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
