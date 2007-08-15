/*+
 * hash_table.h
 *
 * An expanding hash table in which both the indexing value and the
 * target value are void pointers.  Each instance holds a reference to a
 * hashing function specific to the sort of data you expect to put into the table,
 * as well as a compare_to function for key values.  You can replace both of
 * these parameters with ADDRESS_DEFAULTS or STRING_DEFAULTS (C strings only!)
 * if efficiency is not essential, e.g.
 *
 *     HASH_TABLE *myfrobs = hash_table__new(
 *       500, 2, 1.5, my_hashing_func, my_compare_to);
 *
 *     HASH_TABLE *myfrobs = hash_table__new(
 *       0, 0, 0, STRING_DEFAULTS);
 *
 * The first argument is the (int) initial size of the buffer (the actual buffer size
 * is the next prime number above the argument).  Second is a (double) expansion
 * factor which determine how aggressively the table expands when it becomes too full.
 * The third argument is a (double) sparsity factor which tells it when this has
 * occurred.  Any of these values will be set to a default if it is too low.
 *
 * The fourth and fifth arguments are the desired hashing and comparison functions,
 * respectively, or one of the macros ADDRESS_DEFAULTS or STRING_DEFAULTS. The
 * comparison function should accept two void pointers a, b as arguments and return an
 * int which is < 0 when a < b, > 0 when a > b, and 0 when a == b.  The hashing
 * function should accept a single void pointer and return an int, the idea
 * being to distribute values over a broad stretch of the 32-bit range.
 *
 * author: Joshua Shinavier
 * last edited: 1/20/05
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H



#include <stdlib.h>

struct JS_hash_table {
  int size, buffer_size, capacity;
  double expansion, sparsity;
  void **buffer;
  int (*hashing_function) (void *key);
  int (*compare_to) (void *key1, void *key2);
};

#define HASH_TABLE struct JS_hash_table

int compare_addresses(void *key1, void *key2);
int hash_address(void *key);
#define ADDRESS_DEFAULTS hash_address, compare_addresses

int compare_strings(void *key1, void *key2);
int hash_string(void *key);
#define STRING_DEFAULTS hash_string, compare_strings

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

// Add a key/target pair
void hash_table__add(HASH_TABLE *h, void *key, void *target);

// Remove the key and its target
void hash_table__remove(HASH_TABLE *h, void *key);



#endif

/*- end of file --------------------------------------------------------------*/
