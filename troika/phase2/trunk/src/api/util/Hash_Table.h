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


#include <defs.h>


typedef unsigned int ( *hash_f )( const void *key );


/******************************************************************************/

/** \brief An expanding hash table to bind pairs of generic pointers. */
typedef struct Hash_Table Hash_Table;


/******************************************************************************/

/** Constructor.
    \param buffer_size the initial size of the hash table buffer (the actual
    buffer size is the next lowest prime number)
    \param expansion  the hash table expands by this factor whenever it becomes
    too dense.  Re-hashing the into a new buffer is expensive, so beware of
    setting the expansion factor too low.  Will be set to a default if
    absolutely too low.
    \param load  the ratio of occupied cells to total cells is to be at
    most this large. A more sparse hash table takes up more memory but is more
    time-efficient because collisions are less frequent. Will be set to a
    default if zero or otherwise out of range.
    \param hashing_function a hashing function specific to the table's "key"
    type.  The hashing function should accept a single void pointer and
    return an int, the idea being to distribute values over a broad stretch of the
    32-bit range.
    \param compare_to a comparison function for key values.  The comparison
    function should accept two void pointers a, b as arguments and return an int
    which is &lt; 0 when a &lt; b, &gt; 0 when a &gt; b, and 0 when a == b. */
extern Hash_Table *
hash_table__new(
  unsigned int buffer_size,
  double expansion,
  double load,
  hash_f hash,
  Comparator compare );

/** Copy constructor. */
extern Hash_Table *
hash_table__copy( const Hash_Table *h );

/** Destructor. */
extern void
hash_table__delete( Hash_Table *h );


/******************************************************************************/

/** \return  the number of occupied cells in the hash table. */
extern unsigned int
hash_table__size( const Hash_Table *h );

extern double
hash_table__load( const Hash_Table *h );

extern double
hash_table__expansion( const Hash_Table *h );


/******************************************************************************/

/** Adds an key.
    \param key  the key to add
    \return  the displaced target (if any) */
extern void *
hash_table__add( Hash_Table *h, void *key );

/** Looks up a key to obtain a target.
    \warning returns 0 if an key is not found.  Beware of storing a 0 as a
    target value, else you won't be able to tell it apart from a failed lookup.
*/
extern void *
hash_table__lookup( const Hash_Table *h, const void *key );

/** Removes the key.
    \return  the displaced target (if any) */
extern void *
hash_table__remove( Hash_Table *h, const void *key );


extern void
hash_table__walk( Hash_Table *h, Dist_f f );


/******************************************************************************/

#endif  /* HASH_TABLE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
