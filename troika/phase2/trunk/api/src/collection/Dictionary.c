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

#include <collection/Dictionary.h>
#include "../settings.h"


typedef struct Entry Entry;

struct Entry
{
    char    head;
    char    *key;
    void    *target;
};


/** \note  From the hashpjw example by P. J. Weinberger in Aho + Sethi + Ullman. */
static unsigned int
hash( const Entry *e )
{
    char const *p;
    unsigned int h = 0, g;

    if ( DEBUG__SAFE && !e )
        ABORT;

    for ( p = e->key; *p != '\0'; p++ )
    {
        h = ( h << 4 ) + *p;
        if ( ( g = h & 0xf0000000 ) )
        {
            h = h ^ ( g >> 24 );
            h = h ^ g;
        }
    }

#if DEBUG__DICTIONARY
    PRINT( "hash(\"%s\") = %i\n", e->key, h );
#endif

    return h;
}


static int
compare( const Entry *e1, const Entry *e2 )
{
    if ( DEBUG__SAFE && ( !e1 || !e2 ) )
        ABORT;

    return strcmp( e1->key, e2->key );
}


/******************************************************************************/


Dictionary *
dictionary__new( void )
{
    Hash_Table *h = hash_table__new
        ( 0, 0, 0, sizeof( Entry ), ( Hash_f ) hash, ( Comparator ) compare );

    return h;
}


void
dictionary__delete( Dictionary *d )
{
    ACTION helper( Entry *e )
    {
        free( e->key );
        return CONTINUE;
    }

    if ( DEBUG__SAFE && !d )
        ABORT;

    /* Destroy dictionary entries. */
    hash_table__walk( d, ( Visitor ) helper );

    hash_table__delete( d );
}


/******************************************************************************/


void *
dictionary__add( Dictionary *d, const char *key, void *target )
{
    Entry *old, new;

    if ( DEBUG__SAFE && ( !d || !key || !target ) )
        ABORT;

    new.key = STRDUP( key );

    old = hash_table__lookup( d, &new );

    /* Entry exists --> just re-target it. */
    if ( old )
    {
        old->target = target;
        free( new.key );
    }

    /* Create a new entry. */
    else
    {
        new.head = 0x1;
/*
        new.key = STRDUP( key );
*/
        new.target = target;

        hash_table__add( d, &new );
    }

    /* FIXME: ? */
    return target;
}


void *
dictionary__lookup( Dictionary *d, const char *key )
{
    Entry *e, e2;

    if ( DEBUG__SAFE && ( !d || !key ) )
        ABORT;

    /* The compiler will complain about this, but hash_table__lookup is const
       in its second argument, so this is a harmless kludge. */
    e2.key = key;

    e = hash_table__lookup( d, &e2 );

    return ( e ) ? e->target : 0;
}


char *
dictionary__reverse_lookup( Dictionary *d, const void *target )
{
    char *key = 0;

    ACTION helper( Entry *e )
    {
        if ( e->target == target )
        {
            key = STRDUP( e->key );
            return BREAK;
        }

        else
            return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !d || !target ) )
        ABORT;

    hash_table__walk( d, ( Visitor ) helper );
    return key;
}


void *
dictionary__remove( Dictionary *d, char *key )
{
    Entry *e, e2;

    if ( DEBUG__SAFE && ( !d || !key ) )
        ABORT;

    e2.key = key;

    e = hash_table__lookup( d, &e2 );

    if ( e )
    {
        /* TODO: this extra lookup is wasty */
        hash_table__remove( d, &e2 );
        free( e->key );
        return e->target;
    }

    else
        return 0;
}


/******************************************************************************/


void
dictionary__add_all( Dictionary *dest, Dictionary *src )
{
    ACTION helper( Entry *e )
    {
        dictionary__add( dest, e->key, e->target );
        return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !dest || !src ) )
        ABORT;

    hash_table__walk( src, ( Visitor ) helper );
}


/******************************************************************************/


void
dictionary__walk( Dictionary *d, Visitor f )
{
    ACTION helper( Entry *e )
    {
        if ( DEBUG__SAFE && !e )
            ABORT;
/*
PRINT( "e = %p\n", e ); FFLUSH;
PRINT( "e->target = %p\n", e->target ); FFLUSH;
PRINT( "&e->target = %p\n", &e->target ); FFLUSH;
*/
        return f( &e->target );
    }

    if ( DEBUG__SAFE && ( !d || !f ) )
        ABORT;

    hash_table__walk( d, ( Visitor ) helper );
}


void
dictionary__walk_special( Dictionary *d, Dictionary_Visitor v )
{
    ACTION helper( Entry *e )
    {
        if ( DEBUG__SAFE && !e )
            ABORT;

        return v( e->key, &e->target );
    }

    if ( DEBUG__SAFE && ( !d || !v ) )
        ABORT;

    hash_table__walk( d, ( Visitor ) helper );
}


/******************************************************************************/


Array *
dictionary__keys( Dictionary *d )
{
    Array *a;

    ACTION helper( Entry *e )
    {
        array__enqueue( a, e->key );
        return CONTINUE;
    }

    if ( DEBUG__SAFE && !d )
        ABORT;

    a = array__new( hash_table__size( d ), 0 );

    /* Fill the array with key values. */
    hash_table__walk( d, ( Visitor ) helper );

    /* Alphabetize the array. */
    array__sort( a, ( Comparator ) strcmp );

    return a;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
