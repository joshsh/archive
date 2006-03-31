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

    #if DEBUG__OBJECT
    printf( "[%#x] object__new(%#x, %#x, %i)\n",
        ( int ) o, ( int ) type, ( int ) value, flags );
if ( type && value )
{
if (!strcmp( type->name, "type"))
printf( "    This is type '%s'.\n", ( ( Type* ) value )->name );
}
    #endif

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

    #if DEBUG__SAFE
    if ( !o )
    {
        ERROR( "object__delete: null object" );
        return;
    }
    #endif

    #if DEBUG__OBJECT
    printf( "[] object__delete(%#x): value = %#x, type = %#x.\n",
        ( int ) o, ( int ) o->value, ( int ) o->type );
    #endif

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

    #if DEBUG__OBJECT
    FFLUSH;
    #endif
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
    #if DEBUG__SAFE
    if ( !o )
    {
        ERROR( "object__type: null argument" );
        return 0;
    }
    #endif

    return o->type;
}


void *
object__value( const Object *o )
{
    #if DEBUG__SAFE
    if ( !o )
    {
        ERROR( "object__value: null argument" );
        return 0;
    }
    #endif

    return o->value;
}


/* Dispatch functions *********************************************************/


static Object *
object__clone( Object *o )
{
    Object *copy = new( Object );
    *copy = *o;
    copy->value = o->type->clone( o->value );
    return copy;
}


static int
object__compare_to( Object *o1, Object *o2 )
{
    #if DEBUG__SAFE
    if ( o1->type != o2->type )
    {
        ERROR( "object__compare_to: type mismatch" );
        return 0;
    }
    #endif

    return o1->type->compare_to( o1->value, o2->value );
}


/* Note: there is no object decoder, as the data type of an object cannot be
   inferred from the buffer alone. */

/* Note: object destructor has external linkage (see above). */

/* Note: object encoder is omitted for consistency. */


static boolean
object__equals( Object *o1, Object *o2 )
{
    #if DEBUG__SAFE
    if ( o1->type != o2->type )
    {
        ERROR( "object__equals: type mismatch" );
        return 0;
    }
    #endif

    return o1->type->equals( o1->value, o2->value );
}


static void
object__sort( Object *o, Comparator cmp )
{
    o->type->sort( o->value, cmp );
}


static void
object__walk( Object *o, Dist_f f )
{
    o->type->walk( o->value, f );
}


Type *
object__create_type( const char *name )
{
    int flags = 0;

    Type *type = type__new( name, flags );

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
object__trace( Object *o, Dist_f f )
{
    #if DEBUG__OBJECT
    int total = 0;
    #endif

    auto void *obj_trace( Object **opp );

    void *edge_trace( Hash_Map__Entry **epp )
    {
        #if TRIPLES__IMPLICATION__SP_O
        ... not yet written ...
        #else
        #if TRIPLES__IMPLICATION__S_P
        obj_trace( ( Object** ) &( *epp )->key );
        #endif
        #if TRIPLES__IMPLICATION__S_O
        obj_trace( ( Object** ) &( *epp )->target );
        #endif
        #endif

        return 0;
    }

    void *obj_trace( Object **opp )
    {
        Object *o;

        /* If for any reason execution has traced to a NULL, return immediately. */
        if ( !opp )
        {
            #if DEBUG__SAFE
            WARNING( "obj_trace: null object" );
            #endif

            return 0;
        }

        o = *opp;

        /* Execute the inner procedure.  Recurse unless instructed otherwise. */
        if ( !f( ( void** ) opp ) )
        {
            #if DEBUG__OBJECT
            total++;
            #endif

            /* Traverse to children (if any). */
            if ( o->type->flags & TYPE__IS_OBJ_COLL )
            {
                o->type->walk( o->value, ( Dist_f ) obj_trace );
            }

            /* Traverse to associated objects (if any). */
            #if TRIPLES__GLOBAL__OUT_EDGES
            if ( o->outbound_edges )
            {
                hash_map__walk( o->outbound_edges, ( Dist_f ) edge_trace );
            }
            #endif
        }

        return 0;
    }

    #if DEBUG__SAFE
    if ( !o )
    {
        ERROR( "object__trace: null object" );
        return;
    }
    #endif

    obj_trace( &o );

    #if DEBUG_OBJECT
    printf( "[] object__trace(%#x, %#x): visited %i objects.\n",
        ( int ) o, ( int ) f, total );
    #endif
}


#ifdef COMMENTED_OUT_FOR_NOW

static p2_action *
enqueue( Object *o, Array *queue )
{
    array__enqueue( queue, o );
    return 0;
}


/* Note: untested. */
void
object__trace_bfs( Object *o, Closure *p )
{
    Trace_Ctx state;
    Closure trace_proc;
    Closure edge_p;
    Closure outer_p;

    Array *queue;

    #if DEBUG__SAFE
    if ( !o )
    {
        ERROR( "object__trace: null object" );
        return;
    }
    #endif

    queue = array__new( 0, 0 );
    array__enqueue( queue, o );

    edge_p.execute = ( procedure ) apply_to_assoc_edge;
    edge_p.state = &trace_proc;

    outer_p.execute = ( procedure ) enqueue;
    outer_p.state = queue;

    state.outer_p = &outer_p;
    state.inner_p = p;
    state.edge_p = &edge_p;

    trace_proc.execute = ( procedure ) trace_exec;
    trace_proc.state = &state;

    while ( array__size( queue ) )
    {
        closure__execute( ( &trace_proc ),
            ( Object* ) array__pop( queue ) );
    }
}

#endif


/* Association ****************************************************************/


#if TRIPLES__GLOBAL

Object *
object__multiply( Object *subj, Object *pred )
{
    #if DEBUG__SAFE
    if ( !subj || !pred )
    {
        ERROR( "object__multiply: null argument" );
        return 0;
    }
    #endif

    #if TRIPLES__GLOBAL__OUT_EDGES

    return ( subj->outbound_edges ) ?
        ( Object* ) hash_map__lookup( subj->outbound_edges, pred ) : 0 ;

    #else

    return 0;

    #endif
}


/* Note: doesn't take association sets into account yet. */
Object *
object__associate( Object *subj, Object *pred, Object *obj )
{
    #if DEBUG__SAFE
    if ( !subj || !pred || !obj )
    {
        ERROR( "object__associate: null argument" );
        return 0;
    }
    #endif

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

#endif  /* TRIPLES__GLOBAL */


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
