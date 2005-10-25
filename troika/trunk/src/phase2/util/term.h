/**
    \file term.h

    \brief A simple nested sequence utility.

    \note This program assumes that sizeof(void *) == sizeof(unsigned int).

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

#ifndef TERM_H
#define TERM_H


/** By default, the size of a term's buffer will be multiplied by this factor
    when the term outgrows it. */
#define TERM__DEFAULT_EXPANSION    2.0


/** \brief A data structure to represent and manipulate nested sequences.
    The implementation is not recursive.

    \note An empty P2_term has no data; it is represented by the NULL.
 */
typedef struct
{
    /** Number of cells the buffer can hold. */
    unsigned int buffer_size;

    /** An array of 4-byte cells containing both generic pointers ("atoms") and
        unsigned int values (each one indicates the size of the sub-term in
        which it is the first cell). */
    void **buffer;

    /** Position of the first cell of the P2_term representation in the buffer.
        It always contains an unsigned int value indicating the overall size of
        the P2_term. */
    void **head;
} P2_term;


/** \note Needed by sk.c. */
P2_term *P2_term__expand(P2_term *term, unsigned int minimum_buffer_size);


// Constructors and destructor /////////////////////////////////////////////////

/** Creates a new P2_term containing a single atom. */
P2_term *P2_term__new(void *p, unsigned int initial_buffer_size);

/** Copy constructor. */
P2_term *P2_term__copy(P2_term *source);

/** Destructor. */
void P2_term__delete(P2_term *P2_term);


// Accessors ///////////////////////////////////////////////////////////////////

/** \return the logical length of the term, i.e. the number of sub-terms it
    contains.  Not to be confused with the number of cells required to represent
    the term (that's in P2_term->head) or its physical size in memory
    (*(P2_term->head) * sizeof(void *)).
    \warning O(n) time overhead to count n sub-terms. */
unsigned int P2_term__length(P2_term *P2_term);

/** Defines a new expansion factor for terms. When the term outgrows its buffer,
    the new buffer will be this much larger. */
void P2_term__set_expansion_factor(float expansion_factor);


// Normalizing functions ///////////////////////////////////////////////////////


// Merge functions /////////////////////////////////////////////////////////////

/** ((A1 ... Am) (B1 ... Bn))    simple merge */
P2_term *P2_term__merge(P2_term *t1, P2_term *t2);

/** (A1 ... Am (B1 ... Bn))      left-associative merge */
P2_term *P2_term__merge_la(P2_term *t1, P2_term *t2);

/** ((A1 ... Am) B1 ... Bn)      right-associative merge */
P2_term *P2_term__merge_ra(P2_term *t1, P2_term *t2);

/** (A1 ... Am B1 ... Bn)        concatenation */
P2_term *P2_term__cat(P2_term *t1, P2_term *t2);


// Logical set functions and atom substitution /////////////////////////////////

/** \return 1 if the criterion evaluates to a non-zero value ("true") for each
    item in the term, else 0. */
void *P2_term__for_all(P2_term *P2_term, void *(*criterion)(void *));

/** \return the first item for which the criterion evaluates to a non-zero
    value ("true").  If no such item exists, the return value is 0.
    \warning output may not be meaningful if the P2_term contains NULL atoms. */
void *P2_term__exists(P2_term *P2_term, void *(*criterion)(void *));

/** Replaces each item in the term with a new item determined by substitution.
    \note there is no special return value to indicate substitution failure. */
P2_term *P2_term__substitute_all(P2_term *P2_term, void *(*substitution)(void *));


#endif  // TERM_H

