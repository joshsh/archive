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

#include <p2_type.h>


/* Default member functions (use with caution). */
static void *   default__clone( void *cell ) { return 0; }
static int      default__compare_to( const void* cellA, const void *cellB ) { return 0; }
static void *   default__decode( char *buffer ) { return 0; }
static void     default__destroy( void *cell ) { free( cell ); }
static void     default__distribute( void *cell, p2_procedure *p ) { }
static void     default__encode( void *cell, char *buffer ) { *buffer = '\0'; }
static boolean  default__equals( void *cellA, void *cellB ) { return boolean__true; }
static void     default__sort( void *cell, comparator cmp ) { }


static Type default_t =
{
    0,
    0,
    0,
    default__clone,
    default__compare_to,
    default__decode,
    default__destroy,
    default__distribute,
    default__encode,
    default__equals,
    default__sort
};


Type *type__new( const char *name, int flags )
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

    t->flags = flags;

    return t;
}


void type__delete( Type *type)
{
    if ( type->name )
        free( type->name );

    free( type );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
