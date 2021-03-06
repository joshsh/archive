/*+
  set.c

  A simplified expanding hash table to store an unordered set of unique
  identifiers, for instance a set of data addresses.  The idea is not to bind the
  set elements with any other data, but merely to indicate their presence in the
  set.  Each function is modelled after a mathematical equivalent:

      set__size(S) :            "magnitude of S"
      set__lookup(S, s) :       "s is an element of S"
      set__add(S, s) :          "S' = S union {s}"
      set__remove(S, s) :       "S' = S minus {s}"
      set__union(S, T) :        "S' = S union T"
      set__intersection(S, T) : "S' = S intersect T"
      set__forall(S, f) :       "for all s in S, f(s)"
      set__exists(S, f) :       "there exists s in S such that f(s)"
      set__subset(S, f) :       "S' = all s in S such that f(s)"

  Note: there is no "complement of" operator... but one could easily be
  constructed using the above functions.


  last edited: 5/17/05

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

 *///////////////////////////////////////////////////////////////////////////////

#include "set.h"

// #define SET_DEBUG



// By default, the hash table will wait until it is 1/3 full before expanding.
// Note: the sparsity factor does not need to be an integer.
#define DEFAULT_SPARSITY_FACTOR 3

// By default, set___expand() approximately doubles the size of the buffer.
// Note: the expansion factor does not need to be an integer.
#define DEFAULT_EXPANSION_FACTOR 2



/**
 * int find_next_prime_0(int i)
 *
 * Returns the least prime > 2 and >= i.
 */
int find_next_prime_0(int i)
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



void set__expand(SET *h)
{
  int i, size_old, size0 = (int) (h->buffer_size * h->expansion);
  void **p, **q, **buffer_old;

  if (size0 > h->buffer_size) {
    size_old = h->buffer_size;
    buffer_old = h->buffer;
    h->buffer_size = find_next_prime_0(size0);
    h->buffer = (void **) malloc(sizeof(void *) * h->buffer_size);
    h->capacity = (int) (((double) h->buffer_size)/h->sparsity);
    for (i=0; i<h->buffer_size; i++)
      h->buffer[i] = NULL;
    for (i=0; i<size_old; i++) {
      p = buffer_old+i;
      if (*p != NULL) {
	h->size--; //Cancel out the incrementation for this re-hashing add()
        set__add(h, *p);
      }
    }
    free(buffer_old);
  }
}



/**
 *
 */
SET *set__new(int buffer_size, double sparsity, double expansion)
{
  int i;

  SET *h = (SET *) malloc(sizeof(SET));

  h->buffer_size = find_next_prime_0(buffer_size);

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

  h->buffer = (void **) malloc(sizeof(void*) * h->buffer_size);
  h->size = 0;

  for (i=0; i<h->buffer_size; i++)
    h->buffer[i] = NULL;

  // capacity is re-calculated whenever the table resizes
  h->capacity = (int) (((double) h->buffer_size)/h->sparsity);

  return h;
}



// Copy constructor
SET *set__copy(SET *S)
{
    SET *S2 = (SET *) malloc(sizeof(SET));
    S2->size = S->size;
    S2->buffer_size = S->buffer_size;
    S2->capacity = S->capacity;
    S2->expansion = S->expansion;
    S2->sparsity = S->sparsity;
    S2->buffer = (void **) malloc(sizeof(void*) * S2->buffer_size);
    memcpy(S2->buffer, S->buffer, sizeof(void*) * S2->buffer_size);

    return S2;
}



void set__delete(SET *h)
{
  free(h->buffer);
  free(h);
}


// set__size(S) :            "magnitude of S"
int set__size(SET *S)
{
    return S->size;
}



/*
 * returns 0 if an entry is not found.  Beware of
 * storing a 0/NULL as an element of the set, else you won't be able to tell it
 * apart from a failed set___lookup().
 */
void *set__lookup(SET *h, void *key)
{
  int int_key;
  void **p;

  if (key == NULL)
    return NULL;

  int_key = (int) key;
  if (int_key < 0)
    int_key *= -1;
  p = h->buffer + (int_key%h->buffer_size);

  while (1) {
    if (*p == NULL)
      break;
    if (*p == key)
      return key;
    // Increment and wrap
    p = h->buffer + ((p + 1 - h->buffer)%h->buffer_size);
  }
  return NULL;
}



SET *set__add(SET *h, void *key)
{
  int int_key;
  void **p;

//printf("h = %d\n", (int) h);
  if (key == NULL)
    return;

  int_key = (int) key;
  if (int_key < 0)
    int_key *= -1;
  p = h->buffer + (int_key%h->buffer_size);

  while (*p != NULL) {
    if (*p == key) {
      h->size--;
      break;  //No duplicate entries allowed.  Replace with new target value.
    }
    // Increment and wrap
    p = h->buffer + ((p + 1 - h->buffer)%h->buffer_size);
  }
  *p = key;
  h->size++;
  if (h->size >= h->capacity)
    set__expand(h);

  return h;
}



SET *set__remove(SET *h, void *key)
{
  int int_key;
  void **p;

  if (key == NULL)
    return;

  int_key = (int) key;
  if (int_key < 0)
    int_key *= -1;
  p = h->buffer + (int_key%h->buffer_size);

  while (*p != NULL) {
    if (*p == key) {
      *p = NULL;
      h->size--;
      break;
    }
    // Increment and wrap
    p = h->buffer + ((p + 1 - h->buffer)%h->buffer_size);
  }

  return h;
}



// set__union(S, T) :        "S' = S union T"
// O(n) time complexity
SET *set__union(SET *S, SET *T)
{
    void **p = T->buffer, **psup = T->buffer + T->buffer_size;
    while (p < psup)
    {
        if (*p != NULL)
	    set__add(S, *p);
	p++;
    }

    return S;
}



// set__intersection(S, T) : "S' = S intersect T"
// O(n^2) time complexity
SET *set__intersection(SET *S, SET *T)
{
    void **p = S->buffer, **psup = S->buffer + S->buffer_size;
    while (p < psup)
    {
        if ((*p != NULL)&&(!set__lookup(T, *p)))
	{
	    *p = NULL;
	    S->size--;
	}
	p++;
    }

    return S;
}



// set__forall(S, f) :       "for all s in S, f(s)"
void *set__forall(SET *S, void *(*f)(void *))
{
    void **p = S->buffer, **psup = S->buffer + S->buffer_size;
    while (p < psup)
    {
        if ((*p != NULL)&&(f(*p) == NULL))
	  return NULL;
	else
	  p++;
    }

    return (void *) 1;
}



// set__exists(S, f) :       "there exists s in S such that f(s)"
// Note: this function returns the first element s encountered for which f(s) is
// not NULL.  Otherwise it returns NULL.
void *set__exists(SET *S, void *(*f)(void *))
{
    void **p = S->buffer, **psup = S->buffer + S->buffer_size;
    while (p < psup)
    {
        if ((*p != NULL)&&(f(*p) != NULL))
	  return *p;
	else
	  p++;
    }

    return NULL;
}



// set__subset(S, f) :       "S' = all s in S such that f(s)"
SET *set__subset(SET *S, void *(*f)(void *))
{
    void **p = S->buffer, **psup = S->buffer + S->buffer_size;
    while (p < psup)
    {
        if ((*p != NULL)&&(f(*p) == NULL))
	{
	    *p = NULL;
	    S->size--;
	}
        p++;
    }

    return S;
}



// A wrapper for "free" which always returns a 1 (for use in conjunction with
// set__forall)
void *set__free(void *s)
{
    free(s);
    return (void *) 1;
}



#ifdef SET_DEBUG

    #include <stdio.h>

    int main()
    {
	SET *S2, *S = set__new(20, 2.0, 2.0);
	set__add(S, (void *) 5);
	set__add(S, (void *) 6);
	set__add(S, (void *) 7);
	set__add(S, (void *) 8);

	S2 = set__copy(S);

	printf("%d, %d\n", (int) set__lookup(S, (void *) 7),
	(int) set__lookup(S, (void *) 4));

	set__delete(S);
	set__delete(S2);
    }

#endif



/*- end of file --------------------------------------------------------------*/
