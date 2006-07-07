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

#include <collection/Hash_Map.h>
#include "../settings.h"


static unsigned int
hash( const Hash_Map__Entry *entry )
{
    return ( unsigned int ) entry->key;
}


/* Works well enough for addresses so long as there's a one-to-one correspondence
   between addresses and key values. */
static int
compare( const Hash_Map__Entry *entry1, const Hash_Map__Entry *entry2 )
{
    return ( entry1->key != entry2->key );
}


/******************************************************************************/


Hash_Map *
hash_map__new( void )
{
    Hash_Table *h = hash_table__new( 0, 0, 0,
        ( hash_f ) hash, ( Comparator ) compare );

    return h;
}


void
hash_map__delete( Hash_Map *t )
{
    void *helper( Hash_Map__Entry **epp )
    {
        free( *epp );
        return 0;
    }

    /* Destroy graph entries. */
    hash_table__walk( t, ( Dist_f ) helper );

    hash_table__delete( t );
}


void
hash_map__add( Hash_Map *t, void * const key, void * const target )
{
    Hash_Map__Entry *entry, *entry_old;

    #if DEBUG__LOOKUP_TABLE
    printf( "[] hash_map__add(%#x, %#x, %#x)\n",
        ( int ) t, ( int ) key, ( int ) target );
    #endif

    if ( !( entry = new( Hash_Map__Entry ) ) )
        return;

    entry->key = key;
    entry->target = target;

    if ( ( entry_old = ( Hash_Map__Entry* ) hash_table__add( t, entry ) ) )
        free( entry_old );
}


void *
hash_map__lookup( Hash_Map *t, void * const key )
{
    Hash_Map__Entry *entry;
    Hash_Map__Entry match_entry;
    match_entry.key = key;

    if ( !( entry = hash_table__lookup( t, &match_entry ) ) )
        return 0;

    else
        return entry->target;
}


void
hash_map__remove
    ( Hash_Map *t, void * const key )
{
    Hash_Map__Entry *entry_old;
    Hash_Map__Entry match_entry;
    match_entry.key = key;

    if ( ( entry_old = ( Hash_Map__Entry* ) hash_table__remove( t, &match_entry ) ) )
        free( entry_old );
}


/******************************************************************************/


void
hash_map__walk( Hash_Map *t, Dist_f f )
{
    #if DEBUG__SAFE
    if ( !t || !f )
    {
        ERROR( "hash_map__walk: null argument" );
        return;
    }
    #endif

    #if DEBUG__LOOKUP_TABLE
    printf( "[] hash_map__walk(%#x, %#x)\n", ( int ) t, ( int ) f );
    #endif

    hash_table__walk( t, f );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
