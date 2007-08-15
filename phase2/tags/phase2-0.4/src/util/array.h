/*+

  array.h

  An expanding array of void pointers, suitable for use as a stack or a queue.
  You can even use it as a general ordered list, although there is memory
  copying overhead associated with insertions and removals.

  last edited: 6/5/05

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

#ifndef ARRAY_H
#define ARRAY_H	1



#include <stdlib.h>

struct JS_array {
  int head, size, buffer_size;
  double expansion;
  void **buffer;
};

#define ARRAY struct JS_array



// Constructor and destructor
ARRAY *array__new(int buffer_size, double expansion);
ARRAY *array__copy(ARRAY *a);
void array__delete(ARRAY *a);

// Array resizing
void array__minimize(ARRAY *a);

// Random access
void *array__get(ARRAY *a, int index);
void *array__set(ARRAY *a, int index, void *p);

// Stack and queue operations (note: "dequeue" is nonstandard)
void *array__peek(ARRAY *a);
void array__push(ARRAY *a, void *p);
void *array__pop(ARRAY *a);
void array__enqueue(ARRAY *a, void *p);
void *array__dequeue(ARRAY *a);

// Random insertion and removal
// Note: O(n) memory copying overhead for all except simple_remove
void array__insert_before(ARRAY *a, int index, void *p);
void array__insert_after(ARRAY *a, int index, void *p);
void array__remove(ARRAY *a, int index);
void array__simple_remove(ARRAY *a, int index);

// Array sorting
void array__mergesort(ARRAY *a, int (*compare) (void *, void *));

// Miscellaneous functions
void array__clear(ARRAY *a);
void array__forall(ARRAY *a, void (*func) (void *));


#endif

/*- end of file */
