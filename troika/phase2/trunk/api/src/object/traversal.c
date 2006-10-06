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

#include <phase2/Array.h>
#include "Object-impl.h"


void
object__trace( Object *o, Visitor f, boolean follow_triples )
{
    int total;

    ACTION helper( Object **opp )
    {
#if TRIPLES__IMPLICATION__S_P && TRIPLES__IMPLICATION__S_O
        ACTION edge_helper( Object **pred, Object **obj )
        {
            if ( DEBUG__SAFE && ( !pred || !obj ) )
                ABORT;

            helper( pred );
            helper( obj );

            return CONTINUE;
        }
#else
        Fail to compile.
#endif



/*
data(
    bar=@(@(V())1)
    .()
    .()
    .()
)

data(
    .(.(.().).())
    .()
    .(.)
    .

data(cycle(cycle))
*/

        Object *o;

        /* Null object references may be allowed, but null indirection to object
           references is not. */
        if ( DEBUG__SAFE && !opp )
            ABORT;

        /* Execute the inner procedure.  Recurse unless instructed otherwise. */
        if ( f( ( void** ) opp ) == CONTINUE )
        {
            if ( DEBUG__OBJECT )
                putchar( '(' );

            if ( DEBUG__OBJECT )
                total++;

            /* NOTE: the object is only dereferenced AFTER it has been passed to
               the distributor function, in case the reference is mutated by
               that operation. */
            o = *opp;

            if ( DEBUG__SAFE && !o )
                ABORT;

            /* Traverse to children (if any). */
            if ( o->type->flags & TYPE__IS_OBJ_COLL )
            {
                o->type->walk( o->value, ( Visitor ) helper );
            }

            /* Traverse to associated objects (if any). */
#if TRIPLES__GLOBAL__OUT_EDGES
            if ( follow_triples && o->outbound_edges )
            {
                hash_map__walk2( o->outbound_edges, ( Visitor2 ) edge_helper );
            }
#endif

            if ( DEBUG__OBJECT )
                putchar( ')' );
        }

        return CONTINUE;
    }

    if ( DEBUG__SAFE && !o )
        ABORT;

    if ( DEBUG__OBJECT )
        total = 0;

    helper( &o );

    if ( DEBUG__OBJECT )
        PRINT( "[] object__trace(%p, %p): visited %i objects.\n",
            o, f, total );
}


/* NOTE: untested. */
void
object__trace_bfs( Object *o, Visitor f, boolean follow_triples )
{
    Array *queue;
    int total;

    ACTION enqueue( Object **opp )
    {
        array__enqueue( queue, opp );
        return CONTINUE;
    }

    ACTION helper( Object **opp )
    {
#if TRIPLES__IMPLICATION__S_P && TRIPLES__IMPLICATION__S_O
        ACTION edge_helper( Object **pred, Object **obj )
        {
            enqueue( pred );
            enqueue( obj );
            return CONTINUE;
        }
#else
        Die horribly.
#endif

        Object *o;

        /* If for any reason execution has traced to a NULL, return immediately. */
        if ( !opp )
        {
            if ( DEBUG__SAFE )
                WARNING( "obj_trace_bfs: null object" );

            return CONTINUE;
        }

        /* Execute the inner procedure.  Recurse unless instructed otherwise. */
        if ( !f( ( void** ) opp ) )
        {
            if ( DEBUG__OBJECT )
                total++;

            /* NOTE: the object is only dereferenced AFTER it has been passed to
               the distributor function, in case the reference is mutated by
               that operation. */
            o = *opp;

            /* Traverse to children (if any). */
            if ( o->type->flags & TYPE__IS_OBJ_COLL )
            {
                o->type->walk( o->value, ( Visitor ) enqueue );
            }

            /* Traverse to associated objects (if any). */
#if TRIPLES__GLOBAL__OUT_EDGES
            if ( follow_triples && o->outbound_edges )
            {
                hash_map__walk( o->outbound_edges, ( Visitor ) edge_helper );
            }
#endif
        }

        return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !o  || !f ) )
        ABORT;

    if ( DEBUG__OBJECT )
        total = 0;

    queue = array__new( 0, 0 );
    array__enqueue( queue, &o );

    while ( array__size( queue ) )
    {
        helper( array__pop( queue ) );
    }

    array__free( queue );

    if ( DEBUG__OBJECT )
        PRINT( "[] object__trace_bfs(%p, %p): visited %i objects.\n",
            o, f, total );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
