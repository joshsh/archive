/**

\file  Hash_Map.h

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

#ifndef LOOKUP_TABLE_H
#define LOOKUP_TABLE_H


#include <collection/Hash_Table.h>


typedef struct Hash_Map__Entry Hash_Map__Entry;

struct Hash_Map__Entry
{
    void *key;
    void *target;
};


typedef Hash_Table Hash_Map;


extern Hash_Map *
hash_map__new( void );

extern void
hash_map__delete( Hash_Map *t );


extern void
hash_map__add( Hash_Map *t, void * const key, void * const target );

extern void *
hash_map__lookup( Hash_Map *t, void * const key );

extern void
hash_map__remove( Hash_Map *t, void * const src );


extern void
hash_map__walk( Hash_Map *t, Dist_f f );


#endif  /* LOOKUP_TABLE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
