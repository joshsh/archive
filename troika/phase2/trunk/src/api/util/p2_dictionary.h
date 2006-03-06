/**

\file  p2_dictionary.h

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

#ifndef P2_DICTIONARY_H
#define P2_DICTIONARY_H


#include "../p2_type.h"
#include "p2_hash_table.h"
#include "p2_array.h"


typedef p2_hash_table p2_dictionary;


p2_dictionary *p2_dictionary__new();

void p2_dictionary__delete( p2_dictionary *dict );


void *p2_dictionary__add
    ( p2_dictionary *dict, const char *key, void *target );

void *p2_dictionary__lookup
    ( p2_dictionary *dict, const char *key );

void *p2_dictionary__remove
    ( p2_dictionary *dict, const char *key );


p2_array *p2_dictionary__keys( p2_dictionary *dict );


#endif  /* P2_DICTIONARY_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
