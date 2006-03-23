/*+
 * hash_table.c
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

#include "hash_table.h"
#include <string.h>



#define SIZEOF_CHAR 8

// By default, the hash table will wait until it is 1/3 full before expanding.
// Note: the sparsity factor does not need to be an integer.
#define DEFAULT_SPARSITY_FACTOR 3

// By default, hash_table__expand() approximately doubles the size of the buffer.
// Note: the expansion factor does not need to be an integer.
#define DEFAULT_EXPANSION_FACTOR 2

// The number of void references contained in a single entry.
#define ENTRY_SIZE 2



// Works well enough for addresses so long as there's a one-to-one correspondence
// between addresses and key values.
int hash_address(void *key)
{
  return (int) key;
}
int compare_addresses(void *key1, void *key2)
{
  return (key1 != key2);
}
// Convert the first four bytes of the string to an integer, and use it as the
// hashing value.  Of course, this will result in a collision for any two strings
// with the same four first characters.
// Note: little-vs-big-endian-sensitive.
int hash_string(void *key)
{
  int len = strlen((char *) key);
  if (len < 3)
    return ((int) *((void **) key)) << (3-len)*SIZEOF_CHAR >> (3-len)*SIZEOF_CHAR;
  else
    return (int) *((void **) key);
}
int compare_strings(void *key1, void *key2)
{
  return strcmp((char *) key1, (char *) key2);
}



/**
 * int find_next_prime(int i)
 *
 * Returns the least prime > 2 and >= i.
 */
int find_next_prime(int i)
{
  int j;

  if (i<3)
    i=3;
  else if (!(i%2))
    i++;

  while (1) { //Breaks out when next prime is found.
    for (j=3; j<i; j+=2)
      if (!(i%j))
        j=i+1;
    if (j>i)
      i+=2;
    else
      return i;
  }
}



void hash_table__expand(HASH_TABLE *h)
{
  int i, size_old, size0 = (int) (h->buffer_size * h->expansion);
  void **p, **q, **buffer_old;

  if (size0 > h->buffer_size) {
    size_old = h->buffer_size;
    buffer_old = h->buffer;
    h->buffer_size = find_next_prime(size0);
    h->buffer = (void **) malloc(sizeof(void *) * (ENTRY_SIZE*h->buffer_size));
    h->capacity = (int) (((double) h->buffer_size)/h->sparsity);
    for (i=0; i<ENTRY_SIZE*h->buffer_size; i++)
      h->buffer[i] = NULL;
    for (i=0; i<size_old; i++) {
      p = buffer_old+(i*ENTRY_SIZE);
      if (*p != NULL) {
        q = p+1;
	h->size--; //Cancel out the incrementation for this re-hashing add()
        hash_table__add(h, *p, *q);
      }
    }
    free(buffer_old);
  }
}



/**
 *
 */
HASH_TABLE *hash_table__new(
  int buffer_size,
  double sparsity,
  double expansion,
  int (*hashing_function) (void *),
  int (*compare_to) (void *, void *))
{
  int i;

  HASH_TABLE *h = (HASH_TABLE *) malloc(sizeof(HASH_TABLE));

  h->buffer_size = find_next_prime(buffer_size);

  if (hashing_function == NULL)
    h->hashing_function = hash_address;
  else
    h->hashing_function = hashing_function;

  if (compare_to == NULL)
    h->compare_to = compare_addresses;
  else
    h->compare_to = compare_to;

  // sparsity must be at least 1, otherwise the table will not resize
  // even when it is completely full.
  if (sparsity < 1)
    h->sparsity = DEFAULT_SPARSITY_FACTOR;
  else
    h->sparsity = sparsity;

  // expansion must be greater than 1 for the buffer to actually gain in size
  if (expansion <= 1)
    h->expansion = DEFAULT_EXPANSION_FACTOR;
  else
    h->expansion = expansion;

  h->buffer = (void **) malloc(sizeof(void*) * (ENTRY_SIZE * h->buffer_size));
  h->size = 0;

  for (i=0; i<(h->buffer_size*ENTRY_SIZE); i++)
    h->buffer[i] = NULL;

  // capacity is re-calculated whenever the table resizes
  h->capacity = (int) (((double) h->buffer_size)/h->sparsity);

  return h;
}



void hash_table__delete(HASH_TABLE *h)
{
  free(h->buffer);
  free(h);
}



/*
 * Caution: returns 0/NULL if an entry is not found.  Beware of
 * storing a 0/NULL as a target value, else you won't be able to tell it
 * apart from a failed hash_table__lookup().
 */
void *hash_table__lookup(HASH_TABLE *h, void *key)
{
  int int_key, actual_size;
  void **p;

  if (key == NULL)
    return NULL;

  int_key = h->hashing_function(key);
  if (int_key < 0)
    int_key *= -1;
  p = h->buffer + (ENTRY_SIZE*(int_key%h->buffer_size));

  actual_size = ENTRY_SIZE*h->buffer_size;
  while (1) {
    if ((*p == NULL)||(!h->compare_to(*p, key)))
      break;
    // Increment and wrap
    p = h->buffer + ((p + ENTRY_SIZE - h->buffer)%actual_size);
  }
  p++;
  return *p;  //Note: relies on the target value of empty h->buffer being NULL.
}



void hash_table__add(HASH_TABLE *h, void *key, void *target)
{
  int int_key, actual_size;
  void **p;

//printf("h = %d\n", (int) h);
  if ((key == NULL)||(target == NULL))
    return;

  int_key = h->hashing_function(key);
  if (int_key < 0)
    int_key *= -1;
  p = h->buffer + (ENTRY_SIZE*(int_key%h->buffer_size));

  actual_size = ENTRY_SIZE*h->buffer_size;
  while (*p != NULL) {
    if (!h->compare_to(*p, key)) {
      h->size--;
      break;  //No duplicate entries allowed.  Replace with new target value.
    }
    // Increment and wrap
    p = h->buffer + ((p + ENTRY_SIZE - h->buffer)%actual_size);
  }
  *p = key;
  p++;
  *p = target;
  h->size++;
  if (h->size >= h->capacity)
    hash_table__expand(h);
}



void hash_table__remove(HASH_TABLE *h, void *key)
{
  int int_key, actual_size;
  void **p;

  if (key == NULL)
    return;

  int_key = h->hashing_function(key);
  if (int_key < 0)
    int_key *= -1;
  p = h->buffer + (ENTRY_SIZE*(int_key%h->buffer_size));

  actual_size = ENTRY_SIZE*h->buffer_size;
  while (*p != NULL) {
    if (!h->compare_to(*p, key)) {
      *p = NULL;
      p++;
      *p = NULL;
      h->size--;
      break;
    }
    // Increment and wrap
    p = h->buffer + ((p + ENTRY_SIZE - h->buffer)%actual_size);
  }
}



/*- end of file --------------------------------------------------------------*/
