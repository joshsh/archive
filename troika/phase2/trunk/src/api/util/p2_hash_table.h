/**

\file  p2_hash_table.h

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

#ifndef P2_HASH_TABLE_H
#define P2_HASH_TABLE_H


#include "../p2.h"


/******************************************************************************/

/** Default address comparison function. */
int compare_addresses(const void *key1, const void *key2);

/** Default address hashing function. */
unsigned int hash_address(const void *key);

/** Overrides the function pointer arguments in p2_hash_table__new. */
#define ADDRESS_DEFAULTS hash_address, compare_addresses

/** Default string comparison function. */
int compare_strings(const char *key1, const char *key2);

/** Default string-hashing function. */
unsigned int hash_string(const char *key);

/** Overrides the function pointer arguments in p2_hash_table__new. */
#define STRING_DEFAULTS ( unsigned int (*)(const void*) ) hash_string, \
    ( int (*)(const void*, const void*) ) compare_strings


/******************************************************************************/

/** \brief An expanding hash table to bind pairs of generic pointers. */
typedef struct _p2_hash_table
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
    float sparsity;

    /** The hash table expands by this factor whenever it becomes too dense.
        Re-hashing the into a new buffer is expensive, so beware of setting
        the expansion factor too low. */
    float expansion;

    /** The hash table buffer array. */
    void **buffer;

    /** A hashing function specific to the table's "key" type. */
    unsigned int (*hashing_function) (const void *key);

    /** A comparison function for key values. */
    int (*compare_to) (const void *key1, const void *key2);

} p2_hash_table;

typedef struct _p2_hashing_pair
{
    void *key;
    void *target;
} p2_hashing_pair;


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
    which is &lt; 0 when a &lt; b, &gt; 0 when a &gt; b, and 0 when a == b.
    \note You can replace the function pointer parameters with either of the
    macros ADDRESS_DEFAULTS or STRING_DEFAULTS (C strings only!) if efficiency
    is not foremost, e.g.

      p2_hash_table *myfrobs = p2_hash_table__new(     \n
        500, 2, 1.5, my_hashing_func, my_compare_to);  \n
                                                       \n
      p2_hash_table *myfrobs = p2_hash_table__new(     \n
        0, 0, 0, STRING_DEFAULTS);                     \n */
p2_hash_table *p2_hash_table__new(
  unsigned int buffer_size,
  float expansion,
  float sparsity,
  unsigned int (*hashing_function) (const void *),
  int (*compare_to) (const void *, const void *));

/** Copy constructor. */
p2_hash_table *p2_hash_table__copy(p2_hash_table *h);

/** Destructor. */
void p2_hash_table__delete(p2_hash_table *h);


/******************************************************************************/

/** Adds a key/target pair.
    \param key  the key value to map from.  Should not be 0/NULL.
    \param target  the target value to bind to the key
    \return  The key/target pair which is displaced by the new pair. */
p2_hashing_pair p2_hash_table__add(p2_hash_table *h, void *key, void *target);

/** Looks up a key to obtain a target.
    \warning returns 0 if an entry is not found.  Beware of storing a 0 as a
    target value, else you won't be able to tell it apart from a failed lookup.
*/
void *p2_hash_table__lookup(p2_hash_table *h, const void *key);

/** Removes the key and its target.
    \return  the displaced key / target pair */
p2_hashing_pair p2_hash_table__remove(p2_hash_table *h, void *key);


/******************************************************************************/

/** Applies a void function to each key/target pair in turn. */
void *p2_hash_table__for_all(p2_hash_table *h, void *(*func)(void *, void *));

/** Applies a void function to each key value in turn. */
void *p2_hash_table__for_all_keys(p2_hash_table *h, void *(*func)(void *));

/** Applies a void function to each target value in turn. */
void *p2_hash_table__for_all_targets(p2_hash_table *h, void *(*func)(void *));

void p2_hash_table__distribute( p2_hash_table *h, p2_procedure *p );


/******************************************************************************/

#endif  /* P2_HASH_TABLE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
