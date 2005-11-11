/**

\file  p2_atom.h

\author  Joshua Shinavier   \n
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

#ifdef P2FLAGS__ASSOCIATION
    #include "util/p2_hash_table.h"
#endif


/** The least addressable unit of data in a Phase2 environment. */
typedef struct _p2_atom
{
    /** A positively signed integer indicating the data type of the atom. */
    p2_type type;

    /** A generic pointer which references the atom's data. */
    void *value;

    #ifdef P2FLAGS__ASSOCIATION

        /** Associative edges pointing away from the atom.
            For example, (X, y, z). */
        p2_hash_table *outbound_edges;

        #ifdef P2FLAGS__TRANS_EDGES
            /** Associative edges pointing "through" the atom.
                For example, (x, Y, z). */
            p2_hash_table *trans_edges;
        #endif

        #ifdef P2FLAGS__INBOUND_EDGES
            /** Associative edges pointing towards the atom.
                For example, (x, y, Z). */
            p2_hash_table *inbound_edges;
        #endif

    #endif

} p2_atom;


p2_error p2_atom_init();
p2_error p2_atom_end();

p2_error DESERIALIZATION_FAILED;   //.................

/** Create a new typed constant. */
p2_atom *p2_atom__new(p2_type type, void *value);

#ifndef P2FLAGS__MARK_AND_SWEEP

    /** Deallocate a typed constant. */
    void p2_atom__delete(p2_atom *atom);

#endif

/** Serializes a p2_atom to a string. */
void p2_atom__encode(p2_atom *atom, char *buffer);

/** Deserializes a p2_atom from a string. */
p2_atom *p2_atom__decode(p2_type type_index, char *buffer);


// Association /////////////////////////////////////////////////////////////////

#ifdef P2FLAGS__ASSOCIATION

    p2_atom *p2_multiply(p2_atom *ref, p2_atom *key);

    p2_atom *p2_associate(p2_atom *ref, p2_atom *key, p2_atom *target);

#endif


// "Mark and sweep" memory reclamation /////////////////////////////////////////

#ifdef P2FLAGS__MARK_AND_SWEEP

    //#include "util/p2_term.h"

    /** Marks an atom, and subsequently checks the edge marking queue for other
        atoms which need to be handled. */
    void p2_atom__mark(p2_atom *atom);

    /** Marks all atoms referenced by the given p2_term.
        \note  This function assumes that type identifiers are positive (it
       "marks" an atom by reversing the sign of its type id). */
    //void p2_mark(p2_term *term);

    /** Deallocates all unmarked atoms and unmarks the rest. */
    void p2_sweep();

    /** Returns the number of atoms in the "mark and sweep" collection. */
    unsigned int p2_total_markandsweep_atoms();

#endif  // P2FLAGS__MARK_AND_SWEEP


#endif  // P2_ATOM_H

