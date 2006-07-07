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

#include "Object-impl.h"
#include <collection/Array.h>


void
object__trace( Object *o, Dist_f f, boolean follow_triples )
{
    int total;

    auto void *obj_trace( Object **opp );

    void *edge_trace( Hash_Map__Entry **epp )
    {
#if TRIPLES__IMPLICATION__SP_O
        ... not yet written ...
#else
#  if TRIPLES__IMPLICATION__S_P
        obj_trace( ( Object** ) &( *epp )->key );
#  endif
#  if TRIPLES__IMPLICATION__S_O
        obj_trace( ( Object** ) &( *epp )->target );
#  endif
#endif

        return 0;
    }

    void *obj_trace( Object **opp )
    {
        Object *o;

        /* If for any reason execution has traced to a NULL, return immediately. */
        if ( !opp )
        {
            if ( DEBUG__SAFE )
                WARNING( "obj_trace: null object" );

            return 0;
        }

        /* Execute the inner procedure.  Recurse unless instructed otherwise. */
        if ( !f( ( void** ) opp ) )
        {
            if ( DEBUG__OBJECT )
                total++;

            /* Note: the object is only dereferenced AFTER it has been passed to
               the distributor function, in case the reference is mutated by
               that operation. */
            o = *opp;

            /* Traverse to children (if any). */
            if ( o->type->flags & TYPE__IS_OBJ_COLL )
            {
                o->type->walk( o->value, ( Dist_f ) obj_trace );
            }

            /* Traverse to associated objects (if any). */
#if TRIPLES__GLOBAL__OUT_EDGES
            if ( follow_triples && o->outbound_edges )
            {
                hash_map__walk( o->outbound_edges, ( Dist_f ) edge_trace );
            }
#endif
        }

        return 0;
    }

    if ( DEBUG__SAFE && !o )
        abort();

    if ( DEBUG__OBJECT )
        total = 0;

    obj_trace( &o );

    if ( DEBUG__OBJECT )
        printf( "[] object__trace(%#x, %#x): visited %i objects.\n",
            ( int ) o, ( int ) f, total );
}


/* Note: untested. */
void
object__trace_bfs( Object *o, Dist_f f, boolean follow_triples )
{
    Array *queue;
    int total;

    void *enqueue( Object **opp )
    {
        array__enqueue( queue, opp );
        return 0;
    }

    void *edge_trace( Hash_Map__Entry **epp )
    {
#if TRIPLES__IMPLICATION__SP_O
        ... not yet written ...
#else
#  if TRIPLES__IMPLICATION__S_P
        enqueue( ( Object** ) &( *epp )->key );
#  endif
#  if TRIPLES__IMPLICATION__S_O
        enqueue( ( Object** ) &( *epp )->target );
#  endif
#endif

        return 0;
    }

    void *obj_trace( Object **opp )
    {
        Object *o;

        /* If for any reason execution has traced to a NULL, return immediately. */
        if ( !opp )
        {
            if ( DEBUG__SAFE )
                WARNING( "obj_trace_bfs: null object" );

            return 0;
        }

        /* Execute the inner procedure.  Recurse unless instructed otherwise. */
        if ( !f( ( void** ) opp ) )
        {
            if ( DEBUG__OBJECT )
                total++;

            /* Note: the object is only dereferenced AFTER it has been passed to
               the distributor function, in case the reference is mutated by
               that operation. */
            o = *opp;

            /* Traverse to children (if any). */
            if ( o->type->flags & TYPE__IS_OBJ_COLL )
            {
                o->type->walk( o->value, ( Dist_f ) enqueue );
            }

            /* Traverse to associated objects (if any). */
#if TRIPLES__GLOBAL__OUT_EDGES
            if ( follow_triples && o->outbound_edges )
            {
                hash_map__walk( o->outbound_edges, ( Dist_f ) edge_trace );
            }
#endif
        }

        return 0;
    }

    if ( DEBUG__SAFE && ( !o  || !f ) )
        abort();

    if ( DEBUG__OBJECT )
        total = 0;

    queue = array__new( 0, 0 );
    array__enqueue( queue, &o );

    while ( array__size( queue ) )
    {
        obj_trace( array__pop( queue ) );
    }

    array__delete( queue );

    if ( DEBUG__OBJECT )
        printf( "[] object__trace_bfs(%#x, %#x): visited %i objects.\n",
            ( int ) o, ( int ) f, total );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
