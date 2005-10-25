/**
    \file array.h

    \brief A simple expanding array utility.

    \author Joshua Shinavier   \n
            parcour@gmail.com  \n
            +1 509 570-6990    \n */

/*//////////////////////////////////////////////////////////////////////////////

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

*///////////////////////////////////////////////////////////////////////////////

#ifndef ARRAY_H
#define ARRAY_H


/** \brief An expanding array of void pointers, suitable for use as a stack,
    queue, or indexed array.

    You can even use it as a general ordered list, although there is memory
    copying overhead involved in insertions and removals. */
typedef struct
{
    /** A relative pointer to the first item in the array.  Stack operations
        cause this to change. */
    int head;

    /** The number of occupied cells in the array. */
    int size;

    /** The number of cells the buffer array. */
    int buffer_size;

    /** The array expands by this factor whenever it outgrows its buffer.
        Memory copying into the new buffer is expensive, so beware of setting
        the expansion factor too low. */
    float expansion;

    /** The buffer array. */
    void **buffer;

} P2_array;


// Constructors and destructor /////////////////////////////////////////////////

/** Constructor. */
P2_array *array__new(int buffer_size, float expansion);

/** Copy constructor. */
P2_array *array__copy(P2_array *a);

/** Destructor. */
void array__delete(P2_array *a);


// Random access ///////////////////////////////////////////////////////////////

/** Gets the array item at a given index. */
void *array__get(P2_array *a, int index);

/** Sets the array item at a given index. */
void array__set(P2_array *a, int index, void *p);


// Stack and queue operations //////////////////////////////////////////////////

/** Gets the first item in the array without removing it. */
void *array__peek(P2_array *a);

/** Pushes an item onto the front of the array (as a stack). */
void array__push(P2_array *a, void *p);

/** Removes the item at the front of the array (as a stack). */
void *array__pop(P2_array *a);

/** Adds an item to the tail end of the array (as a queue). */
void array__enqueue(P2_array *a, void *p);

/** Removes an item from the tail end of the array (as a queue).
    \warning this usage is nonstandard */
void *array__dequeue(P2_array *a);


// Random insertion and removal ////////////////////////////////////////////////

/** Insert an item before the given index.
    \warning O(n) memory copying overhead */
void array__insert_before(P2_array *a, int index, void *p);

/** Insert an item after the given index.
    \warning O(n) memory copying overhead */
void array__insert_after(P2_array *a, int index, void *p);

/** Remove the item at the given index.
    \warning O(n) memory copying overhead */
void array__remove(P2_array *a, int index);

/** Removes an array item and replaces it with the last item in the array,
    with no memory copying overhead.
    \warning only use this function if the order of the array elements is not
    important. */
void array__simple_remove(P2_array *a, int index);


// Array sorting ///////////////////////////////////////////////////////////////

/** Sorts the array using MergeSort. */
void array__mergesort(P2_array *a, int (*compare) (void *, void *));


// Miscellaneous ///////////////////////////////////////////////////////////////

/** Empties the array. */
void array__clear(P2_array *a);

/** Resizes the array to occupy as little memory as possible.
    \note only call this function if the number of items in the array is at a
    maximum. */
void array__minimize(P2_array *a);

/** Applies a void function to each array item in turn. */
void array__forall(P2_array *a, void (*func) (void *));


#endif  // ARRAY_H

