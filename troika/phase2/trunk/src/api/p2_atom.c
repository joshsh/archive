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

#include "p2_atom.h"

#include <stdlib.h>  /* malloc */


#ifdef P2FLAGS__MARK_AND_SWEEP

#include "util/p2_bunch.h"
#include "util/p2_term.h"
#include "util/p2_set.h"

/** A global collection of pointers which references all atoms in the
    environment at a given time.  Memory can be reclaimed by eliminating
    orphan atoms with the mark and sweep functions. */
p2_bunch *markandsweep_atoms = 0;

/** Atoms in this queue have already been marked, however they may have outbound
    edges which need to be investigated.
    \note  This collection is always null outside of a mark and sweep interval,
    and is initialized in add_to_queue. */
p2_bunch *edge_marking_queue = 0;

#ifdef P2FLAGS__ASSOCIATION

/** A global lookup table for "trigger" tables.
    \note  This table is is always null outside of a mark and sweep interval,
    and is initialized in mark_edge. */
p2_hash_table *all_triggers = 0;

#endif  // P2FLAGS__ASSOCIATION

#endif  // P2FLAGS__MARK_AND_SWEEP


//!
p2_atom *p2_error_atom(p2_error err) { return 0 ; }
p2_type p2_term__type = (p2_type) 655321;
p2_type p2_set__type = (p2_type) 655322;


p2_error p2_atom_init()
{
    #ifdef P2FLAGS__MARK_AND_SWEEP
        // Initialize "mark and sweep" collection.
        markandsweep_atoms = p2_bunch__new(P2FLAGS__MARKANDSWEEP_ATOMS__BLOCK_SIZE);
    #endif

    return P2_SUCCESS;
}



p2_error p2_atom_end()
{
    #ifdef P2FLAGS__MARK_AND_SWEEP
    p2_sweep();

    if (markandsweep_atoms)
    {
        p2_bunch__delete(markandsweep_atoms);
        markandsweep_atoms = 0;
    }
    #endif

    return P2_SUCCESS;
}



p2_atom *p2_atom__new(p2_type type, void *value)
{
    p2_atom *atom = (p2_atom *) malloc(sizeof(p2_atom));
    atom->type = type;
    atom->value = value;

    #ifdef P2FLAGS__ASSOCIATION

        atom->outbound_edges = 0;

        #ifdef P2FLAGS__INBOUND_EDGES
            atom->inbound_edges = 0;
        #endif

        #ifdef P2FLAGS__TRANS_EDGES
            atom->trans_edges = 0;
        #endif

    #endif

    #ifdef P2FLAGS__MARK_AND_SWEEP
        p2_bunch__add(markandsweep_atoms, (void *) atom);
    #endif
}



void p2_atom__delete(p2_atom *atom)
{
    #ifdef P2FLAGS__ASSOCIATION

        if (atom->outbound_edges)
            p2_hash_table__delete(atom->outbound_edges);

        #ifdef P2FLAGS__INBOUND_EDGES
            if (atom->inbound_edges)
                p2_hash_table__delete(atom->inbound_edges);
        #endif

        #ifdef P2FLAGS__TRANS_EDGES
            if (atom->trans_edges)
                p2_hash_table__delete(atom->trans_edges);
        #endif

    #endif

    // Caution: assumes that the atom owns its value.
    p2_destroy(atom->value, atom->type);

    free(atom);
}



void p2_atom__encode(p2_atom *atom, char *buffer)
{
    p2_encode(atom->value, atom->type, buffer);
}



p2_atom *p2_atom__decode(p2_type type_index, char *buffer)
{
    void *value = p2_decode(type_index, buffer);
    p2_atom *atom;

    if (!value)
        atom = p2_error_atom(DESERIALIZATION_FAILED);  //.................

    else
        atom = p2_atom__new(type_index, value);

    return atom;
}



// Association /////////////////////////////////////////////////////////////////

#ifdef P2FLAGS__ASSOCIATION

p2_atom *p2_multiply(p2_atom *ref, p2_atom *key)
{
    return ref->outbound_edges ?
        p2_hash_table__lookup(ref->outbound_edges, key) : 0 ;
}


// Note: doesn't take association sets into account yet.
p2_atom *p2_associate(p2_atom *ref, p2_atom *key, p2_atom *target)
{
    if (ref->outbound_edges)
    {
        if (target)
            p2_hash_table__add(ref->outbound_edges, key, target);
        else
            p2_hash_table__remove(ref->outbound_edges, key);
    }

    // If the outbound edges hash table does not exist, create it, provided
    // that we have a non-null value to map to (the null mapping is implicit).
    else if (target)
    {
        ref->outbound_edges = p2_hash_table__new(
            P2FLAGS__ATOM__INIT_ASSOC_BUFFER_SIZE, 0, 0, ADDRESS_DEFAULTS);

        p2_hash_table__add(ref->outbound_edges, key, target);
    }

    return ref;
}

#endif  // P2FLAGS__ASSOCIATION


// "Mark and sweep" memory reclamation /////////////////////////////////////////


#ifdef P2FLAGS__MARK_AND_SWEEP


unsigned int p2_total_markandsweep_atoms()
{
   if (markandsweep_atoms)
        return p2_bunch__size(markandsweep_atoms);
   else
        return 0;
}


//void *(*)(void *, void *)

/** \return  a type-specific "apply to all" function, or 0 if the argument is
    not of a collection type. */
void *(*distributing_function (p2_type type))(void *, void *(*)(void *))
{
    if (type == p2_term__type)
        return (void *(*)(void *, void *(*)(void *))) p2_term__for_all;
    else if (type == p2_set__type)
        return (void *(*)(void *, void *(*)(void *))) p2_set__for_all;
    else
        return 0;
}


/** Adds an atom to the edge-marking queue. */
void add_to_queue(p2_atom *atom)
{
    if (!edge_marking_queue)
        edge_marking_queue = p2_bunch__new(P2FLAGS__MARKANDSWEEP_QUEUE__BLOCK_SIZE);

    p2_bunch__add(edge_marking_queue, atom);
}


void *mark_atom(p2_atom *atom)
{
    // If the atom has not already been marked...
    if (atom->type > 0)
    {
        // Mark the atom by reversing the sign of its type.
        atom->type = (p2_type) -((unsigned int) atom->type) ;

        // Before interfering with the atom's type...
        #ifdef P2FLAGS__ASSOCIATION
            // This atom may have edges which need to be investigated.
            add_to_queue(atom) ;

            // Flip any "triggers" bound to this atom.
            p2_hash_table *triggers = p2_hash_table__lookup(all_triggers, atom);
            if (triggers)
            {
                // Mark each atom in the argument atom's trigger table.
                p2_hash_table__for_all(triggers, (void (*)(void *, void *)) mark_atom);

                // The marked atom has no more need for a trigger table.
                p2_hash_table__remove(all_triggers, triggers);
                p2_hash_table__delete(triggers);
            }
        #else
            // If there are no associative edges, then only parent-child edges
            // are possible.
            if (distributing_function(-atom->type))
                add_to_queue(atom) ;
        #endif
    }

    // Perfunctory nonzero return value.
    return (void *) 1;
}


#ifdef P2FLAGS__ASSOCIATION

void mark_edge(p2_atom *key, p2_atom *target)
{
    // If the target has not already been marked...
    if (target->type > 0)
    {
        // If the key is already marked, then we can immediately mark the target.
        if (key->type < 0)
            mark_atom(target) ;

        // Otherwise, set up a "trigger" which will cause the target atom to be
        // marked in the event that the key is marked.
        else
        {
            // Initialize the trigger table lookup table if necessary.
            if (!all_triggers)
                all_triggers = p2_hash_table__new(
                    P2FLAGS__INIT_MARKANDSWEEP_BUFFER_SIZE, 0, 0, ADDRESS_DEFAULTS);

            // Find or create the appropriate trigger table.
            p2_hash_table *triggers = p2_hash_table__lookup(all_triggers, key);
            if (!triggers)
            {
                triggers = p2_hash_table__new(
                    P2FLAGS__INIT_TRIGGER_BUFFER_SIZE, 0, 0, ADDRESS_DEFAULTS);

                p2_hash_table__add(all_triggers, key, triggers);
            }

            // Add a trigger to the table.
            p2_hash_table__add(all_triggers, triggers, target);
        }
    }
}

#endif  // P2FLAGS__ASSOCIATION


/** Marks all of an atom's outbound edges. */
void mark_outbound_edges(p2_atom *atom)
{
    #ifdef P2FLAGS__ASSOCIATION

        // If the atom has any outbound associations...
        if (atom->outbound_edges)
        {
            // Investigate each outbound associative edge.
            p2_hash_table__for_all(atom->outbound_edges,
                (void (*)(void *, void *)) mark_edge) ;
        }

    #endif

    // Note: this logic assumes that the argument atom has already been marked.
    void *(*for_all)(void *, void *(*)(void *))
        = distributing_function((p2_type) -((unsigned int) atom->type)) ;

    // If the argument atom is of a collection type, mark each child atom.
    if (for_all)
        for_all(atom->value, (void *(*)(void *)) mark_atom) ;
}


void p2_atom__mark(p2_atom *atom)
{
    mark_atom(atom) ;

    if (edge_marking_queue)
    {
        while (edge_marking_queue->last_block->filled)  //~?
        {
            atom = (p2_atom *) p2_bunch__remove(edge_marking_queue) ;
            mark_outbound_edges(atom) ;
        }
    }
}


p2_atom *unmark(p2_atom *atom)
{
    if (atom->type < 0)
    {
        atom->type = (p2_type) -((unsigned int) atom->type);

        // Don't exclude this atom.
        atom = 0;
    }
    else
        p2_atom__delete(atom);

    return atom;
}


void p2_sweep()
{
    // Unmark all marked atoms in the environment, and delete the rest.
    markandsweep_atoms = p2_bunch__exclude_if(markandsweep_atoms, (void *(*)(void *)) unmark);

    // Eliminate the edge marking queue.
    if (edge_marking_queue)
    {
        p2_bunch__delete(edge_marking_queue);
        edge_marking_queue = 0;
    }

    #ifdef P2FLAGS__ASSOCIATION

        if (all_triggers)
        {
            // Delete all remaining trigger tables.
            p2_hash_table__for_all_targets(
                all_triggers,
                (void (*)(void *)) p2_hash_table__delete);

            // Eliminate the global lookup table.
            p2_hash_table__delete(all_triggers);
            all_triggers = 0;
        }

    #endif
}


#endif  // P2FLAGS__MARK_AND_SWEEP

