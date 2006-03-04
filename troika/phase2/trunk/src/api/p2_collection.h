/**

\file  p2_array.h

\brief  A simple expanding array utility.

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

#ifndef P2_COLLECTION_H
#define P2_COLLECTION_H


#include "p2_object.h"
#include "util/p2_array.h"


typedef p2_object p2_collection;

/*
typedef p2_type p2_collection_type;

p2_collection_type *p2_collection_type__new( 
*/

#define p2_collection__new( type, value )   p2_object__new( type, value )


void        p2_collection__do_for_all(  p2_collection *c, void_f f );
void        p2_collection__exclude_if(  p2_collection *c, criterion cr );
boolean     p2_collection__exists(      p2_collection *c, criterion cr );
void *      p2_collection__first_match( p2_collection *c, criterion cr );
boolean     p2_collection__for_all(     p2_collection *c, criterion cr );
p2_array *  p2_collection__match(       p2_collection *c, criterion cr );
void        p2_collection__replace_all( p2_collection *c, substitution_f f, void *state );
void        p2_collection__sort(        p2_collection *c, comparator cmp );


#endif  /* P2_COLLECTION_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
