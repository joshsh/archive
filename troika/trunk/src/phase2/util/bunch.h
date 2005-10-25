/**
    \file bunch.h

    \brief Provides the means to keep track of large, disorderly collections of
           items in memory.  Once added to a bunch, items cannot be retrieved
           individually, but are handled en masse by the "for all" "exists" and
           "exclude if" functions, which mimic set operations.

    \author Joshua Shinavier   \n
            parcour@gmail.com  \n
            +1 509 570-6990    \n

*//*////////////////////////////////////////////////////////////////////////////

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

#ifndef BUNCH_H
#define BUNCH_H

#include "array.h"

////////////////////////////////////////////////////////////////////////////////

/** \brief A container for an array of pointers.
    May be completely or partially full. */
typedef struct
{
    /** The number of (void *) cells in the buffer. */
    int size;

    /** The number of cells which contain meaningful data. */
    int filled;

    /** A simple pointer array. */
    void **buffer;

} block;

/** \brief A container for large, unordered bags of references.
    Consists of a P2_array of "blocks" of a preferred size.
    May contain nulls and/or repeat references. */
typedef struct
{
    /** An expanding array of memory blocks. */
    P2_array *blocks;

    /** The intended size of a block. */
    int block_size;

    /** Saves on array lookups. */
    block *last_block;

} P2_bunch;

////////////////////////////////////////////////////////////////////////////////

/** Constructor. */
P2_bunch *P2_bunch__new(int block_size);

/** Copy constructor. */
P2_bunch *P2_bunch__copy(P2_bunch *b);

/** Destructor. */
void P2_bunch__delete(P2_bunch *b);

////////////////////////////////////////////////////////////////////////////////

/** Adds a single item to the bunch. */
void P2_bunch__add(P2_bunch *b, void *p);

/** Adds the contents of one bunch to another. */
void P2_bunch__add_all(P2_bunch *dest, P2_bunch *src);

////////////////////////////////////////////////////////////////////////////////

/** If criterion evaluates to a non-zero value ("true") for each item in the
    bunch, return 1 for "true", else 0 for "false". */
void *P2_bunch__forall(P2_bunch *b, void *(*criterion) (void *));

/** If the criterion evaluates to a non-zero value ("true") for ANY item in the
    bunch, return that value.  Else return 0 for "false". */
void *P2_bunch__exists(P2_bunch *b, void *(*criterion) (void *));

/** Exclude all items from the bunch for which criterion evaluates to a non-zero
    value ("true").
    \return the original bunch, but without the excluded items */
P2_bunch *P2_bunch__exclude_if(P2_bunch *b, void *(*criterion) (void *));

////////////////////////////////////////////////////////////////////////////////

#endif  // BUNCH_H


