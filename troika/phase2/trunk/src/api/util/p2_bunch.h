/**
    \file  p2_bunch.h

    \brief  A utility to keep track of large, unordered collections of pointers.

    Once added to a bunch, items cannot be retrieved individually, but are
    handled en masse via the "for all" "exists" and "exclude if" functions, which
    mimic set operations.

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

#ifndef P2_BUNCH_H
#define P2_BUNCH_H

#include "p2_array.h"


/******************************************************************************/

/** \brief A container for an array of pointers.
    May be completely or partially full. */
typedef struct _block
{
    /** The number of (void *) cells in the buffer. */
    unsigned int size;

    /** The number of cells which contain meaningful data. */
    unsigned int filled;

    /** A simple pointer array. */
    void **buffer;

} block;


/** \brief A container for large, unordered bags of references.
    Consists of a p2_array of "blocks" of a preferred size.
    May contain nulls and/or repeat references. */
typedef struct _p2_bunch
{
    /** An expanding array of memory blocks. */
    p2_array *blocks;

    /** The intended size of a block. */
    unsigned int block_size;

    /** Saves on array lookups. */
    block *last_block;

} p2_bunch;


/******************************************************************************/

/** Constructor. */
p2_bunch *p2_bunch__new(unsigned int block_size);

/** Copy constructor. */
p2_bunch *p2_bunch__copy(p2_bunch *b);

/** Destructor. */
void p2_bunch__delete(p2_bunch *b);


/******************************************************************************/

/** How big of a bunch is it? */
unsigned int p2_bunch__size(p2_bunch *b);


/******************************************************************************/

/** Adds a single item to the bunch. */
void *p2_bunch__add(p2_bunch *b, void *p);

/** Adds the contents of one bunch to another. */
void p2_bunch__add_all(p2_bunch *dest, p2_bunch *src);

/** Removes a single item from the bunch. */
void *p2_bunch__remove(p2_bunch *b);


/******************************************************************************/

void p2_bunch__distribute( p2_bunch *b, p2_procedure *p );


/******************************************************************************/

p2_type *p2_bunch__type( const char *name, int flags );


#endif  /* P2_BUNCH_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */

