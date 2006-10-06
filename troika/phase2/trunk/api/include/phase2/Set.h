/**

\file  Set.h

\brief  Provides a class to simulate mathematical sets.

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

/*******************************************************************************

Phase2 language API, Copyright (C) 2006 Joshua Shinavier.

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

#ifndef SET_H
#define SET_H


#include <phase2/Hash_Table.h>
    #include <phase2/Array.h>

#include <phase2/Type.h>


/** An enumerable set of elements. */
typedef Hash_Table Set;


/** Constructs an empty set. */
extern Set *
set__new( void );

/** Copy constructor. */
extern Set *
set__copy( Set *s );

/** Destructor. */
extern void
set__free( Set *s );


/** \return  the number of elements in s */
extern unsigned int
set__size( Set *s );

/** Applies a function to each element of the set. */
extern void
set__walk( Set *s, Visitor f );


/******************************************************************************/

/** Adds a single element to the set. */
extern void
set__add( Set *s, void *el );

/** Removes an element from the set. */
extern void
set__remove( Set *s, void *el );

/** \return  whether s contains el as an element */
extern boolean
set__contains( const Set *s, void *el );


/******************************************************************************/

/** a - b */
extern Set *
set__exclusion( Set *a, Set *b );

/** Set-theoretic equivalent of AND. */
extern Set *
set__intersection( Set *a, Set *b );

/** Set-theoretic equivalent of XOR. */
extern Set *
set__symmetric_difference( Set *a, Set *b );

/** Set-theoretic equivalent of OR. */
extern Set *
set__union( Set *a, Set *b );


/******************************************************************************/


/** Serializes the set to a string.  E.g. "{1,2,3}". */
extern void
set__encode( Set *t, char *buffer );

/** \return  an instance of the "set" data type */
extern Type *
set__create_type( const char *name, int flags );


/******************************************************************************/

/** A convenience method for implementing code which can't use set__walk. */
extern Array *
set__to_array( Set *s );


#endif  /* SET_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
