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

#include "Namespace-impl.h"
#include <util/Hash_Map.h>


/* Distribute f recursively (and in a breadth-first fashion) through any child namespaces. */
static void
ns_walk_bfs( Namespace_o *ns_o, Dist_f f )
{
    Array *queue = array__new( 0, 0 );

    void *distribute( Object **opp )
    {
        Object *o = *opp;

        /* If we've found a namespace... */
        if ( object__type( o ) == object__type( ns_o ) )
        {
            /* Apply the target function, and quit the traversal if so instructed. */
            if ( f( ( void** ) opp ) == walker__break )
            {
                array__delete( queue );
                queue = 0;
                return walker__break;
            }

            /* Extend the traversal to this namespace's children. */
            else
            {
                array__enqueue( queue, o );
                return 0;
            }
        }

        else
            return 0;
    }

    distribute( &ns_o );

    /* Breaks out when queue is empty (search exhausted) or has been
       destroyed (search aborted). */
    while ( queue && array__size( queue ) )
    {
        namespace__walk(
            object__value( array__pop( queue ) ),
            ( Dist_f ) distribute );
    }
}


Name *
namespace__find( Namespace_o *haystack, Object *needle, Memory_Manager *m )
{
    Hash_Map *parents = hash_map__new();
    Name *name = name__new();
    Object *o, *parent = 0;
    char *key;
    Type *t = object__type( haystack );

    void *find( Namespace_o **nsopp )
    {
        void *helper( Object **opp )
        {
            if ( *opp == needle )
            {
                parent = *nsopp;
                return walker__break;
            }

            /* Only add the object to the "parents" tree if it is a namespace,
               and is not already in the tree. */
            else if ( object__type( *opp ) == t && !hash_map__lookup( parents, *opp ) )
                hash_map__add( parents, *opp, *nsopp );

            return 0;
        }

        namespace__walk( object__value( *nsopp ), ( Dist_f ) helper );

        if ( parent )
            return walker__break;

        else
            return 0;
    }

    /* Trace until 'needle' is found or all namespaces have been visited. */
    memory_manager__trace( m, haystack, ( Walker ) ns_walk_bfs, ( Dist_f ) find );

    if ( parent )
    {
        name = name__new();
        o = needle;

        do
        {
            key = dictionary__reverse_lookup( ( ( Namespace* ) object__value( parent ) )->children, o );
            array__push( name, key );
            o = parent;

        } while ( ( parent = hash_map__lookup( parents, o ) ) );
    }

    else
        name = 0;

    hash_map__delete( parents );
    return name;
}


Object *
namespace__resolve_simple( Namespace_o *ns_obj, char *key, Memory_Manager *m )
{
    Object *o = 0;

    void *test( Namespace_o **ns_opp )
    {
        if ( ( o = namespace__lookup_simple( object__value( *ns_opp ), key ) ) )
            return walker__break;
        else
            return 0;
    }

    memory_manager__trace( m, ns_obj, ( Walker ) ns_walk_bfs, ( Dist_f ) test );

    return o;
}


Object *
namespace__resolve( Namespace_o *nso, Name *name, Memory_Manager *m )
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
namespace__undefine( Namespace_o *nso, Name *name, Memory_Manager *m )
{
    Object *o = nso;
    unsigned int i;
    Namespace *parent;
    char *key = "";
    Type *t = object__type( nso );

    Object *
    resolve( Namespace_o *ns_obj, char *key, Memory_Manager *m )
    {
        Object *o = 0;

        void *test( Namespace_o **nsopp )
        {
            parent = object__value( *nsopp );
            o = namespace__lookup_simple( parent, key );

            if ( o )
                return walker__break;
            else
                return 0;
        }

        memory_manager__trace( m, ns_obj, ( Walker ) ns_walk_bfs, ( Dist_f ) test );

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


static void
error__not_a_namespace( Name *name )
{
    printf( "Error: \"" );
    name__print( name );
    printf( "\" is not a namespace\n" );
}


static void
error__not_defined( Name *name )
{
    printf( "Error: \"" );
    name__print( name );
    printf( "\" is not defined in this namespace\n" );
}


/*
Object *
namespace__copy( Namespace_o *nso, Name *src, Name *dest )
{

}
*/


Object *
namespace__define( Namespace_o *nso, Name *name, Object *o, Memory_Manager *m )
{
    void *key;
    Namespace_o *local;

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
namespace__move( Namespace_o *nso, Name *src, Name *dest )
{

}
*/


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
