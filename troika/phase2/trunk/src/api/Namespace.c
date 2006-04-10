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





/******************************************************************************/


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
    printf( "[] namespace__delete(%#x)\n", ( int ) ns ); fflush( stdout );
    #endif

    dictionary__delete( ns->children );

    free( ns );
}


unsigned int
namespace__size( Namespace *ns )
{
    return hash_table__size( ns->children );
}


Object *
namespace__add( Namespace_o *ns_obj, Name *name, Object *o )
{
    Namespace *ns = ns_obj->value;

    Object *child_ns_obj;
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
        if ( object__immutable( ns_obj ) )
        {
            ERROR( "namespace__add: namespace is write-protected" );
            o = 0;
        }

        else
            dictionary__add( ns->children, array__peek( name ), o );
    }

    else
    {
        key = array__pop( name );

        child_ns_obj = dictionary__lookup( ns->children, key );

        if ( child_ns_obj->type != ns_obj->type )
        {
            ERROR( "namespace__add: not a namespace" );
            o = 0;
        }

        else
            o = namespace__add( child_ns_obj, name, o );

        array__push( name, key );
    }

    return o;
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

    return dictionary__add( ns->children, name, o );
}


Object *
namespace__lookup( Namespace_o *ns_obj, Name *name )
{
    Namespace *ns;
    Object *o;
    char *key;

    #if DEBUG__SAFE
    if ( !ns_obj )
    {
        ERROR( "namespace__lookup: null argument" );
        return 0;
    }
    #endif

    ns = ( Namespace* ) ns_obj->value;

    #if DEBUG__NAMESPACE
    printf( "[...] namespace__lookup(%#x, %#x)\n", ( int ) ns_obj, ( int ) name );
    #endif

    if ( !name || !array__size( name ) )
        return ns_obj;

    o = ( Object* ) dictionary__lookup( ns->children, array__peek( name ) );

    /* Look for the object in a nested namespace. */
    if ( o && array__size( name ) > 1 )
    {
        /* Always check for this error, as namespace references may come directly
           from the user. */
        if ( o->type != ns_obj->type )
        {
            ERROR( "namespace__lookup: not a namespace" );
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
    Namespace *ns;

    Object *child_ns_obj, *o;
    char *key;

    #if DEBUG__SAFE
    if ( !ns_obj || !name )
    {
        ERROR( "namespace__remove: null argument" );
        return 0;
    }
    else if ( !array__size( name ) )
    {
        ERROR( "empty name" );
        return 0;
    }
    #endif

    ns = ns_obj->value;

    if ( array__size( name ) == 1 )
    {
        if ( object__immutable( ns_obj ) )
        {
            ERROR( "namespace__remove: namespace is write-protected" );
            return 0;
        }

        o = dictionary__remove( ns->children, ( char* ) array__peek( name ) );
    }

    else
    {
        key = ( char* ) array__pop( name );

        child_ns_obj = dictionary__lookup( ns->children, key );

        if ( !child_ns_obj )
        {
            ERROR( "namespace__remove: namespace does not exist" );
            o = 0;
        }

        else if ( child_ns_obj->type != ns_obj->type )
        {
            ERROR( "namespace__remove: not a namespace" );
            o = 0;
        }

        else
            o = namespace__remove( child_ns_obj, name );

        array__push( name, key );
    }

    return o;
}


Object *
namespace__remove_simple( Namespace *ns, char *name )
{
    #if DEBUG__SAFE
    if ( !ns || !name)
    {
        ERROR( "namespace__remove_simple: null argument" );
        return 0;
    }
    #endif

    return dictionary__remove( ns->children, name );
}


/******************************************************************************/


void
namespace__walk( Namespace *ns, Dist_f f )
{
    dictionary__walk( ns->children, f );
}


/******************************************************************************/


void
namespace__show_children( const Namespace_o *ns_obj )
{
    Dictionary *dict = ( ( Namespace* ) ns_obj->value )->children;
    int size = hash_table__size( dict );
    Array *keys;
    int maxlen = 0;

    void *find_maxlen( Object **opp )
    {
        int len = strlen( ( *opp )->type->name );
        if ( len > maxlen )
            maxlen = len;
        return 0;
    }

    void *print( char **key )
    {
        Object *o = ( Object* ) dictionary__lookup( dict, *key );
        int i, lim = maxlen - strlen( o->type->name );

        printf( "    " );

        #if COMPILER__SHOW_ADDRESS
        printf( "%#x ", ( int ) o );
        #endif

        printf( "<%s> ", o->type->name );

        for ( i = 0; i < lim; i++ )
            printf( " " );

        printf( "%s\n", *key );

        return 0;
    }

    #if COMPILER__SHOW_ADDRESS
    printf( "%#x ", ( int ) ns_obj );
    #endif

    printf( "<%s>", ns_obj->type->name );

    if ( size )
    {
        printf( "\n{\n" );

        /* Get alphabetized dictionary keys. */
        keys = dictionary__keys(
            ( ( Namespace* ) ns_obj->value )->children );

        namespace__walk( ns_obj->value, ( Dist_f ) find_maxlen );

        /* Print children. */
        array__walk( keys, ( Dist_f ) print );
        array__delete( keys );

        printf( "}\n" );
    }

    else
        printf( " { }\n" );
}


/******************************************************************************/


Type *
namespace__create_type( const char *name, int flags )
{
    Type *t = type__new( name, flags );

    if ( t )
    {
        t->destroy = ( Destructor ) namespace__delete;
        t->size = ( Size_Of ) namespace__size;
        t->walk = ( Walker ) namespace__walk;
    }

    return t;
}


/******************************************************************************/


/* Distribute f recursively (and in a breadth-first fashion) through any child namespaces. */
static void
ns_walk_bfs( Namespace_o *ns_o, Dist_f f )
{
    Array *queue = array__new( 0, 0 );

    #if DEBUG__NAMESPACE
    printf( "[] ns_walk_bfs(%#x, %#x)\n", ( int ) ns_o, ( int ) f );
    #endif

    void *distribute( Object **opp )
    {
        Object *o = *opp;

        /* If we've found a namespace... */
        if ( o->type == ns_o->type )
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
            ( ( Object* ) array__pop( queue ) )->value,
            ( Dist_f ) distribute );
    }
}


/*
typedef struct Name_Fragment
{
    Name *head;
    Namespace *tail;

} Name_Fragment;
*/


Name *
namespace__find( const Namespace_o *haystack, const Object *needle )
{
#if COMMENT
    Hash_Map *parents = hash_map__new();
#endif
    Name *name = name__new();

    

    /* ... */

    return name;
}


Object *
namespace__resolve_simple( Namespace_o *ns_obj, char *key, Memory_Manager *m )
{
    Object *o = 0;

    void *test( Namespace_o **ns_opp )
    {
        if ( ( o = namespace__lookup_simple( ( *ns_opp )->value, key ) ) )
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

    #if DEBUG__SAFE
    if ( !nso || !name )
    {
        ERROR( "namespace__resolve: null argument" );
        return 0;
    }

    else
    #endif

    for ( i = 0; i < array__size( name ); i++ )
    {
        if ( o->type != nso->type )
        {
            o = 0;
            break;
        }

        key = array__get( name, i );

        if ( !( o = namespace__resolve_simple( o, key, m ) ) )
            break;
    }

    #if DEBUG__NAMESPACE
    printf( "[%#x] namespace__resolve(%#x, ", ( int ) o, ( int ) nso );
    name__print( name );
    printf( ")\n" );
    #endif

    return o;
}


Object *
namespace__undefine( Namespace_o *nso, Name *name, Memory_Manager *m )
{
    Object *o = nso;
    unsigned int i;
    Namespace *parent;
    char *namekey = "";

    Object *
    resolve( Namespace_o *ns_obj, char *key, Memory_Manager *m )
    {
        Object *o = 0;

        void *test( Namespace_o **ns_opp )
        {
            parent = ( *ns_opp )->value;
            if ( ( o = namespace__lookup_simple( parent, key ) ) )
                return walker__break;
            else
                return 0;
        }

        memory_manager__trace( m, ns_obj, ( Walker ) ns_walk_bfs, ( Dist_f ) test );

        return o;
    }

    #if DEBUG__SAFE
    if ( !nso || !name )
    {
        ERROR( "namespace__undefine: null argument" );
        return 0;
    }

    else
    #endif

    for ( i = 0; i < array__size( name ); i++ )
    {
        if ( o->type != nso->type )
        {
            o = 0;
            break;
        }

        namekey = array__get( name, i );

        if ( !( o = resolve( o, namekey, m ) ) )
            break;
    }

    if ( o )
        namespace__remove_simple( parent, namekey );

    return o;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
