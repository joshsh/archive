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



//#include <stdlib.h>

typedef struct
{
    int head, size, buffer_size;
    double expansion;
    void **buffer;
} P2_array;

#define P2_array P2_array



// Constructor and destructor
P2_array *array__new(int buffer_size, double expansion);
P2_array *array__copy(P2_array *a);
void array__delete(P2_array *a);

// Array resizing
void array__minimize(P2_array *a);

// Random access
void *array__get(P2_array *a, int index);
void *array__set(P2_array *a, int index, void *p);

// Stack and queue operations (note: "dequeue" is nonstandard)
void *array__peek(P2_array *a);
void array__push(P2_array *a, void *p);
void *array__pop(P2_array *a);
void array__enqueue(P2_array *a, void *p);
void *array__dequeue(P2_array *a);

// Random insertion and removal
// Note: O(n) memory copying overhead for all except simple_remove
void array__insert_before(P2_array *a, int index, void *p);
void array__insert_after(P2_array *a, int index, void *p);
void array__remove(P2_array *a, int index);
void array__simple_remove(P2_array *a, int index);

// Array sorting
void array__mergesort(P2_array *a, int (*compare) (void *, void *));

// Miscellaneous functions
void array__clear(P2_array *a);
void array__forall(P2_array *a, void (*func) (void *));


#endif  // ARRAY_H

/*- end of file */


