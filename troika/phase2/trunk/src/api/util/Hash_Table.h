/**

\file  Hash_Table.h

\brief  A simple 32-bit hashing utility.
The hashing and comparison functions used by the table are customizable.

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

#ifndef HASH_TABLE_H
#define HASH_TABLE_H


#include <p2.h>


typedef unsigned int ( *hash_f )( const void *key );


/******************************************************************************/

typedef struct Hash_Table Hash_Table;

/** \brief An expanding hash table to bind pairs of generic pointers. */
struct Hash_Table
{
    /** The number of occupied cells in the hash table. */
    unsigned int size;

    /** The number of cells the buffer array. */
    unsigned int buffer_size;

    /** The number of occupied cells the buffer can hold before the hash table
        becomes too dense. */
    unsigned int capacity;

    /** The ratio of empty cells to occupied cells is to be at least this large.
        A more sparse hash table takes up more memory but is more time-efficient
        because collisions are less frequent. */
    unsigned int sparsity;

    /** The hash table expands by this factor whenever it becomes too dense.
        Re-hashing the into a new buffer is expensive, so beware of setting
        the expansion factor too low. */
    unsigned int expansion;

    /** The hash table buffer array. */
    void **buffer;

    /** A hashing function specific to the table's "key" type. */
    hash_f hash;

    /** A comparison function for key values. */
    comparator compare;
};


/******************************************************************************/

/** Constructor.

    \param buffer_size the initial size of the hash table buffer (the actual
    buffer size is the next lowest prime number)
    \param expansion see above.  Will be set to a default if too low.
    \param sparsity see above.  Will be set to a default if too low.
    \param hashing_function a hashing function specific to the table's "key"
    type.  The hashing function should accept a single void pointer and
    return an int, the idea being to distribute values over a broad stretch of the
    32-bit range.
    \param compare_to a comparison function for key values.  The comparison
    function should accept two void pointers a, b as arguments and return an int
    which is &lt; 0 when a &lt; b, &gt; 0 when a &gt; b, and 0 when a == b. */
extern Hash_Table *hash_table__new(
  unsigned int buffer_size,
  unsigned int expansion,
  unsigned int sparsity,
  hash_f hash,
  comparator compare );

/** Copy constructor. */
extern Hash_Table *hash_table__copy( const Hash_Table *h );

/** Destructor. */
extern void hash_table__delete( Hash_Table *h );


/******************************************************************************/

/** Adds an key.
    \param key  the key to add
    \return  the displaced target (if any) */
extern void *hash_table__add( Hash_Table *h, void *key );

/** Looks up a key to obtain a target.
    \warning returns 0 if an key is not found.  Beware of storing a 0 as a
    target value, else you won't be able to tell it apart from a failed lookup.
*/
extern void *hash_table__lookup( Hash_Table *h, const void *key );

/** Removes the key.
    \return  the displaced target (if any) */
extern void *hash_table__remove( Hash_Table *h, const void *key );


extern void hash_table__distribute( Hash_Table *h, p2_procedure *p );


/******************************************************************************/

#endif  /* HASH_TABLE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
