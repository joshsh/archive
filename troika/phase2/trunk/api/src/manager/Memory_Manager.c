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

#include <Memory_Manager.h>
#include <collection/Collection.h>
#include "../object/Object-impl.h"

#include <time.h>


#define visited( o )        (o)->flags & OBJECT__VISITED
#define set_visited( o )    (o)->flags |= OBJECT__VISITED
#define clear_visited( o )  (o)->flags &= ~OBJECT__VISITED

#define owned( o )          (o)->flags & OBJECT__OWNED
#define set_owned( o )      (o)->flags |= OBJECT__OWNED


struct Memory_Manager
{
    Bunch *objects;
        Object *objects_o;

    Object *root;

    /** Whether the manager contains only unmarked objects.  Any marked objects
        need to be unmarked prior to any algorithm which relies on object
        marking. */
    boolean clean;

    unsigned int size_at_last_cycle;
};


/******************************************************************************/


Memory_Manager *
memory_manager__new()
{
    Memory_Manager *m;
    Type *object_t;

    if ( !( m = NEW( Memory_Manager ) ) )
        return 0;

    m->objects = bunch__new( MEM__OBJECTS__BLOCK_SIZE );

    if ( !m->objects )
    {
        free( m );
        return 0;
    }

    object_t = object__create_type( "object" );

    m->objects_o = object__new(
        bunch__create_type( "bunch<object>", TYPE__IS_OBJ_COLL | TYPE__OWNS_DESCENDANTS ),
        m->objects, 0 );
    m->objects_o->type->type_arg = object_t;

    /* The manager starts out with no root object. */
    m->root = 0;

    m->clean = 1;
    m->size_at_last_cycle = 0;

    return m;
}


void
memory_manager__delete( Memory_Manager *m )
{
    Type *object_t, *object_bunch_t;

    if ( DEBUG__SAFE && !m )
        abort();

    object_bunch_t = m->objects_o->type;
    object_t = object_bunch_t->type_arg;

    object__delete( m->objects_o );
    /*bunch__for_all( m->objects, (void*(*)(void*)) object__delete );
    bunch__delete( m->objects );*/
    type__delete( object_bunch_t );
    type__delete( object_t );

    free( m );
}


unsigned int
memory_manager__size( Memory_Manager *m )
{
    if ( DEBUG__SAFE && !m )
        abort();

    return bunch__size( m->objects );
}


void
memory_manager__set_root( Memory_Manager *m, Object *o )
{
    if ( DEBUG__SAFE && ( !m || !o ) )
        abort();

    m->root = o;
}


Object *
memory_manager__object( Memory_Manager *m, Type *type, void *value, int flags )
{
    if ( DEBUG__SAFE && !m )
        abort();

    Object *o = object__new( type, value, flags );

    if ( o )
    {
        set_owned( o );

        if ( !bunch__add( m->objects, o ) )
        {
            object__delete( o );
            o = 0;
        }
    }

    return o;
}


/* Unmarking / cleanup ********************************************************/


static void
unmark_all( Memory_Manager *m )
{
    void unmark( Object *o )
    {
        clear_visited( o );
    }

    if ( DEBUG__SAFE && !m )
        abort();

    collection__do_for_all( m->objects_o, ( Void_f ) unmark );
    /*bunch__for_all( m->objects, (void*(*)(void*)) unmark );*/
    m->clean = 1;
}


/* Unmarks all marked objects in the environment, and deletes the rest.*/
static void
sweep( Memory_Manager *m )
{
    boolean unmarked( Object *o )
    {
        if ( DEBUG__SAFE && !o )
            abort();

        /* If marked, unmark. */
        if ( visited( o ) )
        {
            clear_visited( o );

            /* Don't exclude this object. */
            return FALSE;
        }

        /* If unmarked, delete. */
        else
        {
            object__delete( o );

            /* Exclude this object. */
            return TRUE;
        }
    }

    if ( DEBUG__SAFE && !m )
        abort();

    collection__exclude_if( m->objects_o, ( Criterion ) unmarked );
    m->clean = 1;
}


/* Tracing / graph traversal **************************************************/


/* Unmark visited objects. */
static void
unwalk( Object *root, boolean follow_triples )
{
    ACTION helper( Object **opp )
    {
        Object *o = *opp;

        if ( visited( o ) )
        {
            clear_visited( o );
            return CONTINUE;
        }

        else
            return BREAK;
    }

    object__trace( root, ( Dist_f ) helper, follow_triples );
}


void
memory_manager__walk
    ( Memory_Manager *m, Object *root, Dist_f f, boolean use_bfs, boolean follow_triples )
{
    boolean dosweep;

    ACTION helper( Object **opp )
    {
        Object *o;

        if ( DEBUG__SAFE && !opp )
            abort();

        /* If the entire object graph is to be traversed, then it is safe to
           mutate references to indirection nodes. */
        o = ( dosweep )
            ? DEREF( opp )
            : *opp;

        /* Don't try to walk through a null pointer. */
        if ( FIRST_CLASS_NULL && !o )
            return BREAK;

        /* If the object is already marked, abort. */
        if ( visited( o ) )
            return BREAK;

        else
        {
            /* Mark the object. */
            set_visited( o );

            /* Execute the procedure. */
            return f( ( void** ) opp );
        }
    }

    if ( DEBUG__SAFE && ( !m || !f ) )
        abort();

    if ( !root )
    {
        if ( m->root )
            root = m->root;
        else
        {
            ERROR( "can't walk from null root" );
            return;
        }
    }

    dosweep = ( root == m->root );

    if ( !m->clean )
        unmark_all( m );

    m->clean = FALSE;

    if ( use_bfs )
        object__trace_bfs( root, ( Dist_f ) helper, follow_triples );
    else
        object__trace( root, ( Dist_f ) helper, follow_triples );

    /* Might as well sweep. */
    if ( dosweep )
        sweep( m );
    else
        unwalk( root, follow_triples );

    m->clean = TRUE;
}


void
memory_manager__trace
    ( Memory_Manager *m, Object *root, Walker walk, Dist_f dist )
{
    int marked;

    ACTION trace( Object **opp )
    {
        Object *o = DEREF( opp );

        /* If the object is already marked, skip it. */
        if ( visited( o ) )
            return BREAK;

        else
        {
            if ( DEBUG__SAFE )
                marked++;

            /* Mark the object. */
            set_visited( o );

            /* Execute the closure. */
            return dist( ( void** ) opp );
        }
    }

    ACTION untrace( Object **opp )
    {
        Object *o = DEREF( opp );

        if ( visited( o ) )
        {
            if ( DEBUG__SAFE )
                marked--;

            clear_visited( o );

            return CONTINUE;
        }

        else
            return BREAK;
    }

    if ( DEBUG__SAFE && ( !m || !walk || !dist ) )
        abort();

    if ( DEBUG__SAFE )
        marked = 0;

    if ( !root )
    {
        if ( m->root )
            root = m->root;
        else
        {
            ERROR( "can't trace from null root" );
            return;
        }
    }

    /* Make sure no objects are marked to begin with. */
    if ( !m->clean )
        unmark_all( m );

    m->clean = FALSE;
    walk( root, ( Dist_f ) trace );
    walk( root, ( Dist_f ) untrace );
    m->clean = TRUE;

    if ( DEBUG__SAFE )
    {
        if ( marked > 0 )
        {
            WARNING( "memory_manager__trace: unreachable object(s)" );
            /*WARNING( "memory_manager__trace: %i marked objects are still marked as visited", marked );*/
        }

        else if ( marked < 0 )
        {
            WARNING( "memory_manager__trace: more objects unmarked than actually visited" );
            /*WARNING( "memory_manager__trace: %i more objects unmarked than actually visited", -marked );*/
        }
    }
}


/******************************************************************************/


Set *
memory_manager__get_multirefs( Memory_Manager *m, Object *root )
{
    Set *s;

    ACTION helper( Object **opp )
    {
        Object *o;

        if ( DEBUG__SAFE && !opp )
            abort();

        o = *opp;

        if ( FIRST_CLASS_NULL && !o )
            return BREAK;

        /* If the object is already marked, add it and abort. */
        if ( visited( o ) )
        {
            set__add( s, o );
            return BREAK;
        }

        else
        {
            /* Mark the object. */
            set_visited( o );

            /* Object references its triples, which in turn reference the object. */
            if ( ENCODING__TRIPLES_AS_OBJECTS && TRIPLES__GLOBAL__OUT_EDGES
              && o->outbound_edges && hash_table__size( o->outbound_edges ) )
            {
                set__add( s, o );
            }

            return CONTINUE;
        }
    }

    if ( DEBUG__SAFE && !m )
        abort();

    if ( !m->clean )
        unmark_all( m );

    m->clean = 0;

    s = set__new();

    object__trace( root, ( Dist_f ) helper, TRUE );

    return s;
}


/* Mark-and-sweep garbage collection ******************************************/


void
memory_manager__collect( Memory_Manager *m, boolean force, boolean echo )
{
    unsigned int size;

    clock_t t;
    double elapsed_time;

    ACTION noop( Object **opp )
    {
        /* Avoid "unused parameter" warning. */
        opp = 0;

        return CONTINUE;
    }

    if ( DEBUG__SAFE && !m )
        abort();

#if DEBUG__MEMORY
    echo = TRUE;
#endif

    if ( echo )
        t = clock();

    if ( echo || !force )
        size = memory_manager__size( m );

    if ( !force )
    {
        if ( m->size_at_last_cycle
          && ( double ) size / ( double ) m->size_at_last_cycle <= MEM__COLLECTION_THRESHOLD )
            return;
    }

    /* Mark all reachable objects. */
    memory_manager__walk( m, 0, ( Dist_f ) noop, FALSE, TRUE );

    m->size_at_last_cycle = memory_manager__size( m );

    if ( echo )
    {
        elapsed_time = difftime( clock(), t );

        PRINT( "Collected %i of %i objects (%.3g%%)",
            size - m->size_at_last_cycle,
            size,
            ( ( size - m->size_at_last_cycle ) * 100 ) / ( double ) size );
/*
        PRINT( " in %fms", elapsed_time * 1000 );
*/
        PRINT( ".\n" );
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
