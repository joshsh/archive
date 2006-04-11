/**

\file  Dictionary.h

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

#ifndef DICTIONARY_H
#define DICTIONARY_H


#include <util/Hash_Table.h>
#include <util/Array.h>


typedef Hash_Table Dictionary;


extern Dictionary *
dictionary__new( void );

extern void
dictionary__delete( Dictionary *dict );


extern void *
dictionary__add( Dictionary *dict, const char *key, void *target );

extern void *
dictionary__lookup( Dictionary *dict, char *key );

extern void *
dictionary__remove( Dictionary *dict, char *key );


extern void
dictionary__add_all( Dictionary *dest, Dictionary *src );


extern void
dictionary__walk( Dictionary *dict, Dist_f f );


extern Array *
dictionary__keys( Dictionary *dict );


#endif  /* DICTIONARY_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
