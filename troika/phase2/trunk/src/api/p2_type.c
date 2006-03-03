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
static void *default__clone( void *p ) { return 0; }
static void *default__decode( char *buffer ) { return 0; }
static void default__destroy( void *p ) { free( p ); }
static void default__encode( void *p, char *buffer ) { *buffer = '\0'; }
static void *default__exists( void *p, void *(*f)( void* ) ) { return 0; }
static void *default__for_all( void *p, void *(*f)( void* ) ) { return 0; }
static void default__distribute( void *p, p2_action *a ) { }


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


void p2_type__delete( p2_type *type)
{
    if ( type->name )
        free( type->name );

    free( type );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
