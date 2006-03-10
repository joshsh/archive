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

#include "p2_lookup_table.h"


static p2_action * p2_lookup_table__entry__delete( p2_lookup_table__entry *entry, void *ignored )
{
    free( entry );
    return 0;
}


static unsigned int hash( const p2_lookup_table__entry *entry )
{
    return ( unsigned int ) entry->key;
}


/* Works well enough for addresses so long as there's a one-to-one correspondence
   between addresses and key values. */
static int compare(
    const p2_lookup_table__entry *entry1,
    const p2_lookup_table__entry *entry2 )
{
    return ( entry1->key != entry2->key );
}


/******************************************************************************/


p2_lookup_table *p2_lookup_table__new()
{
    p2_hash_table *h = p2_hash_table__new( 0, 0, 0,
        ( hash_f ) hash, ( comparator ) compare );

    return h;
}


void p2_lookup_table__delete( p2_lookup_table *t )
{
    /* Destroy graph entries. */
    p2_procedure p = { ( procedure ) p2_lookup_table__entry__delete, 0 };
    p2_hash_table__distribute( t, &p );

    p2_hash_table__delete( t );
}


void p2_lookup_table__add
    ( p2_lookup_table *t, void * const key, void * const target )
{
    p2_lookup_table__entry *entry, *entry_old;

    if ( !( entry = new( p2_lookup_table__entry ) ) )
        return;

    entry->key = key;
    entry->target = target;

    if ( ( entry_old = ( p2_lookup_table__entry* ) p2_hash_table__add( t, entry ) ) )
        free( entry_old );
}


void *p2_lookup_table__lookup( p2_lookup_table *t, void * const key )
{
    p2_lookup_table__entry *entry;
    p2_lookup_table__entry match_entry;
    match_entry.key = key;

    if ( !( entry = p2_hash_table__lookup( t, &match_entry ) ) )
        return 0;

    else
        return entry->target;
}


void p2_lookup_table__remove
    ( p2_lookup_table *t, void * const key )
{
    p2_lookup_table__entry *entry_old;
    p2_lookup_table__entry match_entry;
    match_entry.key = key;

    if ( ( entry_old = ( p2_lookup_table__entry* ) p2_hash_table__remove( t, &match_entry ) ) )
        free( entry_old );
}


/******************************************************************************/


void p2_lookup_table__distribute( p2_lookup_table *t, p2_procedure *p )
{
    p2_hash_table__distribute( t, p );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
