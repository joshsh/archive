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

#include "Indirection.h"
#include <phase2/Object.h>
#include "../type/Type-impl.h"


/** \note  The indirection type has a trivial destructor, as an indirection node
   does not own the object it points to. */
static void
indirection__free( void *p )
{
    /* Avoid a compiler warning. */
    p = 0;
}


static void
indirection__encode( Object *o, char *buffer )
{
    object__encode( o, buffer );
}


/* FIXME: beware of trying to mutate this reference (it won't have any effect). */
static void
indirection__walk( void *p, Visitor f )
{
    if ( DEBUG__SAFE && ( !p || !f ) )
        abort();

    f( &p );
}


Type *
indirection__create_type( const char *name, int flags )
{
    Type *type;

    if ( DEBUG__SAFE && ( !name ) )
        abort();

    type = type__new( name, flags );

    if ( type )
    {
        type->destroy = ( Destructor ) indirection__free;
        type->encode = ( Encoder ) indirection__encode;
        type->walk = ( Walker ) indirection__walk;
    }

    return type;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
