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

#include "p2_dictionary.h"


/******************************************************************************/


typedef struct _dictionary_entry
{
    char *key;
    void *target;

} dictionary_entry;


static dictionary_entry *dictionary__entry__new( const char *key, void *target )
{
    dictionary_entry *entry = new( dictionary_entry );

    if ( entry )
    {
        entry->target = target;

        if ( !( entry->key = STRDUP( key ) ) )
        {
            free( entry );
            entry = 0;
        }
    }

    #if DEBUG__SAFE
    if ( !entry )
        PRINTERR( "dictionary__entry__new: allocation failure" );
    #endif

    return entry;
}


static p2_procedure__effect dictionary_entry__delete
    ( dictionary_entry **entry_p, void *state )
{
    free( ( *entry_p )->key );
    free( *entry_p );
    return p2_procedure__effect__continue;
}


/** \note  From the hashpjw example by P. J. Weinberger in Aho + Sethi + Ullman. */
static unsigned int hash( const dictionary_entry *entry )
{
    char const *p;
    unsigned int h = 0, g;

    for ( p = entry->key; *p != '\0'; p++ )
    {
        h = ( h << 4 ) + *p;
        if ( ( g = h & 0xf0000000 ) )
        {
            h = h ^ ( g >> 24 );
            h = h ^ g;
        }
    }

    return h;
}


static int compare(
    const dictionary_entry *entry1,
    const char *key2 )
{
    return strcmp( entry1->key, key2 );
}


/******************************************************************************/


p2_dictionary *p2_dictionary__new()
{
    p2_hash_table *h = p2_hash_table__new( 0, 0, 0,
        ( hash_f ) hash, ( comparator ) compare );

    return h;
}


void p2_dictionary__delete( p2_dictionary *dict )
{
    /* Destroy dictionary entries. */
    p2_procedure p = { ( procedure ) dictionary_entry__delete, 0 };
    p2_hash_table__distribute( dict, &p );

    p2_hash_table__delete( dict );
}


/******************************************************************************/


void *p2_dictionary__add
    ( p2_dictionary *dict, const char *key, void *target )
{
    dictionary_entry *old_entry, *new_entry;
    void *r = 0;

    if ( ( new_entry = dictionary__entry__new( key, target ) )
      || ( old_entry = ( dictionary_entry* ) p2_hash_table__add( dict, new_entry ) ) )
    {
        r = old_entry->target;
        dictionary__entry__delete( &old_entry, 0 );
    }

    return r;
}


void *p2_dictionary__lookup
    ( p2_dictionary *dict, const char *key )
{
    dictionary_entry *entry = p2_hash_table__lookup( dict, key );
    return ( entry ) ? entry->target : 0;
}


void *p2_dictionary__remove
    ( p2_dictionary *dict, const char *key )
{
    void *r = 0;

    dictionary_entry *entry = ( dictionary_entry* ) p2_hash_table__remove( dict, key );
    if ( entry )
    {
        r = entry->target;
        dictionary__entry__delete( &entry, 0 );
    }

    return r;
}


/******************************************************************************/


static p2_procedure__effect add_to_array
    ( dictionary_entry **entry_p, p2_array *a )
{
    p2_array__enqueue( a, ( *entry_p )->key );

    return p2_procedure__effect__continue;
}


p2_array *p2_dictionary__keys( p2_dictionary *dict )
{
    p2_array *a = p2_array__new( dict->size, 0 );

    /* Fill the array with key values. */
    p2_procedure p = { ( procedure ) add_to_array, a };
    p2_hash_table__distribute( dict, &p );

    /* Alphabetize the array. */
    p2_array__sort( a, ( comparator ) strcmp );

    return a;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
