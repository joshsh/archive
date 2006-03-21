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

    #if DEBUG__OBJECT
    if ( o->type && o->value )
    {
        printf( "object__new: created object %#x (value %#x) of type '%s' (%#x).\n",
            ( int ) o, ( int ) o->value, o->type->name, ( int ) o->type );
if (!strcmp( o->type->name, "type"))
printf( "    This is type '%s'.\n", ( ( Type* ) o->value )->name );
    }
    #endif

    return o;
}


static p2_action *
delete_p( void *p, Type *type )
{
    type->destroy( p );
    return 0;
}


void
object__delete( Object *o )
{
    p2_procedure p;

    #if DEBUG__SAFE
    if ( !o )
    {
        ERROR( "object__delete: null object" );
        return;
    }
    #endif

    #if DEBUG__OBJECT
    printf( "object__delete(%#x): value = %#x, type = %#x.\n",
        ( int ) o, ( int ) o->value, ( int ) o->type );
    #endif

    #if TRIPLES__GLOBAL__IN_EDGES
    if ( o->inbound_edges )
        lookup_table__delete( o->inbound_edges );
    #endif

    #if TRIPLES__GLOBAL__OUT_EDGES
    if ( o->outbound_edges )
        lookup_table__delete( o->outbound_edges );
    #endif

    #if TRIPLES__GLOBAL__TRANS_EDGES
    if ( o->trans_edges )
        lookup_table__delete( o->trans_edges );
    #endif

    /* If the object owns its children (and has any), free them. */
    if ( o->type->flags & TYPE__OWNS_DESCENDANTS )
    {
        p.execute = ( procedure ) delete_p;
        p.state = o->type->type_arg;
        o->type->distribute( o->value, &p );
    }

    /* Free the object's data. */
    o->type->destroy( o->value );

    free( o );

    #if DEBUG__OBJECT
    FFLUSH;
    #endif
}


/* Graph traversal ************************************************************/


typedef struct Trace_Ctx Trace_Ctx;

struct Trace_Ctx
{
    p2_procedure *outer_p;
    p2_procedure *inner_p;
    p2_procedure *edge_p;
};


static p2_action *
apply_to_assoc_edge( Lookup_Table__Entry *entry, p2_procedure *p )
{
    #if TRIPLES__IMPLICATION__SP_O
    ... not yet written ...
    #else
    #if TRIPLES__IMPLICATION__S_P
    p2_procedure__execute( p, entry->key );
    #endif
    #if TRIPLES__IMPLICATION__S_O
    p2_procedure__execute( p, entry->target );
    #endif
    #endif

    return 0;
}


static p2_action *
trace_exec( Object *o, Trace_Ctx *state )
{
    p2_action *action;

    /* If for any reason execution has traced to a NULL, return immediately. */
    if ( !o )
    {
        #if DEBUG__SAFE
        WARNING( "trace_exec: null object" );
        #endif

        return 0;
    }

    /* Execute the inner procedure.  Recurse unless instructed otherwise. */
    if ( !( action = p2_procedure__execute( ( state->inner_p ), o ) ) )
    {
        /* Traverse to children (if any). */
        if ( o->type->flags & TYPE__IS_OBJ_COLL )
        {
            o->type->distribute( o->value, state->outer_p );
        }

        /* Traverse to associated objects (if any). */
        #if TRIPLES__GLOBAL__OUT_EDGES
        if ( o->outbound_edges )
        {
printf( "distributing to edges (o = %#x, o->outbound_edges = %#x)\n", ( int ) o, ( int ) o->outbound_edges );
printf( "o->type = %#x\n", ( int ) o->type ); fflush( stdout );
printf( "o->type->name = '%s'\n", o->type->name ); fflush( stdout );
if ( !strcmp( o->type->name, "int" ) )
printf( "value = %i\n", *( ( int* ) o->value ) );
            lookup_table__distribute( o->outbound_edges, state->edge_p );
        }
        #endif
    }

    return 0;
}


void
object__trace( Object *o, p2_procedure *p )
{
    Trace_Ctx state;
    p2_procedure trace_proc;
    p2_procedure edge_p;

    #if DEBUG__SAFE
    if ( !o )
    {
        ERROR( "object__trace: null object" );
        return;
    }
    #endif

    edge_p.execute = ( procedure ) apply_to_assoc_edge;
    edge_p.state = &trace_proc;

    state.outer_p = &trace_proc;
    state.inner_p = p;
    state.edge_p = &edge_p;

    trace_proc.execute = ( procedure ) trace_exec;
    trace_proc.state = &state;

    p2_procedure__execute( ( &trace_proc ), o );
}


static p2_action *
enqueue( Object *o, Array *queue )
{
    array__enqueue( queue, o );
    return 0;
}


/* Note: untested. */
void
object__trace_bfs( Object *o, p2_procedure *p )
{
    Trace_Ctx state;
    p2_procedure trace_proc;
    p2_procedure edge_p;
    p2_procedure outer_p;

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
        p2_procedure__execute( ( &trace_proc ),
            ( Object* ) array__pop( queue ) );
    }
}


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
        ( Object* ) lookup_table__lookup( subj->outbound_edges, pred ) : 0 ;

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
printf( "---o a 1---\n" ); fflush( stdout );
printf( "subj->outbound_edges = %#x\n", ( int ) subj->outbound_edges ); fflush( stdout );

    if ( !subj->outbound_edges
      && !( subj->outbound_edges = lookup_table__new() ) )
        subj = 0;

    else
    {
printf( "---o a 2---\n" ); fflush( stdout );
printf( "subj->outbound_edges = %#x\n", ( int ) subj->outbound_edges ); fflush( stdout );
        if ( obj )
{printf( "---o a 2.1---\n" ); fflush( stdout );
            lookup_table__add( subj->outbound_edges, pred, obj );
}
        else
            lookup_table__remove( subj->outbound_edges, pred );
printf( "---o a 3---\n" ); fflush( stdout );
    }

    #else
printf( "---o a 4---\n" ); fflush( stdout );

    subj = 0;

    #endif

    return subj;
}

#endif  /* TRIPLES__GLOBAL */


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
