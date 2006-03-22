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

#include <util/Lookup_Table.h>


static p2_action *
lookup_table__entry__delete( Lookup_Table__Entry *entry, void *ignored )
{
    free( entry );
    return 0;
}


static unsigned int
hash( const Lookup_Table__Entry *entry )
{
    return ( unsigned int ) entry->key;
}


/* Works well enough for addresses so long as there's a one-to-one correspondence
   between addresses and key values. */
static int
compare( const Lookup_Table__Entry *entry1, const Lookup_Table__Entry *entry2 )
{
    return ( entry1->key != entry2->key );
}


/******************************************************************************/


Lookup_Table *
lookup_table__new( void )
{
    Hash_Table *h = hash_table__new( 0, 0, 0,
        ( hash_f ) hash, ( Comparator ) compare );

    return h;
}


void
lookup_table__delete( Lookup_Table *t )
{
    /* Destroy graph entries. */
    p2_procedure p = { ( procedure ) lookup_table__entry__delete, 0 };
    hash_table__distribute( t, &p );

    hash_table__delete( t );
}


void
lookup_table__add( Lookup_Table *t, void * const key, void * const target )
{
    Lookup_Table__Entry *entry, *entry_old;

    #if DEBUG__LOOKUP_TABLE
    printf( "lookup_table__add(%#x, %#x, %#x)\n",
        ( int ) t, ( int ) key, ( int ) target );
    #endif

    if ( !( entry = new( Lookup_Table__Entry ) ) )
        return;

    entry->key = key;
    entry->target = target;

    if ( ( entry_old = ( Lookup_Table__Entry* ) hash_table__add( t, entry ) ) )
        free( entry_old );
}


void *
lookup_table__lookup( Lookup_Table *t, void * const key )
{
    Lookup_Table__Entry *entry;
    Lookup_Table__Entry match_entry;
    match_entry.key = key;

    if ( !( entry = hash_table__lookup( t, &match_entry ) ) )
        return 0;

    else
        return entry->target;
}


void
lookup_table__remove
    ( Lookup_Table *t, void * const key )
{
    Lookup_Table__Entry *entry_old;
    Lookup_Table__Entry match_entry;
    match_entry.key = key;

    if ( ( entry_old = ( Lookup_Table__Entry* ) hash_table__remove( t, &match_entry ) ) )
        free( entry_old );
}


/******************************************************************************/


void
lookup_table__distribute( Lookup_Table *t, p2_procedure *p )
{
    #if DEBUG__SAFE
    if ( !t || !p )
    {
        ERROR( "lookup_table__distribute: null argument" );
        return;
    }
    #endif

    #if DEBUG__LOOKUP_TABLE
    printf( "lookup_table__distribute(%#x, %#x)\n", ( int ) t, ( int ) p );
    #endif

    hash_table__distribute( t, p );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
