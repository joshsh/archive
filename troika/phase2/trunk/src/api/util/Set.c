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


/******************************************************************************/


void
set__walk( Set *s, Dist_f f )
{
    hash_table__walk( s, f );
}


/******************************************************************************/


void
set__encode( Set *s, char *buffer )
{
    void encode( Object **opp )
    {
        Object *o = *opp;

        sprintf( buffer, " " );
        buffer++;

        o->type->encode( o->value, buffer );
        buffer += strlen( buffer );
    }

    #if DEBUG__SAFE
    if ( !s || !buffer )
    {
        ERROR( "set__encode: null argument" );
        return;
    }
    #endif

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
        type->destroy = ( Destructor ) set__delete;
        type->encode = ( Encoder ) set__encode;
        type->walk = ( Walker ) set__walk;
    }

    return type;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */