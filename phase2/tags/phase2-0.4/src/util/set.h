/*+
  set.h
  
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

#ifndef SET_H
#define SET_H



#include <stdlib.h>

struct JS_set {
  int size, buffer_size, capacity;
  double expansion, sparsity;
  void **buffer;
};

#define SET struct JS_set



/** Constructor
 * The first argument is the (int) initial size of the buffer (the actual buffer size
 * is the next prime number above the argument).  Second is a (double) expansion
 * factor which determine how aggressively the table expands when it becomes too full.
 * The third argument is a (double) sparsity factor which tells it when this has
 * occurred.  Any of these values will be set to a default if it is too low.
*/
SET *set__new(int buffer_size, double expansion, double sparsity);

// Copy constructor
SET *set__copy(SET *S);

// Destructor
void set__delete(SET *S);

// set__size(S) :            "magnitude of S"      
int set__size(SET *S);

// set__lookup(S, s) :       "s is an element of S"
void *set__lookup(SET *S, void *s);

// set__add(S, s) :          "S' = S union {s}"
SET *set__add(SET *S, void *s);

// set__remove(S, s) :       "S' = S minus {s}"
SET *set__remove(SET *S, void *s);

// set__union(S, T) :        "S' = S union T"
SET *set__union(SET *S, SET *T);

// set__intersection(S, T) : "S' = S intersect T"
SET *set__intersection(SET *S, SET *T);

// set__forall(S, f) :       "for all s in S, f(s)"
void *set__forall(SET *S, void *(*f)(void *));

// set__exists(S, f) :       "there exists s in S such that f(s)"
// Note: this function returns the first element s encountered for which f(s) is
// not NULL.  Otherwise it returns NULL.
void *set__exists(SET *S, void *(*f)(void *));

// set__subset(S, f) :       "S' = all s in S such that f(s)"
SET *set__subset(SET *S, void *(*f)(void *));



// A wrapper for "free" which always returns a 1 (for use in conjunction with
// set__forall)
void *set__free(void *s);



#endif

/*- end of file --------------------------------------------------------------*/
