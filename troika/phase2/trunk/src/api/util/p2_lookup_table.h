/**

\file  p2_lookup_table.h

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

#ifndef P2_LOOKUP_TABLE_H
#define P2_LOOKUP_TABLE_H


#include "../p2_type.h"
#include "p2_hash_table.h"


typedef struct _p2_lookup_table__entry
{
    void *key;
    void *target;

} p2_lookup_table__entry;


typedef p2_hash_table p2_lookup_table;


p2_lookup_table *p2_lookup_table__new();

void p2_lookup_table__delete( p2_lookup_table *t );


void p2_lookup_table__add
    ( p2_lookup_table *t, void * const key, void * const target );

void *p2_lookup_table__lookup( p2_lookup_table *t, void * const key );

void p2_lookup_table__remove
    ( p2_lookup_table *t, void * const src );


void p2_lookup_table__distribute( p2_lookup_table *t, p2_procedure *p );


#endif  /* P2_LOOKUP_TABLE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
