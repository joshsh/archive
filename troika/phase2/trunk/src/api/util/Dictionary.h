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


#include "Hash_Table.h"
#include "Array.h"


typedef Hash_Table p2_dictionary;


p2_dictionary *dictionary__new();

void dictionary__delete( p2_dictionary *dict );


void *dictionary__add
    ( p2_dictionary *dict, const char *key, void *target );

void *dictionary__lookup
    ( p2_dictionary *dict, const char *key );

void *dictionary__remove
    ( p2_dictionary *dict, const char *key );


void dictionary__add_all( p2_dictionary *dest, p2_dictionary *src );


void dictionary__distribute( p2_dictionary *dict, p2_procedure *p );


Array *dictionary__keys( p2_dictionary *dict );


#endif  /* P2_DICTIONARY_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
