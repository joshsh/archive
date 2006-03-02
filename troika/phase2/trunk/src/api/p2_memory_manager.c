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

#include "p2_memory_manager.h"


/* Warning: multithreading hazard. */
static void *(*global_f)(p2_object*);


/******************************************************************************/


p2_memory_manager *p2_memory_manager__new( p2_object *root )
{
    p2_memory_manager *m;
printf( "---m 1---\n" ); fflush( stdout );

    #if DEBUG__SAFE
    if ( !root )
    {
        PRINTERR( "p2_memory_manager__new: null root" );
        return 0;
    }
    else if ( root->flags & OBJECT__MARKED )
    {
        PRINTERR( "p2_memory_manager__new: root is marked" );
        return 0;
    }
    #endif
printf( "---m 2---\n" ); fflush( stdout );

    if ( !( m = new( p2_memory_manager ) ) )
    {
        p2_object__delete( root );
        return 0;
    }
printf( "---m 3---\n" ); fflush( stdout );

    m->objects = p2_bunch__new( MEM_MANAGER__OBJECTS__BLOCK_SIZE );
printf( "---m 4---\n" ); fflush( stdout );

    if ( !m->objects )
    {
        p2_object__delete( root );
        free( m );
        return 0;
    }
printf( "---m 5---\n" ); fflush( stdout );

    m->root = root;
    if ( !p2_memory_manager__add( m, root ) )
    {
        PRINTERR( "p2_memory_manager__new: could not add root object" );
        p2_object__delete( root );
        p2_bunch__delete( m->objects );
        free( m );
        return 0;
    }

printf( "---m 6---\n" ); fflush( stdout );

    m->clean = 1;

    global_f = 0;

    return m;
}


void p2_memory_manager__delete( p2_memory_manager *m )
{
    #if DEBUG__SAFE
        if ( !m )
        {
            PRINTERR( "p2_memory_manager__delete: null manager" );
            return;
        }
    #endif

    p2_bunch__for_all( m->objects, (void*(*)(void*)) p2_object__delete );
    p2_bunch__delete( m->objects );
    free( m );
}


unsigned int p2_memory_manager__size( p2_memory_manager *m )
{
    #if DEBUG__SAFE
        if ( !m )
        {
            PRINTERR( "p2_memory_manager__delete: null manager" );
            return 0;
        }
    #endif

    return p2_bunch__size( m->objects );
}


p2_object *p2_memory_manager__add( p2_memory_manager *m, p2_object *o )
{
printf( "---m add 1---\n" ); fflush( stdout );

    #if DEBUG__SAFE
        if ( !o )
        {
            PRINTERR( "p2_memory_manager__add: null object" );
            return 0;
        }

        else if ( o->flags & OBJECT__OWNED )
        {
            PRINTERR( "p2_memory_manager__add: object already has a manager" );
            return 0;
        }

        else if ( o->flags & OBJECT__MARKED )
        {
            PRINTERR( "p2_memory_manager__new: object is marked" );
            return 0;
        }

        o->flags = o->flags | OBJECT__OWNED;
    #endif
printf( "---m add 2---\n" ); fflush( stdout );

    return ( p2_object* ) p2_bunch__add( m->objects, o );
}


/* Unmarking / cleanup ********************************************************/


static void *unmark( p2_object *o )
{
    #ifdef DEBUG__SAFE
        if ( !o )
        {
            PRINTERR( "unmark: null object" );
            return 0;
        }
    #endif

    o->flags = o->flags & ~OBJECT__MARKED;
    return o;
}


static void *unmark_for_sweep( p2_object *o )
{
    #ifdef DEBUG__SAFE
        if ( !o )
        {
            PRINTERR( "unmark_for_sweep: null object" );
            return 0;
        }
    #endif

    /* If marked, unmark. */
    if ( o->flags & OBJECT__MARKED )
    {
        o->flags = o->flags & ~OBJECT__MARKED;

        /* Don't exclude this object. */
        return 0;
    }

    /* If unmarked, delete. */
    else
    {
        p2_object__delete( o );

        /* Exclude this object. */
        return o;
    }
}


static void unmark_all( p2_memory_manager *m )
{
    #ifdef DEBUG__SAFE
        if ( !m )
        {
            PRINTERR( "unmark_all: null manager" );
            return;
        }
    #endif

    p2_bunch__for_all( m->objects, (void*(*)(void*)) unmark );
    m->clean = 1;
}


/* Unmarks all marked objects in the environment, and deletes the rest.*/
static void sweep( p2_memory_manager *m )
{
    #ifdef DEBUG__SAFE
        if ( !m )
        {
            PRINTERR( "sweep: null manager" );
            return;
        }
    #endif

    p2_bunch__exclude_if( m->objects, (void*(*)(void*)) unmark_for_sweep );
    m->clean = 1;
}


/* Tracing / graph traversal **************************************************/


/* Needed by trace_through_assoc_edge */
static void *trace( p2_object *o );


static void *trace_through_assoc_edge( p2_object *pred, p2_object *obj )
{
    #if TRIPLES__IMPLICATION__SP_O

        ... not yet written ...

    #else

        #if TRIPLES__IMPLICATION__S_P
            if ( !trace( pred ) )
                return 0;
        #endif

        #if TRIPLES__IMPLICATION__S_O
            if ( !trace( obj ) )
                return 0;
        #endif

    #endif

    /* Obligatory non-zero return value. */
    return pred;
}


static void *trace( p2_object *o )
{
    #ifdef DEBUG__SAFE
        if ( !o )
        {
            PRINTERR( "trace: null object" );
            return 0;
        }
    #endif
printf( "---m trace 0x%X (0x%X)---\n", ( int ) o, ( int ) o->value ); fflush( stdout );

    /* If the object is not already marked... */
    if ( !( o->flags & OBJECT__MARKED ) )
    {
        /* Mark the object. */
        o->flags = o->flags | OBJECT__MARKED;

        /* Apply the target function (if any). */
        if ( global_f && !global_f( o ) )
            return 0;

if ( o->flags & OBJECT__IS_OBJ_COLL )
    printf( "---m trace (found collection: 0x%X)---\n", ( int ) o );
        /* If it's a collection of objects, mark its descendants. */
        if ( ( o->flags & OBJECT__IS_OBJ_COLL )
          && !p2_object__for_all( o, (void*(*)(void*)) trace ) )
                return 0;

        #if TRIPLES__GLOBAL

            #if TRIPLES__GLOBAL__OUT_EDGES

                if ( o->outbound_edges
                  && !p2_hash_table__for_all(
                        o->outbound_edges,
                        (void*(*)(void*, void*)) trace_through_assoc_edge ) )
                    return 0;

            #endif

        #endif
    }

    /* Perfunctory non-zero return value. */
    return o;
}


void *p2_memory_manager__for_all_reachable(
    p2_memory_manager *m, void *(*f)(p2_object*) )
{
    #ifdef DEBUG__SAFE
        if ( !m )
        {
            PRINTERR( "p2_memory_manager__for_all_reachable: null manager" );
            return 0;
        }
    #endif

    if ( !m->clean )
        unmark_all( m );

    m->clean = 0;

    global_f = f;
    return trace( m->root );
}


/* Mark-and-sweep garbage collection ******************************************/


void p2_memory_manager__mark_and_sweep( p2_memory_manager *m )
{
    #ifdef DEBUG__SAFE
        if ( !m )
        {
            PRINTERR( "p2_memory_manager__mark_and_sweep: null manager" );
            return;
        }
    #endif
printf( "---m ms 1---\n" ); fflush( stdout );

    if ( !m->clean )
        unmark_all( m );
printf( "---m ms 2---\n" ); fflush( stdout );

    m->clean = 0;

    /* Mark. */
    global_f = 0;
    trace( m->root );
printf( "---m ms 3---\n" ); fflush( stdout );

    /* Sweep. */
    sweep( m );
printf( "---m ms 4---\n" ); fflush( stdout );

}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
