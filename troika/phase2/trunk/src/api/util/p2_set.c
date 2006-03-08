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

#include "p2_set.h"


static unsigned int hash( const void *el )
{
    return ( unsigned int ) el;
}


static int compare( const void *el1, const void *el2 )
{
    return ( el1 != el2 );
}


/******************************************************************************/


p2_set *p2_set__new()
{
    p2_hash_table *s = p2_hash_table__new( 0, 0, 0,
        ( hash_f ) hash, ( comparator ) compare );

    return s;
}


void p2_set__delete( p2_set *s )
{
    p2_hash_table__delete( s );
}


void p2_set__add( p2_set *s, void *el )
{
    p2_hash_table__add( s, el );
}


void p2_set__remove( p2_set *s, void *el )
{
    p2_hash_table__remove( s, el );
}

/******************************************************************************/


void p2_set__distribute( p2_set *s, p2_procedure *p )
{
    p2_hash_table__distribute( s, p );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
