/*+
  set.h

  A simplified expanding hash table to store an unordered set of unique
  identifiers, for instance a set of data addresses.  The idea is not to bind the
  set elements with any other data, but merely to indicate their presence in the
  set.  Each function is modelled after a mathematical equivalent:

      P2_set__size(S) :            "magnitude of S"
      P2_set__lookup(S, s) :       "s is an element of S"
      P2_set__add(S, s) :          "S' = S union {s}"
      P2_set__remove(S, s) :       "S' = S minus {s}"
      P2_set__union(S, T) :        "S' = S union T"
      P2_set__intersection(S, T) : "S' = S intersect T"
      P2_set__forall(S, f) :       "for all s in S, f(s)"
      P2_set__exists(S, f) :       "there exists s in S such that f(s)"
      P2_set__subset(S, f) :       "S' = all s in S such that f(s)"

  Note: there is no "complement of" operator... but one could easily be
  constructed using the above functions.

  last edited: 5/17/05

*//*/////////////////////////////////////////////////////////////////////////////

Phase2 version 0.4, Copyright (C) 2005 Joshua Shinavier.

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

Joshua Shinavier
parcour@gmail.com
+1 509 747-6051

 *///////////////////////////////////////////////////////////////////////////////


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
    HASH_TABLE *inbound_edges;
    HASH_TABLE *outbound_edges;
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


