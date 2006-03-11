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

#include "p2_object.h"


p2_object *p2_object__new( p2_type *type, void *value, int flags )
{
    p2_object *o;

    #if DEBUG__SAFE
    if ( !type || !value)
    {
        ERROR( "p2_object__new: null type or value" );
        return 0;
    }
    #endif

    o = new( p2_object );

    if ( !o )
        return 0;

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
    printf( "p2_object__new: created object %#x (value %#x) of type '%s' (%#x).\n",
        ( int ) o, ( int ) o->value, o->type->name, ( int ) o->type );
if (!strcmp( o->type->name, "type"))
printf( "    This is type '%s'.\n", ( ( p2_type* ) o->value )->name );
    #endif

    return o;
}


static p2_action *delete_proc( void *p, p2_type *type )
{
    type->destroy( p );
    return 0;
}


void p2_object__delete( p2_object *o )
{
    p2_procedure p;

    #if DEBUG__SAFE
    if ( !o )
    {
        ERROR( "p2_object__delete: null object" );
        return;
    }
    #endif

    #if DEBUG__OBJECT
    printf( "p2_object__delete(%#x): value = %#x, type = %#x.\n",
        ( int ) o, ( int ) o->value, ( int ) o->type );
    #endif

    #if TRIPLES__GLOBAL__IN_EDGES
    if ( o->inbound_edges )
        p2_lookup_table__delete( o->inbound_edges );
    #endif

    #if TRIPLES__GLOBAL__OUT_EDGES
    if ( o->outbound_edges )
        p2_lookup_table__delete( o->outbound_edges );
    #endif

    #if TRIPLES__GLOBAL__TRANS_EDGES
    if ( o->trans_edges )
        p2_lookup_table__delete( o->trans_edges );
    #endif

    /* If the object owns its children (and has any), free them. */
    if ( o->flags & OBJECT__OWNS_DESCENDANTS )
    {
        p.execute = ( procedure ) delete_proc;
        p.state = o->type->type_arg;
        o->type->distribute( o->value, &p );
    }

    /* Free the object's data. */
    o->type->destroy( o->value );

    free( o );
}


/* Graph traversal ************************************************************/


typedef struct _trace_proc_st
{
    p2_procedure *outer_p;
    p2_procedure *inner_p;
    p2_procedure *edge_p;

} trace_proc_st;


static p2_action * apply_to_assoc_edge
    ( p2_lookup_table__entry *entry, p2_procedure *p )
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


static p2_action * trace_exec( p2_object *o, trace_proc_st *state )
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
        if ( o->flags & OBJECT__IS_OBJ_COLL )
        {
            o->type->distribute( o->value, state->outer_p );
        }

        /* Traverse to associated objects (if any). */
        #if TRIPLES__GLOBAL__OUT_EDGES
        if ( o->outbound_edges )
        {
            p2_lookup_table__distribute( o->outbound_edges, state->edge_p );
        }
        #endif
    }

    return 0;
}


void p2_object__trace( p2_object *o, p2_procedure *p )
{
    trace_proc_st state;
    p2_procedure trace_proc;
    p2_procedure edge_p;

    #if DEBUG__SAFE
    if ( !o )
    {
        ERROR( "p2_object__trace: null object" );
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


/* Association ****************************************************************/


#if TRIPLES__GLOBAL

p2_object *p2_object__multiply
    ( p2_object *subj, p2_object *pred )
{
    #if DEBUG__SAFE
    if ( !subj || !pred )
    {
        ERROR( "p2_object__multiply: null argument" );
        return 0;
    }
    #endif

    #if TRIPLES__GLOBAL__OUT_EDGES

    return ( subj->outbound_edges ) ?
        ( p2_object* ) p2_lookup_table__lookup( subj->outbound_edges, pred ) : 0 ;

    #else

    return 0;

    #endif
}


/* Note: doesn't take association sets into account yet. */
p2_object *p2_object__associate
    ( p2_object *subj, p2_object *pred, p2_object *obj )
{
    #if DEBUG__SAFE
    if ( !subj || !pred || !obj )
    {
        ERROR( "p2_object__associate: null argument" );
        return 0;
    }
    #endif

    #if TRIPLES__GLOBAL__OUT_EDGES

    if ( !subj->outbound_edges
      && !( subj->outbound_edges = p2_lookup_table__new() ) )
        return 0;

    if ( obj )
        p2_lookup_table__add( subj->outbound_edges, pred, obj );
    else
        p2_lookup_table__remove( subj->outbound_edges, pred );

    return subj;

    #else

    return 0;

    #endif
}

#endif  /* TRIPLES__GLOBAL */


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
