/*+
 * associative_array.cpp
 *
 * An expanding hash table in which both the indexing value and the
 * target value are void pointers.
 *
 * author: Josh Shinavier
 * last edited: 11/18/04
 */
 
#include "associative_array.h"
#include <cstdlib>
using namespace std;


 
#ifdef DEFAULT_EXPANSION_FACTOR
  #undef DEFAULT_EXPANSION_FACTOR
#endif
#ifdef DEFAULT_SPARSITY_FACTOR
  #undef DEFAULT_SPARSITY_FACTOR
#endif
#ifdef ENTRY_SIZE
  #undef ENTRY_SIZE
#endif

// By default, the hash table will wait until it is 1/3 full before expanding.
// Note: the sparsity factor does not need to be an integer.
#define DEFAULT_SPARSITY_FACTOR 3

// By default, expand() approximately doubles the size of the table.
// Note: the expansion factor does not need to be an integer.
#define DEFAULT_EXPANSION_FACTOR 2

// The number of void references contained in a single entry.
#define ENTRY_SIZE 2



/**
 * int SNPL_associative_array::find_next_prime(int i)
 *
 * Returns the least prime > 2 and >= i.
 */
int SNPL_associative_array::find_next_prime(int i)
{
  if (i<3)
    i=3;
  else if (!(i%2))
    i++;
  
  while (true) { //Breaks out when next prime is found.
    register int j;
    for (j=3; j<i; j+=2)
      if (!(i%j))
        j=i+1;
    if (j>i)
      i+=2;
    else
      return i;
  }
}



void SNPL_associative_array::resize(int size0)
{
  if (size0 > table_size) {
    int size_old = table_size;
    void **entries_old = entries;
    table_size = find_next_prime(size0);
    entries = new void *[ENTRY_SIZE*table_size];
    capacity = (int) (((double) table_size)/sparsity_factor);
    for (register int i=0; i<ENTRY_SIZE*table_size; i++)
      entries[i] = NULL;
    for (register int i=0; i<size_old; i++) {
      void **p = entries_old+(i*ENTRY_SIZE);
      if (*p != NULL) {
        void **q = p+1;
	filled--; //Cancel out the incrementation for this re-hashing add()
        add(*p, *q);
      }
    }
    delete entries_old;
  }
}



/*
 * No good reason to have this separate from resize() any more...
 */
void SNPL_associative_array::expand()
{
  resize((int) (table_size * expansion_factor));
}



/**
 *
 */
SNPL_associative_array::SNPL_associative_array(int size0,
                                                     double sparsity0,
						     double expansion0)
                 : table_size(find_next_prime(size0)),
		   sparsity_factor(sparsity0),
		   expansion_factor(expansion0),
		   entries(new void *[ENTRY_SIZE*table_size]),
		   filled(0)
{
  for (register int i=0; i<table_size*ENTRY_SIZE; i++)
    entries[i] = NULL;
    
  // sparsity_factor must be at least 1, otherwise the table will not resize
  // even when it is completely full.
  if (sparsity_factor < 1)
    sparsity_factor = DEFAULT_SPARSITY_FACTOR;

  // capacity is re-calculated whenever the table resizes
  capacity = (int) (((double) table_size)/sparsity_factor);
    
  // if expansion_factor is too small, the table might not actually gain in
  // integer size when it expand()'s, leading to a segmentation fault.
  if ((expansion_factor <= 1)
    ||(table_size == (int) (table_size*expansion_factor)))
    expansion_factor = DEFAULT_EXPANSION_FACTOR;
}



SNPL_associative_array::~SNPL_associative_array()
{
  delete entries;
}



/**
  Returns the number of entries in the array (NOT the physical size of the hash
  table).
*/
int SNPL_associative_array::size()
{
  return filled;
}



double SNPL_associative_array::sparsity()
{
  return sparsity_factor;
}



double SNPL_associative_array::expansion()
{
  return expansion_factor;
}



/*
 * Caution: returns 0/NULL if an entry is not found.  Beware of
 * storing a 0/NULL as a target value, else you won't be able to tell it
 * apart from a failed lookup().
 */
void *SNPL_associative_array::lookup(void *key)
{
  if (key == NULL)
    return NULL;

  int int_key = (int) key;
  if (int_key < 0)
    int_key *= -1;
  void **p = entries + (ENTRY_SIZE*(int_key%table_size));
  
  int actual_size = ENTRY_SIZE*table_size;
  while (*p != key) {
    if (*p == NULL)
      break;
    // Increment and wrap
    p = entries + ((p+ENTRY_SIZE-entries)%actual_size);
  }
  p++;
  return *p;  //Note: relies on the target value of empty entries being NULL.
}


//#include <iostream>
void SNPL_associative_array::add(void *key, void *target)
{
//if (target == NULL)
//cout << "null target for key " << (int) key << endl;
  
  if (key == NULL)
    return;

  int int_key = (int) key;
  if (int_key < 0)
    int_key *= -1;
  void **p = entries + (ENTRY_SIZE*(int_key%table_size));
  
  int actual_size = ENTRY_SIZE*table_size;  
  while (*p != NULL) {
    if (*p == key) {
      filled--;
      break;  //No duplicate entries allowed.  Replace with new target value.
    }
    // Increment and wrap
    p = entries + ((p+ENTRY_SIZE-entries)%actual_size);
  }
  *p = key;
  p++;
  *p = target;
  filled++;
  if (filled >= capacity)
    expand();
}



void SNPL_associative_array::remove(void *key)
{
  if (key == NULL)
    return;

  int int_key = (int) key;
  if (int_key < 0)
    int_key *= -1;
  void **p = entries + (ENTRY_SIZE*(int_key%table_size));
  
  int actual_size = ENTRY_SIZE*table_size;  
  while (*p != NULL) {
    if (*p == key) {
      *p = NULL;
      p++;
      *p = NULL;
      filled--;
      break;
    }
    // Increment and wrap
    p = entries + ((p+ENTRY_SIZE-entries)%actual_size);
  }
}


//#include <iostream>
// Return a new (compact) array containing all entries.
void **SNPL_associative_array::condense()
{
  void **r = new void *[filled*2], **cur_out = r;
  void **cur_in = entries, **max = cur_in + (table_size*2);
  
  while (cur_in < max) {
    while (*cur_in == NULL) {
      cur_in += 2;
      if (cur_in >= max)
        goto label;
    }
//cout << (int) *cur_in << ",";
    *cur_out = *cur_in;
    cur_out++; cur_in++;
//cout << (int) *cur_in << endl;
    *cur_out = *cur_in;
    cur_out++; cur_in++;   
  }
label:
  
  return r;
}



/*- end of file --------------------------------------------------------------*/

