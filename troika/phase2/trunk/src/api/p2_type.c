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

#include "p2_type.h"


/* Default member functions (use with caution). */
static void *   default__clone( void *cell ) { return 0; }
static int      default__compare_to( void* cellA, void *cellB ) { return 0; }
static void *   default__decode( char *buffer ) { return 0; }
static void     default__destroy( void *cell ) { free( cell ); }
static void     default__distribute( void *cell, p2_procedure *p ) { }
static void     default__encode( void *cell, char *buffer ) { *buffer = '\0'; }
static boolean  default__equals( void *cellA, void *cellB ) { return boolean__true; }
static void *   default__exists( void *cell, void *(*f)( void* ) ) { return 0; }
static void *   default__for_all( void *cell, void *(*f)( void* ) ) { return 0; }
static void     default__sort( void *cell, comparator cmp ) { }


static p2_type default_t =
{
    0,
    0,
    default__clone,
    default__compare_to,
    default__decode,
    default__destroy,
    default__distribute,
    default__encode,
    default__equals,
    default__exists,
    default__for_all,
    default__sort
};


p2_type *p2_type__new( const char *name, int flags )
{
    #if DEBUG__SAFE
    if ( !name || !( *name ) )
    {
        PRINTERR( "p2_type__new: null or empty name" );
        return 0;
    }
    #endif

    p2_type *t = new( p2_type );
    if ( !t )
        return 0;

    *t = default_t;

    if ( !( t->name = STRDUP( name ) ) )
    {
        free( t );
        return 0;
    }

    t->flags = flags;
/*
    t->clone = default__clone;
    t->compare_to = default__compare_to;
    t->decode = default__decode;
    t->destroy = default__destroy;
    t->distribute = default__distribute;
    t->encode = default__encode;
    t->equals = default__equals;
    t->exists = default__exists;
    t->for_all = default__for_all;
    t->sort = default__sort;
*/
    return t;
}


/*
p2_type *p2_type__new
(
    char *name,
    void *( *clone )( void* ),
    void *( *decode )( char* ),
    void ( *destroy )( void* ),
    void ( *encode )( void*, char* ),
    void *( *exists )( void*, void *(*)( void* ) ),
    void *( *for_all )( void*, void *(*)( void* ) ),
    distributor distribute
)
{
    #if DEBUG__SAFE
        if ( !name || !( *name ) )
        {
            PRINTERR( "p2_type__new: null or empty name" );
            return 0;
        }
    #endif

    p2_type *t = new( p2_type );
    if ( !t )
        return 0;

    t->name = name;

    t->clone = clone ? clone : default__clone;
    t->decode = decode ? decode : default__decode;
    t->destroy = destroy ? destroy : default__destroy;
    t->encode = encode ? encode : default__encode;
    t->exists = exists ? exists : default__exists;
    t->for_all = for_all ? for_all : default__for_all;
    t->distribute = distribute ? distribute : default__distribute;

    return t;
}
*/

void p2_type__delete( p2_type *type)
{
    if ( type->name )
        free( type->name );

    free( type );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
