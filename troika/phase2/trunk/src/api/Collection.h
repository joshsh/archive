/**

\file  Collection.h

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

#ifndef COLLECTION_H
#define COLLECTION_H


#include <Object.h>
#include <util/Array.h>


typedef Object Collection;

/*
typedef Type Collection_type;

Collection_type *Collection_type__new( 
*/

#define collection__new( type, value )   object__new( type, value )


extern void
collection__do_for_all(  Collection *c, void_f f );

extern void
collection__exclude_if(  Collection *c, criterion cr );

extern boolean
collection__exists(      Collection *c, criterion cr );

extern void *
collection__first_match( Collection *c, criterion cr );

extern boolean
collection__for_all(     Collection *c, criterion cr );

extern Array *
collection__match(       Collection *c, criterion cr );

extern void
collection__replace_all( Collection *c, substitution_f f, void *state );

extern void
collection__sort(        Collection *c, comparator cmp );


#endif  /* COLLECTION_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
