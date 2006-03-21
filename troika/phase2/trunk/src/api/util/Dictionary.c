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
        ERROR( "dictionary__entry__new: allocation failure" );
    #endif

    return entry;
}


static p2_action * dictionary_entry__delete
    ( dictionary_entry *entry, void *ignored )
{
    free( entry->key );
    free( entry );
    return 0;
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
    const dictionary_entry *entry2 )
{
    return strcmp( entry1->key, entry2->key );
}


/******************************************************************************/


p2_dictionary *dictionary__new()
{
    Hash_Table *h = hash_table__new( 0, 0, 0,
        ( hash_f ) hash, ( comparator ) compare );

    return h;
}


void dictionary__delete( p2_dictionary *dict )
{
    /* Destroy dictionary entries. */
    p2_procedure p;
    p.execute = ( procedure ) dictionary_entry__delete;
    hash_table__distribute( dict, &p );

    hash_table__delete( dict );
}


/******************************************************************************/


void *dictionary__add
    ( p2_dictionary *dict, const char *key, void *target )
{
    dictionary_entry *old_entry, *new_entry;
    void *r = 0;

    if ( ( new_entry = dictionary__entry__new( key, target ) )
      && ( old_entry = ( dictionary_entry* ) hash_table__add( dict, new_entry ) ) )
    {
        r = old_entry->target;
        dictionary_entry__delete( old_entry, 0 );
    }

    return r;
}


void *dictionary__lookup
    ( p2_dictionary *dict, const char *key )
{
    dictionary_entry *entry;
    dictionary_entry match_entry;

    match_entry.key = key;
    entry = hash_table__lookup( dict, &match_entry );

    return ( entry ) ? entry->target : 0;
}


void *dictionary__remove
    ( p2_dictionary *dict, const char *key )
{
    void *r = 0;
    dictionary_entry *entry;
    dictionary_entry match_entry;

    match_entry.key = key;

    entry = ( dictionary_entry* )
        hash_table__remove( dict, &match_entry );

    if ( entry )
    {
        r = entry->target;
        dictionary_entry__delete( entry, 0 );
    }

    return r;
}


/******************************************************************************/


p2_action * add_to_dict( dictionary_entry *entry, p2_dictionary *dest )
{
    dictionary__add( dest, entry->key, entry->target );
    return 0;
}


void dictionary__add_all( p2_dictionary *dest, p2_dictionary *src )
{
    p2_procedure proc;
    proc.execute = ( procedure ) add_to_dict;
    proc.state = dest;

    hash_table__distribute( src, &proc );
}


/******************************************************************************/


/* Procedure which points the argument procedure to the target value of a
   hashing pair. */
static p2_action * apply_to_target
    ( dictionary_entry *entry, p2_procedure *p )
{
    return p2_procedure__execute( p, entry->target );
}


void dictionary__distribute( p2_dictionary *dict, p2_procedure *p )
{
    p2_procedure p_alt;
    p_alt.execute = ( procedure ) apply_to_target;
    p_alt.state = p;

    hash_table__distribute( dict, &p_alt );
}


/******************************************************************************/


static p2_action * add_to_array
    ( dictionary_entry *entry, Array *a )
{
    Array__enqueue( a, entry->key );

    return 0;
}


Array *dictionary__keys( p2_dictionary *dict )
{
    Array *a = Array__new( dict->size, 0 );

    /* Fill the array with key values. */
    p2_procedure p;
    p.execute = ( procedure ) add_to_array;
    p.state = a;
    hash_table__distribute( dict, &p );

    /* Alphabetize the array. */
    Array__sort( a, ( comparator ) strcmp );

    return a;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
