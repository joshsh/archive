#ifndef P2_ATOM_H
#define P2_ATOM_H


#include "p2_type.h"

#ifdef USE_ASSOCIATION
#include "util/hash_table.h"
#endif


typedef struct
{
    // A positively signed integer indicating the data type of the atom.
    P2_type type;

    // A generic pointer referencing the atom's data.
    void *value;

    #ifdef USE_ASSOCIATION
    P2_hash_table *inbound_edges;
    P2_hash_table *outbound_edges;
    #endif
} P2_atom;


P2_error P2_atom_init();
P2_error P2_atom_end();

P2_atom *P2_atom__new(P2_type type, void *value);
void P2_atom__delete(P2_atom *atom);


/** "Mark and sweep" memory reclamation *////////////////////////////////////////

#ifdef MARK_AND_SWEEP

#include "util/term.h"

/**
 * Marks all atoms referenced by the given P2_term.
 */
void P2_mark(P2_term *term);

/**
 * Deallocates all unmarked atoms and unmarks the rest.
 */
void P2_sweep();

/**
 * Returns the number of atoms in the "mark and sweep" buffer.
 */
unsigned int P2_total_markandsweep_atoms();

#endif


#endif  // P2_ATOM_H

