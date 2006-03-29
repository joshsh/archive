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

#include <Memory_Manager.h>
#include <Collection.h>


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
    int clean;
};


/******************************************************************************/


Memory_Manager *
memory_manager__new( Object *root )
{
    Memory_Manager *m;
    Type *Object_t;
printf( "---m 1---\n" ); fflush( stdout );

    #if DEBUG__SAFE
    if ( !root )
    {
        ERROR( "memory_manager__new: null root" );
        return 0;
    }
    else if ( visited( root ) )
    {
        ERROR( "memory_manager__new: root is marked as visited" );
        return 0;
    }
    #endif
printf( "---m 2---\n" ); fflush( stdout );

    if ( !( m = new( Memory_Manager ) ) )
    {
        object__delete( root );
        return 0;
    }
printf( "---m 3---\n" ); fflush( stdout );

    m->objects = bunch__new( MEM__OBJECTS__BLOCK_SIZE );

printf( "---m 4---\n" ); fflush( stdout );

    if ( !m->objects )
    {
        object__delete( root );
        free( m );
        return 0;
    }
printf( "---m 4.5---\n" ); fflush( stdout );
    Object_t = type__new( "object", 0 );
    Object_t->destroy = ( Destructor ) object__delete;
    m->objects_o = object__new(
        bunch__type( "bunch<object>", TYPE__IS_OBJ_COLL | TYPE__OWNS_DESCENDANTS ),
        m->objects, 0 );
    m->objects_o->type->type_arg = Object_t;
printf( "---m 5---\n" ); fflush( stdout );

    m->root = root;
    if ( !memory_manager__add( m, root ) )
    {
        ERROR( "memory_manager__new: could not add root object" );
        object__delete( root );
        bunch__delete( m->objects );
        free( m );
        return 0;
    }

printf( "---m 6---\n" ); fflush( stdout );

    m->clean = 1;

    #if DEBUG__MEMORY
    printf( "[%#x] memory_manager__new(%#x)\n", ( int ) m, ( int ) root );
    #endif

    return m;
}


void
memory_manager__delete( Memory_Manager *m )
{
    Type *Object_t, *Object_bunch_t;

    #if DEBUG__SAFE
    if ( !m )
    {
        ERROR( "memory_manager__delete: null manager" );
        return;
    }
    #endif

    #if DEBUG__MEMORY
    printf( "[] memory_manager__delete(%#x)\n", ( int ) m );
    #endif

printf( "---m d 1---\n" ); fflush( stdout );
    Object_bunch_t = m->objects_o->type;
printf( "---m d 2---\n" ); fflush( stdout );
    Object_t = Object_bunch_t->type_arg;
printf( "---m d 3---\n" ); fflush( stdout );
printf( "m->objects = %#x\n", ( int ) m->objects );
printf( "bunch__size( m->objects ) = %i\n", bunch__size( m->objects ) );

    object__delete( m->objects_o );
printf( "---m d 4---\n" ); fflush( stdout );
    /*bunch__for_all( m->objects, (void*(*)(void*)) object__delete );
    bunch__delete( m->objects );*/
    type__delete( Object_bunch_t );
printf( "---m d 5---\n" ); fflush( stdout );
    type__delete( Object_t );
printf( "---m d 6---\n" ); fflush( stdout );

    free( m );
}


unsigned int
memory_manager__size( Memory_Manager *m )
{
    #if DEBUG__SAFE
    if ( !m )
    {
        ERROR( "memory_manager__delete: null argument" );
        return 0;
    }
    #endif

    return bunch__size( m->objects );
}


void
memory_manager__set_root( Memory_Manager *m, Object *o )
{
    #if DEBUG__SAFE
    if ( !m || !o )
    {
        ERROR( "memory_manager__set_root: null argument" );
        return;
    }
    #endif

    m->root = o;
}


Object *
memory_manager__add( Memory_Manager *m, Object *o )
{
printf( "---m add 1---\n" ); fflush( stdout );

    #if DEBUG__SAFE
    if ( !o )
    {
        ERROR( "memory_manager__add: null object" );
        return 0;
    }
    else if ( owned( o ) )
    {
        ERROR( "memory_manager__add: object already has a manager" );
        return 0;
    }
    else if ( visited( o ) )
    {
        ERROR( "memory_manager__new: object is marked" );
        return 0;
    }

    set_owned( o );
    #endif
printf( "---m add 2---\n" ); fflush( stdout );

    return ( Object* ) bunch__add( m->objects, o );
}


/* Unmarking / cleanup ********************************************************/


static void
unmark_all( Memory_Manager *m )
{
    void unmark( Object *o )
    {
        clear_visited( o );
    }

    #ifdef DEBUG__SAFE
    if ( !m )
    {
        ERROR( "unmark_all: null argument" );
        return;
    }
    #endif

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

    #ifdef DEBUG__SAFE
    if ( !m )
    {
        ERROR( "sweep: null manager" );
        return;
    }
    #endif

    collection__exclude_if( m->objects_o, ( Criterion ) unmarked );
    m->clean = 1;
}


/* Tracing / graph traversal **************************************************/


void
memory_manager__walk( Memory_Manager *m, Dist_f f )
{
    void *helper( Object **opp )
    {
        Object *o = *opp;

        /* If the object is already marked, abort. */
        if ( visited( o ) )
            return walker__break;

        else
        {
            /* Mark the object. */
            set_visited( o );

            /* Execute the procedure. */
            return f( ( void** ) opp );
        }
    }

    if ( !m->clean )
        unmark_all( m );

    m->clean = 0;
printf( "---mm d 1---\n" ); fflush( stdout );

    object__trace( m->root, ( Dist_f ) helper );
printf( "---mm d 2---\n" ); fflush( stdout );

    /* Might as well sweep. */
    sweep( m );
}


/******************************************************************************/


Set *
memory_manager__get_multirefs( Memory_Manager *m, Object *root )
{
    Set *s;

    void *helper( Object **opp )
    {
        Object *o = *opp;

printf( "---mm aim 1---\n" ); FFLUSH;
printf( "o = %#x, o->type=%#x\n", ( int ) o, ( int ) o->type ); FFLUSH;
        /* If the object is already marked, abort. */
        if ( visited( o ) )
        {
printf( "---mm aim 2a---\n" ); FFLUSH;
            set__add( s, o );
            return walker__break;
        }

        else
        {
printf( "---mm aim 2b 1---\n" ); FFLUSH;
            /* Mark the object. */
            set_visited( o );
printf( "---mm aim 2b 2---\n" ); FFLUSH;

            #if ENCODING__TRIPLES_AS_OBJECTS & TRIPLES__GLOBAL__OUT_EDGES
            /* Object references its triples, which in turn reference the object. */
            if ( o->outbound_edges && hash_table__size( o->outbound_edges ) )
            {
                set__add( s, o );
            }
            #endif
printf( "---mm aim 2b 3---\n" ); FFLUSH;

            return 0;
        }
    }

    if ( !m->clean )
        unmark_all( m );
    m->clean = 0;

    s = set__new();
    object__trace( root, ( Dist_f ) helper );
    return s;
}


/* Mark-and-sweep garbage collection ******************************************/


void
memory_manager__collect( Memory_Manager *m )
{
    void *noop( Object **opp )
    {
        /* Avoid "unused parameter" warning. */
        opp = 0;

        return 0;
    }

    #if DEBUG__MEMORY
    int n_initial = bunch__size( m->objects );
    #endif

    #ifdef DEBUG__SAFE
    if ( !m )
    {
        ERROR( "memory_manager__collect: null manager" );
        return;
    }
    #endif
printf( "---mm c 1---\n" ); FFLUSH;

    /* Mark all reachable objects. */
    memory_manager__walk( m, ( Dist_f ) noop );
printf( "---mm c 2---\n" ); FFLUSH;

    #if DEBUG__MEMORY
    printf( "memory_manager__collect(%#x): deallocated %i of %i.\n",
        ( int ) m, n_initial - bunch__size( m->objects ), n_initial );
    FFLUSH;
    #endif
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
