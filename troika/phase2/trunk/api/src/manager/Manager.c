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

#include <phase2/Manager.h>
#include <phase2/Collection.h>
#include "../object/Object-impl.h"

#include <time.h>


#define visited( o )        (o)->flags & OBJECT__VISITED
#define set_visited( o )    (o)->flags |= OBJECT__VISITED
#define clear_visited( o )  (o)->flags &= ~OBJECT__VISITED

#define owned( o )          (o)->flags & OBJECT__OWNED
#define set_owned( o )      (o)->flags |= OBJECT__OWNED


struct Manager
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


Manager *
manager__new()
{
    Manager *m;
    Type *object_t = 0, *bunch_t = 0;

    if ( !( m = NEW( Manager ) ) )
        return 0;

    m->objects_o = 0;
    m->objects = bunch__new( MEM__OBJECTS__BLOCK_SIZE );

    if ( !m->objects )
        goto fail;

    /* NOTE: these are freed in manager__free */
    object_t = object__create_type( "object" );
    bunch_t = bunch__create_type( "bunch<object>", TYPE__IS_OBJ_COLL | TYPE__OWNS_DESCENDANTS );

    if ( !object_t || !bunch_t )
        goto fail;

    m->objects_o = object__new( bunch_t, m->objects, 0 );
    m->objects_o->type->type_arg = object_t;

    /* The manager starts out with no root object. */
    m->root = 0;

    m->clean = 1;
    m->size_at_last_cycle = 0;

    return m;

fail:

    if ( object_t )
        type__free( object_t );
    if ( bunch_t )
        type__free( bunch_t );

    if ( m->objects_o )
        object__free( m->objects_o );
    else
        bunch__free( m->objects );

    free( m );

    return 0;
}


void
manager__free( Manager *m )
{
    Type *object_t, *object_bunch_t;

    if ( DEBUG__SAFE && !m )
        ABORT;

    object_bunch_t = m->objects_o->type;
    object_t = object_bunch_t->type_arg;

    object__free( m->objects_o );
    /*bunch__for_all( m->objects, (void*(*)(void*)) object__free );
    bunch__free( m->objects );*/
    type__free( object_bunch_t );
    type__free( object_t );

    free( m );
}


unsigned int
manager__size( Manager *m )
{
    if ( DEBUG__SAFE && !m )
        ABORT;

    return bunch__size( m->objects );
}


void
manager__set_root( Manager *m, Object *o )
{
    if ( DEBUG__SAFE && ( !m || !o ) )
        ABORT;

    m->root = o;
}


Object *
manager__object( Manager *m, Type *type, void *value, int flags )
{
    Object *o;

    if ( DEBUG__SAFE && !m )
        ABORT;

    o = object__new( type, value, flags );

    if ( o )
    {
        set_owned( o );

        if ( !bunch__add( m->objects, o ) )
        {
            object__free( o );
            o = 0;
            WARNING__ALLOC( "failed to add new object" );
        }
    }

    return o;
}


/* Unmarking / cleanup ********************************************************/


static void
unmark_all( Manager *m )
{
    ACTION unmark( Object **opp )
    {
        clear_visited( *opp );

        return CONTINUE;
    }

    if ( DEBUG__SAFE && !m )
        ABORT;

    bunch__walk( m->objects_o->value, ( Visitor ) unmark );

    m->clean = 1;
}


/* Unmarks all marked objects in the environment, and deletes the rest.*/
static void
sweep( Manager *m )
{
    boolean unmarked( Object *o )
    {
        if ( DEBUG__SAFE && !o )
            ABORT;

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
            object__free( o );

            /* Exclude this object. */
            return TRUE;
        }
    }

    if ( DEBUG__SAFE && !m )
        ABORT;

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

    object__trace( root, ( Visitor ) helper, follow_triples );
}


void
manager__walk
    ( Manager *m, Object *root, Visitor f, boolean use_bfs, boolean follow_triples )
{
    boolean dosweep;

    ACTION helper( Object **opp )
    {
        Object *o;

        if ( DEBUG__SAFE && !opp )
            ABORT;

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
        ABORT;

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
        object__trace_bfs( root, ( Visitor ) helper, follow_triples );
    else
        object__trace( root, ( Visitor ) helper, follow_triples );

    /* Might as well sweep. */
    if ( dosweep )
        sweep( m );
    else
        unwalk( root, follow_triples );

    m->clean = TRUE;
}


void
manager__trace( Manager *m,
                Object *root,
                Walker walk,
                Visitor v )
{
    int marked;
    Array *visited_objects;

    /* If an object has already been visited, do not apply the visitor function
       a second time (but don't abort the walk). */
    ACTION singlevisit_filter( Object **opp )
    {
        Object *o = DEREF( opp );

        if ( visited( o ) )
            return CONTINUE;

        else
        {
            array__enqueue( visited_objects, o );

            if ( DEBUG__SAFE )
                marked++;

            /* Mark the object. */
            set_visited( o );

            /* Apply the closure. */
            return v( ( void** ) opp );
        }
    }

    ACTION unmark( Object **opp )
    {
        Object *o = DEREF( opp );

        if ( DEBUG__SAFE && visited( o ) )
            marked--;

        clear_visited( o );

        return CONTINUE;
    }

/*
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
*/

    if ( DEBUG__SAFE && ( !m || !walk || !v ) )
        ABORT;

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

    visited_objects = array__new( 0, 0 );

    /* Make sure no objects are marked to begin with. */
    if ( !m->clean )
        unmark_all( m );

    m->clean = FALSE;
    walk( root, ( Visitor ) singlevisit_filter );
/*
    walk( root, ( Visitor ) untrace );
*/

    array__walk( visited_objects, ( Visitor ) unmark );
    array__free( visited_objects );

    if ( DEBUG__SAFE )
    {
        if ( marked > 0 )
            WARNING( "manager__trace: %i objects are still marked as visited", marked );

        else if ( marked < 0 )
            WARNING( "manager__trace: %i more objects unmarked than actually visited", -marked );
    }

    m->clean = TRUE;
}


/******************************************************************************/


Set *
manager__get_multirefs( Manager *m, Object *root )
{
    Set *s;

    ACTION helper( Object **opp )
    {
        Object *o;

        if ( DEBUG__SAFE && !opp )
            ABORT;

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
        ABORT;

    if ( !m->clean )
        unmark_all( m );

    m->clean = 0;

    s = set__new();

    object__trace( root, ( Visitor ) helper, TRUE );

    return s;
}


/* Mark-and-sweep garbage collection ******************************************/


void
manager__collect( Manager *m, boolean force, boolean echo )
{
    /* Avoid "used uninitialized" warning. */
    unsigned int size = 0;

    clock_t t;
    double elapsed_time;

    ACTION noop( Object **opp )
    {
        /* Avoid "unused parameter" warning. */
        opp = 0;

        return CONTINUE;
    }

    if ( DEBUG__SAFE && !m )
        ABORT;

#if DEBUG__MEMORY
    echo = TRUE;
#endif

    if ( echo )
        t = clock();

    if ( echo || !force )
        size = manager__size( m );

    if ( !force )
    {
        if ( m->size_at_last_cycle
          && ( double ) size / ( double ) m->size_at_last_cycle <= MEM__COLLECTION_THRESHOLD )
            return;
    }

    /* Mark all reachable objects. */
    manager__walk( m, 0, ( Visitor ) noop, FALSE, TRUE );

    m->size_at_last_cycle = manager__size( m );

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
