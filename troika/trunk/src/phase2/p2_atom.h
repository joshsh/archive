/**
    \file p2_atom.h

    \author Joshua Shinavier   \n
            parcour@gmail.com  \n
            +1 509 570-6990    \n */

/*//////////////////////////////////////////////////////////////////////////////

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

*///////////////////////////////////////////////////////////////////////////////

#ifndef P2_ATOM_H
#define P2_ATOM_H


#include "p2_flags.h"
#include "p2_type.h"

#ifdef P2DEF_ASSOCIATION
    #include "util/hash_table.h"
#endif


/** The least addressable unit of data in a Phase2 environment. */
typedef struct
{
    /** A positively signed integer indicating the data type of the atom. */
    P2_type type;

    /** A generic pointer which references the atom's data. */
    void *value;

    #ifdef P2DEF_ASSOCIATION

        #ifdef P2DEF_OUTBOUND_EDGES
            /** Associative edges pointing away from the atom.
                For example, (X, y, z). */
            P2_hash_table *outbound_edges;
        #endif

        #ifdef P2DEF_TRANS_EDGES
            /** Associative edges pointing "through" the atom.
                For example, (x, Y, z). */
            P2_hash_table *trans_edges;
        #endif

        #ifdef P2DEF_INBOUND_EDGES
            /** Associative edges pointing towards the atom.
                For example, (x, y, Z). */
            P2_hash_table *inbound_edges;
        #endif

    #endif

} P2_atom;


P2_error P2_atom_init();
P2_error P2_atom_end();

P2_atom *P2_atom__new(P2_type type, void *value);
void P2_atom__delete(P2_atom *atom);


// "Mark and sweep" memory reclamation /////////////////////////////////////////

#ifdef P2DEF_MARK_AND_SWEEP

    #include "util/term.h"

    /** Marks all atoms referenced by the given P2_term.
        \note  this function assumes that type identifiers are positive (it
       "marks" an atom by reversing the sign of its type id). */
    void P2_mark(P2_term *term);

    /** Deallocates all unmarked atoms and unmarks the rest. */
    void P2_sweep();

    /** Returns the number of atoms in the "mark and sweep" collection. */
    unsigned int P2_total_markandsweep_atoms();

#endif


#endif  // P2_ATOM_H

