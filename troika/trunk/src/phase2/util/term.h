/*+
  P2_term.h

  An empty P2_term has no data; it is represented by the NULL.

  Note: this program assumes that sizeof(void *) == sizeof(unsigned int).

  last edited: 9/5/05

*//*/////////////////////////////////////////////////////////////////////////////

Phase2 version 0.4, Copyright (C) 2005 Joshua Shinavier.

This program is free software; you can redistribute it and/or modify it under
the P2_terms of the GNU General Public License as published by the Free Software
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

#ifndef TERM_H
#define TERM_H

#define TERM__DEFAULT_EXPANSION    2.0



typedef struct
{
    // Number of cells the buffer can hold.
    unsigned int buffer_size;

    // An array of 4-byte cells containing both generic pointers ("atoms") and
    // unsigned int values (each one indicates the size of the sub-term in
    // which it is the first cell).
    void **buffer;

    // Position of the first cell of the P2_term representation in the buffer.
    // It always contains an unsigned int value indicating the overall size of
    // the P2_term.
    void **head;
} P2_term;



// Needed by sk.c.
P2_term *P2_term__expand(P2_term *term, unsigned int minimum_buffer_size);

/** Constructors and destructor *///////////////////////////////////////////////

/**
 * Create a new P2_term containing a single atom.
 */
P2_term *P2_term__new(void *p, unsigned int initial_buffer_size);

/**
 * Copy constructor.
 */
P2_term *P2_term__copy(P2_term *source);

void P2_term__delete(P2_term *P2_term);

/** Accessors */////////////////////////////////////////////////////////////////

/**
 * Logical length of the P2_term, i.e. the number of sub-P2_terms it contains.  Not to
 * be confused with the number of cells required to represent the P2_term (that's in
 * P2_term->head) or its physical size in memory (*(P2_term->head) * sizeof(void *)).
 * Caution: O(n) time overhead to count n sub-P2_terms.
 */
unsigned int P2_term__length(P2_term *P2_term);

void P2_term__set_expansion_factor(float expansion_factor);

/** Normalizing functions *//////////////////////////////////////////////////////

/** Merge functions *////////////////////////////////////////////////////////////

/**
 * ((A1 ... Am) (B1 ... Bn))    simple merge
 */
P2_term *P2_term__merge(P2_term *t1, P2_term *t2);

/**
 * (A1 ... Am (B1 ... Bn))      left-associative merge
 */
P2_term *P2_term__merge_la(P2_term *t1, P2_term *t2);

/**
 * ((A1 ... Am) B1 ... Bn)      right-associative merge
 */
P2_term *P2_term__merge_ra(P2_term *t1, P2_term *t2);

/**
 * (A1 ... Am B1 ... Bn)        concatenation
 */
P2_term *P2_term__cat(P2_term *t1, P2_term *t2);

/** Logical set functions and atom substitution *////////////////////////////////

void *P2_term__for_all(P2_term *P2_term, void *(*criterion)(void *));

// Caution: output may not be meaningful if the P2_term contains NULL atoms.
void *P2_term__exists(P2_term *P2_term, void *(*criterion)(void *));

/**
  Note: unlike the equivalent sequence__replace_atoms, there is no return value
  to indicate substitution failure.
*/
P2_term *P2_term__substitute_all(P2_term *P2_term, void *(*substitution)(void *));



#endif  // TERM_H

/*- end of file -*/


