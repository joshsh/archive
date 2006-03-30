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


/* Default member functions (use with caution). */
static void *
default__clone( void *cell )
{ return 0; }

static int
default__compare_to( const void* cellA, const void *cellB )
{ return 0; }

static void *
default__decode( char *buffer )
{ return 0; }

static void
default__destroy( void *cell )
{ free( cell ); }

static void
default__encode( void *cell, char *buffer )
{ sprintf( buffer, "?" ); }

static boolean
default__equals( void *cellA, void *cellB )
{ return TRUE; }

static unsigned int
default__size( const void *self )
{ return 0; }

static void
default__sort( void *cell, Comparator cmp )
{ }

static void
default__walk( void *coll, Dist_f f )
{ }


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
    #if DEBUG__SAFE
    if ( !t || !buffer )
    {
        ERROR( "type__encode: null argument" );
        return;
    }
    #endif

    sprintf( buffer, t->name );
}


Type *
type__create_type( const char *name, int flags )
{
    Type *t = type__new( name, flags );

    if ( t )
    {
        t->destroy = ( Destructor ) type__delete;
        t->encode = ( Encoder ) type__encode;
    }

    return t;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
