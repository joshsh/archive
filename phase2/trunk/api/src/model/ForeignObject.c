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

#include <ForeignObject.h>


struct ForeignObject
{
    Model *model;
    Name *name;

    Object *cached;
};


ForeignObject *
foreignobject__create( Model *m, Name *name )
{
    if ( DEBUG__SAFE && ( !m || !name ) )
        abort();

    ForeignObject *f = NEW( ForeignObject );
    if ( !f )
        return 0;

    /* ForeignObjects are not resolved until they are needed. */
    f->cached = 0;

    f->model = model;
    f->name = name__copy( name );
    if ( !f->name )
    {
        free( f );
        return 0;
    }

    return f;
}


Object *
foreignobject__resolve( ForeignObject *f )
{
    Object *o;

    if ( DEBUG__SAFE && !f )
        abort();

    o = f->cached;

    if ( !o )
    {
        o = model__resolve( f->model, f->name );
        f->cached = o;
    }

    return o;
}


void
foreignobject__walk( ForeignObject *f, Visitor v )
{
    Object *o;

    if ( DEBUG__SAFE && ( !f || !v ) )
        abort();

    object__walk( foreignobject__resolve( f ), v );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
