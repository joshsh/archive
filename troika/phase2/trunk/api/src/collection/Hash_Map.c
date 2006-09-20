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


typedef struct Entry Entry;

struct Entry
{
    char    head;
    void *  key;
    void *  target;
};


union pointer_or_int
{
    unsigned int intval;
    void *pointerval;
};

static unsigned int
hash( const Entry *e )
{
    union pointer_or_int u;
    u.intval = 0;
    u.pointerval = e->key;

    /* FIXME: on a machine where sizeof ( void* ) > sizeof ( int ), high addresses will hash poorly */
    return u.intval;
}


/* Works well enough for addresses so long as there's a one-to-one correspondence
   between addresses and key values. */
static int
compare( const Entry *e1, const Entry *e2 )
{
    return ( e1->key != e2->key );
}


/******************************************************************************/


Hash_Map *
hash_map__new( void )
{
    Hash_Table *h = hash_table__new( 0, 0, 0, sizeof( Entry ),
        ( Hash_f ) hash, ( Comparator ) compare );

    return h;
}


void
hash_map__delete( Hash_Map *t )
{
    hash_table__delete( t );
}


void
hash_map__add( Hash_Map *t, void * const key, void * const target )
{
    Entry e;

/*
char s[100];
sprintf( s, "echo \"add(%p,%p)\" >> debug.txt", key, target );
system( s );
PRINT( "add(%p,%p)\n", key, target ); FFLUSH;
*/
    e.head = 0x1;
    e.key = key;
    e.target = target;

    hash_table__add( t, &e );
}


void *
hash_map__lookup( Hash_Map *t, void * const key )
{
    Entry e, *match;
    e.key = key;

    match = hash_table__lookup( t, &e );

    if ( match )
        return match->target;

    else
        return 0;
}


void
hash_map__remove
    ( Hash_Map *t, void * const key )
{
    Entry e;
    e.key = key;

    hash_table__remove( t, &e );
}


/******************************************************************************/


void
hash_map__walk( Hash_Map *t, Visitor f )
{
    ACTION helper( Entry *e )
    {
        return f( &e->target );
    }

    if ( DEBUG__SAFE && ( !t || !f ) )
        ABORT;

    hash_table__walk( t, ( Visitor ) helper );
}


void
hash_map__walk2( Hash_Map *t, Visitor2 f )
{
    ACTION helper( Entry *e )
    {
        return f( &e->key, &e->target );
    }

    if ( DEBUG__SAFE && ( !t || !f ) )
        ABORT;

    hash_table__walk( t, ( Visitor ) helper );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
