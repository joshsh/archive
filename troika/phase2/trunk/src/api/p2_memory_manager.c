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
#include "p2_collection.h"


/******************************************************************************/


p2_memory_manager *p2_memory_manager__new( p2_object *root )
{
    p2_memory_manager *m;
    p2_type *p2_object_t;
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
printf( "---m 4.5---\n" ); fflush( stdout );
    p2_object_t = p2_type__new( "object", 0 );
    p2_object_t->destroy = ( destructor ) p2_object__delete;
    m->objects_obj = p2_object__new( p2_bunch__type( "bunch<object>" ), m->objects, OBJECT__OWNS_DESCENDANTS );
    m->objects_obj->type->type_arg = p2_object_t;
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

    return m;
}


void p2_memory_manager__delete( p2_memory_manager *m )
{
    p2_type *p2_object_t, *p2_object_bunch_t;

    #if DEBUG__SAFE
    if ( !m )
    {
        PRINTERR( "p2_memory_manager__delete: null manager" );
        return;
    }
    #endif

    p2_object_bunch_t = m->objects_obj->type;
    p2_object_t = p2_object_bunch_t->type_arg;

    p2_object__delete( m->objects_obj );
    /*p2_bunch__for_all( m->objects, (void*(*)(void*)) p2_object__delete );
    p2_bunch__delete( m->objects );*/
    p2_type__delete( p2_object_bunch_t );
    p2_type__delete( p2_object_t );

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


static boolean unmark_for_sweep( p2_object *o )
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
        return boolean__false;
    }

    /* If unmarked, delete. */
    else
    {
        p2_object__delete( o );

        /* Exclude this object. */
        return boolean__true;
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

    p2_collection__do_for_all( m->objects_obj, ( void_f ) unmark );
    /*p2_bunch__for_all( m->objects, (void*(*)(void*)) unmark );*/
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

    p2_collection__exclude_if( m->objects_obj, ( criterion ) unmark_for_sweep );
    /*p2_bunch__exclude_if( m->objects, (void*(*)(void*)) unmark_for_sweep );*/
    m->clean = 1;
}


/* Tracing / graph traversal **************************************************/


static p2_action * dist_p_exec( p2_object *o, p2_procedure *p )
{
    /* If the object is already marked, abort. */
    if ( o->flags & OBJECT__MARKED )
    {
        return ( p2_action* ) 1;
    }

    else
    {
        /* Mark the object. */
        o->flags = o->flags | OBJECT__MARKED;

        /* Execute the procedure. */
        return p2_procedure__execute( p, o );
    }
}


void p2_memory_manager__distribute( p2_memory_manager *m, p2_procedure *p )
{
    p2_procedure dist_p;

    dist_p.execute = ( procedure ) dist_p_exec;
    dist_p.state = p;

    if ( !m->clean )
        unmark_all( m );

    m->clean = 0;

    p2_object__trace( m->root, &dist_p );

    /* Might as well sweep. */
    sweep( m );
}


/* Mark-and-sweep garbage collection ******************************************/


static p2_action * noop( void *x, void *state )
{
    return 0;
}


void p2_memory_manager__mark_and_sweep( p2_memory_manager *m )
{
    p2_procedure noop_p;

    #ifdef DEBUG__SAFE
    if ( !m )
    {
        PRINTERR( "p2_memory_manager__mark_and_sweep: null manager" );
        return;
    }
    #endif
printf( "---m ms 1---\n" ); fflush( stdout );
    noop_p.execute = ( procedure ) noop;

    /* Mark all reachable objects. */
    p2_memory_manager__distribute( m, &noop_p );
printf( "---m ms 2---\n" ); fflush( stdout );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
