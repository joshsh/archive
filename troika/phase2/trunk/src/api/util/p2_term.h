/**

\file  p2_term.h

\brief  A simple nested sequence utility.

Terms are represented by arrays of 4-byte cells, each of which contains either
a reference to an "atom" or an integer i indicating the head of a sub-term with
a length of i cells.  For instance, \n
                                                  \n
    "a"  <-->  2 a                                \n
                                                  \n
    "a b"  <-->  5 2 a 2 b                        \n
                                                  \n
    "a (b c)"  <-->  8 2 a 5 2 b 2 c              \n

The term array occupies the higher-address portion of an expanding buffer and
grows towards the lower-address end.

\note  This library assumes that sizeof(void *) == sizeof(unsigned int).

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

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

#ifndef P2_TERM_H
#define P2_TERM_H


/** By default, the size of a term's buffer will be multiplied by this factor
    when the term outgrows it. */
#define TERM__DEFAULT_EXPANSION    2.0


/** \brief A data structure to represent and manipulate nested sequences.
    The implementation is not recursive.

    \note An empty p2_term has no data; it is represented by the NULL.
 */
typedef struct _p2_term
{
    /** Number of cells the buffer can hold. */
    unsigned int buffer_size;

    /** An array of 4-byte cells containing both generic pointers ("atoms") and
        unsigned int values (each one indicates the size of the sub-term in
        which it is the first cell). */
    void **buffer;

    /** Position of the first cell of the p2_term representation in the buffer.
        It always contains an unsigned int value indicating the overall size of
        the p2_term. */
    void **head;
} p2_term;


/** \note Needed by sk.c. */
p2_term *p2_term__expand(p2_term *t, unsigned int minimum_buffer_size);


/* Constructors and destructor ************************************************/

/** Creates a new p2_term containing a single atom. */
p2_term *p2_term__new(void *p, unsigned int initial_buffer_size);

/** Copy constructor. */
p2_term *p2_term__copy(p2_term *t);

/** Destructor. */
void p2_term__delete(p2_term *t);


/* Accessors ******************************************************************/

/** \return  the logical length of the term, i.e. the number of sub-terms it
    contains.  Not to be confused with the number of cells required to represent
    the term (that's in p2_term->head) or its physical size in memory
    (*(p2_term->head) * sizeof(void *)).
    \warning  O(n) time overhead to count n sub-terms. */
unsigned int p2_term__length(p2_term *t);

/** \return  a new p2_term representing the subterm at a given index */
/** \warning  For the sake of efficiency, there is no error checking.  Make sure
    your index is in range by first calling p2_term__length. */
p2_term *p2_term__subterm_at(p2_term *t, int index);

/** Defines a new expansion factor for terms. When the term outgrows its buffer,
    the new buffer will be this much larger. */
void p2_term__set_expansion_factor(float expansion_factor);


/* Normalizing functions ******************************************************/


/* Merge functions ************************************************************/

/** ((A1 ... Am) (B1 ... Bn))    simple merge */
p2_term *p2_term__merge(p2_term *t1, p2_term *t2);

/** (A1 ... Am (B1 ... Bn))      left-associative merge */
p2_term *p2_term__merge_la(p2_term *t1, p2_term *t2);

/** ((A1 ... Am) B1 ... Bn)      right-associative merge */
p2_term *p2_term__merge_ra(p2_term *t1, p2_term *t2);

/** (A1 ... Am B1 ... Bn)        concatenation */
p2_term *p2_term__cat(p2_term *t1, p2_term *t2);


/* Logical set functions and item substitution ********************************/

/** \return 1 if the criterion evaluates to a non-zero value ("true") for each
    item in the term, else 0. */
void *p2_term__for_all(p2_term *t, void *(*criterion)(void *));

/** \return the first item for which the criterion evaluates to a non-zero
    value ("true").  If no such item exists, the return value is 0.
    \warning output may not be meaningful if the p2_term contains NULL atoms. */
void *p2_term__exists(p2_term *t, void *(*criterion)(void *));

/** Replaces each item in the term with a new item determined by substitution.
    \note there is no special return value to indicate substitution failure. */
p2_term *p2_term__substitute_all(p2_term *t, void *(*substitution)(void *));


#endif  /* P2_TERM_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
