/**

\file  Lookup_Table.h

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

#ifndef LOOKUP_TABLE_H
#define LOOKUP_TABLE_H


#include <util/Hash_Table.h>


typedef struct Lookup_Table__Entry Lookup_Table__Entry;

struct Lookup_Table__Entry
{
    void *key;
    void *target;
};


typedef Hash_Table Lookup_Table;


extern Lookup_Table *lookup_table__new( void );

extern void lookup_table__delete( Lookup_Table *t );


extern void lookup_table__add
    ( Lookup_Table *t, void * const key, void * const target );

extern void *lookup_table__lookup( Lookup_Table *t, void * const key );

extern void lookup_table__remove
    ( Lookup_Table *t, void * const src );


extern void lookup_table__distribute( Lookup_Table *t, p2_procedure *p );


#endif  /* LOOKUP_TABLE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
