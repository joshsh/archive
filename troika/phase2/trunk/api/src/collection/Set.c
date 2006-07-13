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

#include <collection/Set.h>
#include <Object.h>
#include "../settings.h"
    #include "../type/Type-impl.h"


typedef struct Entry Entry;

struct Entry
{
    char    head;
    void *  elmt;
};


static unsigned int
hash( const Entry *e )
{
    return ( unsigned int ) e->elmt;
}


static int
compare( const Entry *e1, const Entry *e2 )
{
    return ( e1->elmt != e2->elmt );
}


/******************************************************************************/


Set *
set__new( void )
{
    Hash_Table *s = hash_table__new( 0, 0, 0, sizeof ( Entry ),
        ( Hash_f ) hash, ( Comparator ) compare );

    return s;
}


void
set__delete( Set *s )
{
    hash_table__delete( s );
}


unsigned int
set__size( Set *s )
{
    return hash_table__size( s );
}


/******************************************************************************/


void
set__add( Set *s, void *el )
{
    Entry e;
    e.head = 0x1;
    e.elmt = el;

    hash_table__add( s, &e );
}


void
set__remove( Set *s, void *el )
{
    Entry e;
    e.elmt = el;

    hash_table__remove( s, &e );
}


boolean
set__contains( const Set *s, void *el )
{
    Entry e;
    e.elmt = el;

    return ( hash_table__lookup( s, &e ) != 0 );
}


/******************************************************************************/


void
set__walk( Set *s, Dist_f f )
{
    ACTION helper( Entry *e )
    {
        return f( &e->elmt );
    }

    hash_table__walk( s, ( Dist_f ) helper );
}


/******************************************************************************/


Set *
set__exclusion( Set *a, Set *b )
{
    Set *c;

    ACTION helper( Entry *e )
    {
        if ( set__contains( b, e->elmt ) )
            return REMOVE;
        else
            return CONTINUE;
    }

    c = hash_table__copy( a );
    hash_table__walk( c, ( Dist_f ) helper );

    return c;
}


Set *
set__intersection( Set *a, Set *b )
{
    Set *c, *d;

    ACTION helper( Entry *e )
    {
        if ( !set__contains( d, e->elmt ) )
            return REMOVE;
        else
            return CONTINUE;
    }

    if ( hash_table__size( a ) < hash_table__size( b ) )
    {
        c = hash_table__copy( a );
        d = b;
    }

    else
    {
        c = hash_table__copy( b );
        d = a;
    }

    hash_table__walk( c, ( Dist_f ) helper );
    return c;
}


Set *
set__symmetric_difference( Set *a, Set *b )
{
    Set *c;

    ACTION helper( Entry *e )
    {
        if ( set__contains( a, e->elmt ) && set__contains( b, e->elmt ) )
            return REMOVE;
        else
            return CONTINUE;
    }

    c = set__union( a, b );
    hash_table__walk( c, ( Dist_f ) helper );

    return c;
}


Set *
set__union( Set *a, Set *b )
{
    Set *c;

    ACTION helper( Entry *e )
    {
        hash_table__add( c, e->elmt );
        return CONTINUE;
    }

    if ( hash_table__size( a ) > hash_table__size( b ) )
    {
        c = hash_table__copy( a );
        hash_table__walk( b, ( Dist_f ) helper );
    }

    else
    {
        c = hash_table__copy( b );
        hash_table__walk( a, ( Dist_f ) helper );
    }

    return c;
}


/******************************************************************************/


void
set__encode( Set *s, char *buffer )
{
    boolean first = TRUE;

    ACTION encode( Entry *e )
    {
        Object *o = e->elmt;

        if ( first )
        {
            sprintf( buffer, " " );
            buffer++;
        }

        else
        {
            sprintf( buffer, ", " );
            buffer += 2;
        }

        object__encode( o, buffer );
        buffer += strlen( buffer );

        first = FALSE;

        return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !s || !buffer ) )
        abort();

    sprintf( buffer, "{" );
    buffer++;

    set__walk( s, ( Dist_f ) encode );

    sprintf( buffer, " }" );
}


Type *
set__create_type( const char *name, int flags )
{
    Type *type = type__new( name, flags );

    if ( type )
    {
        type->clone = ( Copy_Cons ) hash_table__copy;
        type->destroy = ( Destructor ) set__delete;
        type->encode = ( Encoder ) set__encode;
        type->size = ( Size_Of ) hash_table__size;
        type->walk = ( Walker ) set__walk;
    }

    return type;
}


/******************************************************************************/


Array *
set__to_array( Set *s )
{
    Array *a = array__new( 0, 0 );

    ACTION helper( Entry *e )
    {
        array__enqueue( a, e->elmt );
        return CONTINUE;
    }

    set__walk( s, ( Dist_f ) helper );
    return a;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
