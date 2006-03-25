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

#include <util/Dictionary.h>


typedef struct Dictionary_Entry Dictionary_Entry;

struct Dictionary_Entry
{
    char *key;
    void *target;
};


static Dictionary_Entry *
dictionary_entry__new( const char *key, void *target )
{
    Dictionary_Entry *entry = new( Dictionary_Entry );

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
        ERROR( "dictionary_entry__new: allocation failure" );
    #endif

    return entry;
}


static void
dictionary_entry__delete
    ( Dictionary_Entry *entry )
{
    free( entry->key );
    free( entry );
}


static void *
dictionary_entry__delete__proc
    ( Dictionary_Entry **entry_p, void *ignored )
{
    dictionary_entry__delete( *entry_p );
    return 0;
}


/** \note  From the hashpjw example by P. J. Weinberger in Aho + Sethi + Ullman. */
static unsigned int
hash( const Dictionary_Entry *entry )
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


static int
compare( const Dictionary_Entry *entry1, const Dictionary_Entry *entry2 )
{
    return strcmp( entry1->key, entry2->key );
}


/******************************************************************************/


Dictionary *
dictionary__new( void )
{
    Hash_Table *h = hash_table__new
        ( 0, 0, 0, ( hash_f ) hash, ( Comparator ) compare );

    return h;
}


void
dictionary__delete( Dictionary *dict )
{
    /* Destroy dictionary entries. */
    Closure *c = closure__new( ( procedure ) dictionary_entry__delete__proc, 0 );
    hash_table__distribute( dict, c );
    closure__delete( c );

    hash_table__delete( dict );
}


/******************************************************************************/


void *
dictionary__add( Dictionary *dict, const char *key, void *target )
{
    Dictionary_Entry *old_entry, *new_entry;
    void *r = 0;

    if ( ( new_entry = dictionary_entry__new( key, target ) )
      && ( old_entry = ( Dictionary_Entry* ) hash_table__add( dict, new_entry ) ) )
    {
        r = old_entry->target;
        dictionary_entry__delete( old_entry );
    }

    return r;
}


void *
dictionary__lookup( Dictionary *dict, char *key )
{
    Dictionary_Entry *entry;
    Dictionary_Entry match_entry;

    match_entry.key = key;
    entry = hash_table__lookup( dict, &match_entry );

    return ( entry ) ? entry->target : 0;
}


void *
dictionary__remove( Dictionary *dict, char *key )
{
    void *r = 0;
    Dictionary_Entry *entry;
    Dictionary_Entry match_entry;

    match_entry.key = key;

    entry = ( Dictionary_Entry* )
        hash_table__remove( dict, &match_entry );

    if ( entry )
    {
        r = entry->target;
        dictionary_entry__delete( entry );
    }

    return r;
}


/******************************************************************************/


static void *
add_to_dict( Dictionary_Entry **entry_p, Dictionary *dest )
{
    dictionary__add( dest, ( *entry_p )->key, ( *entry_p )->target );
    return 0;
}


void
dictionary__add_all( Dictionary *dest, Dictionary *src )
{
    Closure *c = closure__new( ( procedure ) add_to_dict, dest );
    hash_table__distribute( src, c );
    closure__delete( c );
}


/******************************************************************************/


/* Points the argument procedure to the target value of a hashing pair. */
static void *
apply_to_target( Dictionary_Entry **entry_p, Closure *c )
{
    return closure__apply( c, &( *entry_p )->target );
}


void
dictionary__distribute( Dictionary *dict, Closure *c )
{
    Closure *alt = closure__new( ( procedure ) apply_to_target, c );

    hash_table__distribute( dict, alt );

    closure__delete( alt );
}


/******************************************************************************/


static void *
add_to_array( Dictionary_Entry **entry_p, Array *a )
{
    array__enqueue( a, ( *entry_p )->key );
    return 0;
}


Array *
dictionary__keys( Dictionary *dict )
{
    Array *a = array__new( hash_table__size( dict ), 0 );

    /* Fill the array with key values. */
    Closure *c = closure__new( ( procedure ) add_to_array, a );
    hash_table__distribute( dict, c );
    closure__delete( c );

    /* Alphabetize the array. */
    array__sort( a, ( Comparator ) strcmp );

    return a;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
