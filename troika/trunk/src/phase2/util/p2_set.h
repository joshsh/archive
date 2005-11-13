/**

\file  p2_set.h

\brief  A simple mathematical set utility.

The idea is not to bind set elements with data, but only to indicate their
presence in the set.  Each function mimics a mathematical equivalent:

    p2_set__size(S) :            "magnitude of S"                      \n
    p2_set__lookup(S, s) :       "s is an element of S"                \n
    p2_set__add(S, s) :          "S' = S union {s}"                    \n
    p2_set__remove(S, s) :       "S' = S minus {s}"                    \n
    p2_set__union(S, T) :        "S' = S union T"                      \n
    p2_set__intersection(S, T) : "S' = S intersect T"                  \n
    p2_set__forall(S, f) :       "for all s in S, f(s)"                \n
    p2_set__exists(S, f) :       "there exists s in S such that f(s)"  \n
    p2_set__subset(S, f) :       "S' = all s in S such that f(s)"      \n

\note  There is no "complement of" operator... but one could easily be
constructed using the above functions.

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

#ifndef P2_SET_H
#define P2_SET_H


#include <stdlib.h>

/** \brief Simulates a mathematical set.

    Implemented using hash tables.  Set elements are represented by unique
    32-bit addresses, or equivalently, by 32-bit integers. */
typedef struct _p2_set
{
    /** The number of occupied cells in the hash table. */
    int size;

    /** The number of cells the buffer array. */
    int buffer_size;

    /** The number of occupied cells the buffer can hold before the hash table
        becomes too dense. */
    int capacity;

    /** The ratio of empty cells to occupied cells is to be at least this large.
        A more sparse hash table takes up more memory but is more time-efficient
        because collisions are less frequent. */
    float sparsity;

    /** The hash table expands by this factor whenever it becomes too dense.
        Re-hashing the set into a new buffer is expensive, so beware of setting
        the expansion factor too low. */
    float expansion;

    /** The hash table buffer array. */
    void **buffer;

} p2_set;


////////////////////////////////////////////////////////////////////////////////

/** Constructor.
    \param buffer_size the initial size of the hash table buffer (the actual
    buffer size is the next lowest prime number)
    \param expansion see above.  Will be set to a default if too low.
    \param sparsity see above.  Will be set to a default if too low.
*/
p2_set *p2_set__new(int buffer_size, float expansion, float sparsity);

/** Copy constructor. */
p2_set *p2_set__copy(p2_set *S);

/** Destructor. */
void p2_set__delete(p2_set *S);


////////////////////////////////////////////////////////////////////////////////

/** "magnitude of S" */
int p2_set__size(p2_set *S);

/** "s is an element of S" */
void *p2_set__lookup(p2_set *S, void *s);

/** "S' = S union {s}" */
p2_set *p2_set__add(p2_set *S, void *s);

/** "S' = S minus {s}" */
p2_set *p2_set__remove(p2_set *S, void *s);


////////////////////////////////////////////////////////////////////////////////

/** "S' = S union T"
    \note O(n) time complexity */
p2_set *p2_set__union(p2_set *S, p2_set *T);

/** "S' = S intersect T"
    \note O(n^2) time complexity */
p2_set *p2_set__intersection(p2_set *S, p2_set *T);

/** "for all s in S, f(s)"
    \return 1 if the criterion evaluates to a non-zero value ("true") for each
    item in the bunch, else 0. */
void *p2_set__for_all(p2_set *S, void *(*f)(void *));

/** "there exists s in S such that f(s)"
    \return the first item for which the criterion evaluates to a non-zero
    value ("true").  If no such item exists, the return value is 0. */
void *p2_set__exists(p2_set *S, void *(*f)(void *));

/** "S' = all s in S such that f(s)" */
p2_set *p2_set__subset(p2_set *S, void *(*f)(void *));


////////////////////////////////////////////////////////////////////////////////

/** A wrapper for "free" which always returns a 1 (for use in conjunction with
    p2_set__for_all) */
void *p2_set__free(void *s);


#endif  // P2_SET_H

