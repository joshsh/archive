/**

\file  Collection.h

\brief  Defines some common visitor patterns.

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

#ifndef COLLECTION_H
#define COLLECTION_H


#include <phase2/Object.h>
#include <phase2/collection/Array.h>


/** A collection is an object with a "walk" method. */
typedef Object Collection;

/*
typedef Type Collection_type;

Collection_type *Collection_type__new( 
*/

#define collection__new( type, value )   object__new( type, value )


/** Removes every object from a collection which satisfies the given
    criterion. */
extern void
collection__exclude_if(  Collection *c, Criterion cr );

/** \return  whether the collection contains an object which satisfies the
    given criterion */
extern boolean
collection__exists(      Collection *c, Criterion cr );

/** \return  the first object encountered in the collection which satisfies
    the given criterion */
extern void *
collection__first_match( Collection *c, Criterion cr );

/** \return  whether every object in the collection satisfies the given
    criterion */
extern boolean
collection__for_all(     Collection *c, Criterion cr );

/** \return  an array containing each object in the collection which satisfies
    the given criterion */
extern Array *
collection__match(       Collection *c, Criterion cr );

/** Replaces each object in the collection with the object yielded by the
    given substitution function */
extern void
collection__replace_all( Collection *c, Substitution f );

/** Sorts a collection according to the given comparison function. */
extern void
collection__sort(        Collection *c, Comparator cmp );


#endif  /* COLLECTION_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
