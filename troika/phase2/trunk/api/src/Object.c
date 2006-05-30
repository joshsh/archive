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

#include <Object.h>
#include <util/Array.h>
#include "Object-impl.h"


Object *
object__new( Type *type, void *value, int flags )
{
    Object *o;

    /* Note: temporary objects with null type/value are allowed. */

    o = new( Object );

    if ( !o )
    {
        ERROR( "object__new: allocation failure" );
        return 0;
    }

    o->type = type;
    o->value = value;
    o->flags = flags;

#if TRIPLES__GLOBAL__IN_EDGES
    o->inbound_edges = 0;
#endif

#if TRIPLES__GLOBAL__OUT_EDGES
    o->outbound_edges = 0;
#endif

#if TRIPLES__GLOBAL__TRANS_EDGES
    o->trans_edges = 0;
#endif

    return o;
}


void
object__delete( Object *o )
{
    Type *t;

    void *helper( void **refp )
    {
        t->destroy( *refp );
        return 0;
    }

    if ( DEBUG__SAFE && !o )
        abort();

#if TRIPLES__GLOBAL__IN_EDGES
    if ( o->inbound_edges )
        hash_map__delete( o->inbound_edges );
#endif

#if TRIPLES__GLOBAL__OUT_EDGES
    if ( o->outbound_edges )
        hash_map__delete( o->outbound_edges );
#endif

#if TRIPLES__GLOBAL__TRANS_EDGES
    if ( o->trans_edges )
        hash_map__delete( o->trans_edges );
#endif

    /* If the object owns its children (and has any), free them. */
    if ( o->type->flags & TYPE__OWNS_DESCENDANTS )
    {
        t = o->type->type_arg;
        o->type->walk( o->value, ( Dist_f ) helper );
    }

    /* Free the object's data. */
    o->type->destroy( o->value );

    free( o );
}


/* Accessors ******************************************************************/


boolean
object__immutable( const Object *o )
{
    return o->flags & OBJECT__IMMUTABLE;
}


Type *
object__type( const Object *o )
{
    if ( DEBUG__SAFE && !o )
        abort();

    return o->type;
}


void *
object__value( const Object *o )
{
    if ( DEBUG__SAFE && !o )
        abort();

    return o->value;
}


/* Dispatch functions *********************************************************/


static Object *
object__clone( Object *o )
{
    Object *copy;

    if ( DEBUG__SAFE && !o )
        abort();

    copy = new( Object );
    *copy = *o;
    copy->value = o->type->clone( o->value );
    return copy;
}


static int
object__compare_to( Object *o1, Object *o2 )
{
    if ( DEBUG__SAFE && ( !o1 || !o2 ) )
        abort();

    else if ( o1->type != o2->type )
    {
        ERROR( "object__compare_to: type mismatch" );
        return 0;
    }

    else
        return o1->type->compare_to( o1->value, o2->value );
}


/* Note: there is no object decoder, as the data type of an object cannot be
   inferred from the buffer alone. */

/* Note: object destructor has external linkage (see above). */

/* Note: object encoder is omitted for consistency. */


static boolean
object__equals( Object *o1, Object *o2 )
{
    if ( DEBUG__SAFE && ( !o1 || !o2 ) )
        abort();

    else if ( o1->type != o2->type )
    {
        ERROR( "object__equals: type mismatch" );
        return 0;
    }

    else
        return o1->type->equals( o1->value, o2->value );
}


static void
object__sort( Object *o, Comparator cmp )
{
    if ( DEBUG__SAFE && !o )
        abort();

    o->type->sort( o->value, cmp );
}


static void
object__walk( Object *o, Dist_f f )
{
    if ( DEBUG__SAFE && !o )
        abort();

    o->type->walk( o->value, f );
}


Type *
object__create_type( const char *name )
{
    int flags = 0;
    Type *type;

    if ( DEBUG__SAFE && !name )
        abort();

    type = type__new( name, flags );

    if ( type )
    {
        type->clone = ( Copy_Cons ) object__clone;
        type->compare_to = ( Comparator ) object__compare_to;
        type->destroy = ( Destructor ) object__delete;
        type->equals = ( Criterion2 ) object__equals;
        type->sort = ( Sort ) object__sort;
        type->walk = ( Walker ) object__walk;
    }

    return type;
}


/* Graph traversal ************************************************************/


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


/* Association ****************************************************************/


#if TRIPLES__GLOBAL

Object *
object__multiply( Object *subj, Object *pred )
{
    Object *obj;

    if ( DEBUG__SAFE && ( !subj || !pred ) )
        abort();

#if TRIPLES__GLOBAL__OUT_EDGES
    obj = ( subj->outbound_edges )
        ? hash_map__lookup( subj->outbound_edges, pred )
        : 0 ;

#else
    obj = 0;
#endif

    return obj;
}


Object *
object__associate( Object *subj, Object *pred, Object *obj )
{
    if ( DEBUG__SAFE && ( !subj || !pred || !obj ) )
        abort();

#if TRIPLES__GLOBAL__OUT_EDGES
    if ( !subj->outbound_edges
      && !( subj->outbound_edges = hash_map__new() ) )
        subj = 0;

    else
    {
        if ( obj )
            hash_map__add( subj->outbound_edges, pred, obj );

        else
            hash_map__remove( subj->outbound_edges, pred );
    }
#else
    subj = 0;
#endif

    return subj;
}


Object *
object__dissociate( Object *subj, Object *pred )
{

    if ( DEBUG__SAFE && ( !subj || !pred ) )
        abort();

#if TRIPLES__GLOBAL__OUT_EDGES
    if ( subj->outbound_edges )
    {
        hash_map__remove( subj->outbound_edges, pred );
        if ( !hash_table__size( subj->outbound_edges ) )
        {
            hash_map__delete( subj->outbound_edges );
            subj->outbound_edges = 0;
        }
    }
#else
    subj = 0;
#endif

    return subj;
}


#ifdef NOT_FINISHED
static Object *
union_of( Object *o1, Object *o2 )
{
    Set *s;
    Object *o3;

    if ( o1 )
    {
        if ( o1->type == set_t )
        {
            if ( o2 )
            {
                /* A + A */
                if ( o2->type == set_t )
                {

                }

                /* A + a */
                else
                {

                }
            }

            /* A + 0 */
            else
            {
                set__remove( s, o2 );
            }
        }

        else if ( o2 )
        {
            /* a + A */
            if ( o2->type == set_t )
            {

            }

            /* a + a */
            else
            {
                s = set__new();
                set__add( s, o1 );
                set__add( s, o2 );
            }
        }

        /* a + 0 */
        else
        {

        }
    }

    else
    {
        if ( o2 )
        {
            /* 0 + A */
            if ( o2->type == set_t )
            {
                s = set__copy( o2->value );
                o3 = object__new( set_t, s, 0 );
                memory_manager__add( environment__manager( global_env ), o3 );
                return o3;
            }

            /* 0 + a */
            else
                return o2;
        }

        /* 0 + 0 */
        else
            return 0;
    }
}


Object *
object__union_associate( Object *subj, Object *pred, Object *obj )
{
    Object *o;

    if ( DEBUG__SAFE && ( !subj || !pred || !obj )
        abort();

#if TRIPLES__GLOBAL__OUT_EDGES
    if ( !( subj->outbound_edges || ( subj->outbound_edges = hash_map__new() ) ) )
        subj = 0;

    else
    {
        o = union_of( hash_map__lookup( subj->outbound_edges, pred ), obj );
        hash_map__add( subj->outbound_edges, pred, o );
    }
#else
    subj = 0;
#endif

    return subj;
}
#endif

#endif  /* TRIPLES__GLOBAL */


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
