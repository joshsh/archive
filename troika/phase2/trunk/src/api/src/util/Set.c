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

#include <util/Set.h>
#include <Object.h>
#include "../settings.h"


static unsigned int
hash( const void *el )
{
    return ( unsigned int ) el;
}


static int
compare( const void *el1, const void *el2 )
{
    return ( el1 != el2 );
}


/******************************************************************************/


Set *
set__new( void )
{
    Hash_Table *s = hash_table__new( 0, 0, 0,
        ( hash_f ) hash, ( Comparator ) compare );

    return s;
}


void
set__delete( Set *s )
{
    hash_table__delete( s );
}


/******************************************************************************/


void
set__add( Set *s, void *el )
{
    hash_table__add( s, el );
}


void
set__remove( Set *s, void *el )
{
    hash_table__remove( s, el );
}


boolean
set__contains( const Set *s, const void *el )
{
    return ( hash_table__lookup( s, el ) != 0 );
}


/******************************************************************************/


void
set__walk( Set *s, Dist_f f )
{
    hash_table__walk( s, f );
}


/******************************************************************************/


Set *
set__exclusion( Set *a, Set *b )
{
    Set *c;

    void *helper( void **refp )
    {
        if ( set__contains( b, *refp ) )
            return walker__remove;
        else
            return 0;
    }

    c = hash_table__copy( a );
    hash_table__walk( c, helper );

    return c;
}


Set *
set__intersection( Set *a, Set *b )
{
    Set *c, *d;

    void *helper( void **refp )
    {
        if ( !set__contains( d, *refp ) )
            return walker__remove;
        else
            return 0;
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

    hash_table__walk( c, helper );
    return c;
}


Set *
set__symmetric_difference( Set *a, Set *b )
{
    Set *c;

    void *helper( void **refp )
    {
        if ( set__contains( a, *refp ) && set__contains( b, *refp ) )
            return walker__remove;
        else
            return 0;
    }

    c = set__union( a, b );
    hash_table__walk( c, helper );

    return c;
}


Set *
set__union( Set *a, Set *b )
{
    Set *c;

    void *helper( void **refp )
    {
        hash_table__add( c, *refp );
        return 0;
    }

    if ( hash_table__size( a ) > hash_table__size( b ) )
    {
        c = hash_table__copy( a );
        hash_table__walk( b, helper );
    }

    else
    {
        c = hash_table__copy( b );
        hash_table__walk( a, helper );
    }

    return c;
}


/******************************************************************************/


void
set__encode( Set *s, char *buffer )
{
    boolean first = TRUE;

    void encode( Object **opp )
    {
        Object *o = *opp;

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

        object__type( o )->encode( object__value( o ), buffer );
        buffer += strlen( buffer );

        first = FALSE;
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

    void *helper( void **refp )
    {
        array__enqueue( a, *refp );
        return 0;
    }

    set__walk( s, ( Dist_f ) helper );
    return a;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
