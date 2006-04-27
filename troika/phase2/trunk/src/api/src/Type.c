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

#include <Type.h>
#include "settings.h"


/* Default member functions (use with caution). */
static void *
default__clone( void *cell )
{
    cell = 0;
    return 0;
}

static int
default__compare_to( const void* cellA, const void *cellB )
{
    cellA = cellB = 0;
    return 0;
}

static void *
default__decode( char *buffer )
{
    buffer = 0;
    return 0;
}

static void
default__destroy( void *cell )
{
    free( cell );
}

static void
default__encode( void *cell, char *buffer )
{
    cell = 0;
    sprintf( buffer, "?" );
}

static void
default__encode_safe( void *cell, String *buffer )
{
    cell = 0;
    string__append( buffer, "?" );
}

static boolean
default__equals( void *cellA, void *cellB )
{
    cellA = cellB = 0;
    return TRUE;
}

static unsigned int
default__size( const void *self )
{
    self = 0;
    return 0;
}

static void
default__sort( void *cell, Comparator cmp )
{
    cell = 0;
    cmp = 0;
}

static void
default__walk( void *coll, Dist_f f )
{
    coll = 0;
    f = 0;
}


/* Override these default members to create a custom type. */
static Type default_t =
{
    0,
    0,
    0,
    default__clone,
    default__compare_to,
    default__decode,
    default__destroy,
    default__encode,
    default__encode_safe,
    default__equals,
    default__size,
    default__sort,
    default__walk
};


Type *
type__new( const char *name, int flags )
{
    Type *t;

    #if DEBUG__SAFE
    if ( !name || !( *name ) )
    {
        ERROR( "type__new: null or empty name" );
        return 0;
    }
    #endif

    t = new( Type );
    if ( !t )
        return 0;

    *t = default_t;

    if ( !( t->name = STRDUP( name ) ) )
    {
        free( t );
        return 0;
    }

    #if DEBUG__TYPE
    printf( "[%#x] type__new(\"%s\", %i)\n", ( int ) t, name, flags );
    #endif

    t->flags = flags;

    return t;
}


void
type__delete( Type *type )
{
    if ( type->name )
        free( type->name );

    free( type );
}


/******************************************************************************/


static void
type__encode( Type *t, char *buffer )
{
    if ( DEBUG__SAFE && ( !t || !buffer ) )
        abort();

    sprintf( buffer, t->name );
}

static void
type__encode_safe( Type *t, String *buffer )
{
    if ( DEBUG__SAFE && ( !t || !buffer ) )
        abort();

    string__append( buffer, t->name );
}


Type *
type__create_type( const char *name, int flags )
{
    Type *t = type__new( name, flags );

    if ( t )
    {
        t->destroy = ( Destructor ) type__delete;
        t->encode = ( Encoder ) type__encode;
        t->encode_safe = ( Encoder_Safe ) type__encode_safe;
    }

    return t;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
