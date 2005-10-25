/*+
  set.h

  A simplified expanding hash table to store an unordered set of unique
  identifiers, for instance a set of data addresses.  The idea is not to bind the
  set elements with any other data, but merely to indicate their presence in the
  set.  Each function is modelled after a mathematical equivalent:

      P2_set__size(S) :            "magnitude of S"
      P2_set__lookup(S, s) :       "s is an element of S"
      P2_set__add(S, s) :          "S' = S union {s}"
      P2_set__remove(S, s) :       "S' = S minus {s}"
      P2_set__union(S, T) :        "S' = S union T"
      P2_set__intersection(S, T) : "S' = S intersect T"
      P2_set__forall(S, f) :       "for all s in S, f(s)"
      P2_set__exists(S, f) :       "there exists s in S such that f(s)"
      P2_set__subset(S, f) :       "S' = all s in S such that f(s)"

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

#ifndef SET_H
#define SET_H



#include <stdlib.h>

typedef struct
{
  int size, buffer_size, capacity;
  float expansion, sparsity;
  void **buffer;
} P2_set;



/** Constructor
 * The first argument is the (int) initial size of the buffer (the actual buffer size
 * is the next prime number above the argument).  Second is a (double) expansion
 * factor which determine how aggressively the table expands when it becomes too full.
 * The third argument is a (double) sparsity factor which tells it when this has
 * occurred.  Any of these values will be set to a default if it is too low.
*/
P2_set *P2_set__new(int buffer_size, float expansion, float sparsity);

// Copy constructor
P2_set *P2_set__copy(P2_set *S);

// Destructor
void P2_set__delete(P2_set *S);

// P2_set__size(S) :            "magnitude of S"      
int P2_set__size(P2_set *S);

// P2_set__lookup(S, s) :       "s is an element of S"
void *P2_set__lookup(P2_set *S, void *s);

// P2_set__add(S, s) :          "S' = S union {s}"
P2_set *P2_set__add(P2_set *S, void *s);

// P2_set__remove(S, s) :       "S' = S minus {s}"
P2_set *P2_set__remove(P2_set *S, void *s);

// P2_set__union(S, T) :        "S' = S union T"
P2_set *P2_set__union(P2_set *S, P2_set *T);

// P2_set__intersection(S, T) : "S' = S intersect T"
P2_set *P2_set__intersection(P2_set *S, P2_set *T);

// P2_set__forall(S, f) :       "for all s in S, f(s)"
void *P2_set__forall(P2_set *S, void *(*f)(void *));

// P2_set__exists(S, f) :       "there exists s in S such that f(s)"
// Note: this function returns the first element s encountered for which f(s) is
// not NULL.  Otherwise it returns NULL.
void *P2_set__exists(P2_set *S, void *(*f)(void *));

// P2_set__subset(S, f) :       "S' = all s in S such that f(s)"
P2_set *P2_set__subset(P2_set *S, void *(*f)(void *));



// A wrapper for "free" which always returns a 1 (for use in conjunction with
// P2_set__forall)
void *P2_set__free(void *s);



#endif  // SET_H

/*- end of file --------------------------------------------------------------*/
