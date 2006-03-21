/**

\file  p2_array.h

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


/*#include <p2/Type.h>*/
#include <p2_type.h>


typedef struct p2_array p2_array;


/* Constructors and destructor ************************************************/

/** Constructor. */
extern p2_array *p2_array__new( int buffer_size, unsigned int expansion );

/** Copy constructor. */
extern p2_array *p2_array__copy( p2_array *a );

/** Destructor. */
extern void p2_array__delete( p2_array *a );


/******************************************************************************/

extern int p2_array__size( p2_array *a );

extern unsigned int p2_array__expansion( p2_array *a );


/* Random access **************************************************************/

/** Gets the array item at a given index. */
extern void *p2_array__get( p2_array *a, int i );

/** Sets the array item at a given index. */
extern void *p2_array__set( p2_array *a, int i, void *p );


/* Stack and queue operations *************************************************/

/** Gets the first item in the array without removing it. */
extern void *p2_array__peek( p2_array *a );

/** Pushes an item onto the front of the array (as a stack). */
extern void *p2_array__push( p2_array *a, void *p );

/** Removes the item at the front of the array (as a stack). */
extern void *p2_array__pop( p2_array *a );

/** Adds an item to the tail end of the array (as a queue). */
extern void *p2_array__enqueue( p2_array *a, void *p );

/** Removes an item from the tail end of the array (as a queue).
    \warning this usage is nonstandard */
extern void *p2_array__dequeue( p2_array *a );


/* Random insertion and removal ***********************************************/

/** Insert an item before the given index.
    \warning O(n) memory copying overhead */
extern void *p2_array__insert_before( p2_array *a, int i, void *p );

/** Insert an item after the given index.
    \warning O(n) memory copying overhead */
extern void *p2_array__insert_after( p2_array *a, int i, void *p );

/** Remove the item at the given index.
    \warning O(n) memory copying overhead */
extern void *p2_array__remove( p2_array *a, int i );

/** Removes an array item and replaces it with the last item in the array,
    with no memory copying overhead.
    \warning only use this function if the order of the array elements is not
    important. */
extern void *p2_array__simple_remove( p2_array *a, int i );


/* Array sorting **************************************************************/

/** Sorts the array using MergeSort. */
extern void p2_array__sort( p2_array *a, comparator compare );


/* Logical set functions and item substitution ********************************/


extern void p2_array__distribute( p2_array *a, p2_procedure *p );


/* Miscellaneous **************************************************************/

/** Empties the array. */
extern p2_array *p2_array__clear( p2_array *a );

/** Resizes the array to occupy as little memory as possible.
    \note only call this function if the number of items in the array is at a
    maximum. */
extern p2_array *p2_array__minimize( p2_array *a );


/******************************************************************************/

extern p2_type *p2_array__type( const char *name, int flags );


#endif  /* ARRAY_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
