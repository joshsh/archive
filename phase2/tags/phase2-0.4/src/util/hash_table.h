/*+

  hash_table.h

  An expanding hash table in which both the indexing value and the
  target value are void pointers.  Each instance holds a reference to a
  hashing function specific to the sort of data you expect to put into the
  table, as well as a compare_to function for key values.  You can replace both
  of these parameters with ADDRESS_DEFAULTS or STRING_DEFAULTS (C strings only!)
  if efficiency is not essential, e.g.

      HASH_TABLE *myfrobs = hash_table__new(
        500, 2, 1.5, my_hashing_func, my_compare_to);

      HASH_TABLE *myfrobs = hash_table__new(
        0, 0, 0, STRING_DEFAULTS);

  The first argument is the (int) initial size of the buffer (the actual buffer
  size is the next prime number above the argument).  Second is a (double)
  expansion factor which determine how aggressively the table expands when it
  becomes too full.  The third argument is a (double) sparsity factor which
  tells it when this has occurred.  Any of these values will be set to a default
  if it is too low.

  The fourth and fifth arguments are the desired hashing and comparison
  functions, respectively, or one of the macros ADDRESS_DEFAULTS or
  STRING_DEFAULTS. The comparison function should accept two void pointers a, b
  as arguments and return an int which is < 0 when a < b, > 0 when a > b, and 0
  when a == b.  The hashing function should accept a single void pointer and
  return an int, the idea being to distribute values over a broad stretch of the
  32-bit range.

  last edited: 6/26/05

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

 *//////////////////////////////////////////////////////////////////////////////

#ifndef HASH_TABLE_H
#define HASH_TABLE_H


// If defined, bypass 
//#define USE_DEFAULTS

#include <stdlib.h>

struct JS_hash_table {
  int size, buffer_size, capacity;
  double expansion, sparsity;
  void **buffer;
  int (*hashing_function) (void *key);
  int (*compare_to) (void *key1, void *key2);
};

#define HASH_TABLE struct JS_hash_table



#ifndef DEFAULTS_DECLARED

    int compare_addresses(void *key1, void *key2);
    int hash_address(void *key);
    #define ADDRESS_DEFAULTS hash_address, compare_addresses

    int compare_strings(void *key1, void *key2);
    int hash_string(void *key);
    #define STRING_DEFAULTS hash_string, compare_strings

    #define DEFAULTS_DECLARED

#endif



// Constructor
HASH_TABLE *hash_table__new(
  int buffer_size,
  double expansion,
  double sparsity,
  int (*hashing_function) (void *),
  int (*compare_to) (void *, void *));

// Destructor
void hash_table__delete(HASH_TABLE *h);

// Look up a key to obtain a target
void *hash_table__lookup(HASH_TABLE *h, void *key);

// Add a key/target pair.  Return value is the target which is displaced by the
// new pair (if any).
void *hash_table__add(HASH_TABLE *h, void *key, void *target);

// Remove the key and its target.  Return value is the target.
void *hash_table__remove(HASH_TABLE *h, void *key);

void hash_table__forall(HASH_TABLE *h, void (*func)(void *, void *));
void hash_table__forall_keys(HASH_TABLE *h, void (*func)(void *));
void hash_table__forall_targets(HASH_TABLE *h, void (*func)(void *));



#endif

/*- end of file */
