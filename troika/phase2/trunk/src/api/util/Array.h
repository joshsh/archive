/**

\file  Array.h

\brief An expanding array of void pointers, suitable for use as a stack,
queue, or indexed array.  You can even use it as a general ordered list,
although there is memory copying overhead involved in insertions and removals.

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

/*******************************************************************************

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

*******************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H


#include <Type.h>


typedef struct Array Array;


/* Constructors and destructor ************************************************/

/** Constructor. */
extern Array *
array__new( int buffer_size, unsigned int expansion );

/** Copy constructor. */
extern Array *
array__copy( Array *a );

/** Destructor. */
extern void
array__delete( Array *a );


/******************************************************************************/

extern int
array__size( Array *a );

extern unsigned int
array__expansion( Array *a );


/* Random access **************************************************************/

/** Gets the array item at a given index. */
extern void *
array__get( Array *a, int i );

/** Sets the array item at a given index. */
extern void *
array__set( Array *a, int i, void *p );


/* Stack and queue operations *************************************************/

/** Gets the first item in the array without removing it. */
extern void *
array__peek( Array *a );

/** Pushes an item onto the front of the array (as a stack). */
extern void *
array__push( Array *a, void *p );

/** Removes the item at the front of the array (as a stack). */
extern void *
array__pop( Array *a );

/** Adds an item to the tail end of the array (as a queue). */
extern void *
array__enqueue( Array *a, void *p );

/** Removes an item from the tail end of the array (as a queue).
    \warning this usage is nonstandard */
extern void *
array__dequeue( Array *a );


/* Random insertion and removal ***********************************************/

/** Insert an item before the given index.
    \warning O(n) memory copying overhead */
extern void *
array__insert_before( Array *a, int i, void *p );

/** Insert an item after the given index.
    \warning O(n) memory copying overhead */
extern void *
array__insert_after( Array *a, int i, void *p );

/** Remove the item at the given index.
    \warning O(n) memory copying overhead */
extern void *
array__remove( Array *a, int i );

/** Removes an array item and replaces it with the last item in the array,
    with no memory copying overhead.
    \warning only use this function if the order of the array elements is not
    important. */
extern void *
array__simple_remove( Array *a, int i );


/* Array sorting **************************************************************/

/** Sorts the array using MergeSort. */
extern void
array__sort( Array *a, comparator compare );


/* Logical set functions and item substitution ********************************/


extern void
array__distribute( Array *a, p2_procedure *p );


/* Miscellaneous **************************************************************/

/** Empties the array. */
extern Array *
array__clear( Array *a );

/** Resizes the array to occupy as little memory as possible.
    \note only call this function if the number of items in the array is at a
    maximum. */
extern Array *
array__minimize( Array *a );


/******************************************************************************/

extern Type *
array__type( const char *name, int flags );


#endif  /* ARRAY_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
