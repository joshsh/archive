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

#include "Object-impl.h"


static Object *
object__clone( Object *o )
{
    Object *copy;

    if ( DEBUG__SAFE && !o )
        abort();

    copy = new( Object );
    *copy = *o;
    copy->value = o->type->clone( o->value );
    return copy;
}


static int
object__compare_to( Object *o1, Object *o2 )
{
    if ( DEBUG__SAFE && ( !o1 || !o2 ) )
        abort();

    else if ( o1->type != o2->type )
    {
        ERROR( "object__compare_to: type mismatch" );
        return 0;
    }

    else
        return o1->type->compare_to( o1->value, o2->value );
}


/* Note: there is no object decoder, as the data type of an object cannot be
   inferred from the buffer alone. */

/* Note: object destructor has external linkage (see above). */


void
object__encode( const Object *o, const char *buffer )
{
    if ( !o )
    {
        if ( FIRST_CLASS_NULL )
        {
            sprintf( buffer, "()" );
            return;
        }

        else if ( DEBUG__SAFE )
            abort();
    }

    else if ( DEBUG__SAFE && !o->value )
        abort();

    o->type->encode( o->value, buffer );
}


static boolean
object__equals( Object *o1, Object *o2 )
{
    if ( DEBUG__SAFE && ( !o1 || !o2 ) )
        abort();

    else if ( o1->type != o2->type )
    {
        ERROR( "object__equals: type mismatch" );
        return 0;
    }

    else
        return o1->type->equals( o1->value, o2->value );
}


static void
object__sort( Object *o, Comparator cmp )
{
    if ( DEBUG__SAFE && !o )
        abort();

    o->type->sort( o->value, cmp );
}


static void
object__walk( Object *o, Dist_f f )
{
    if ( DEBUG__SAFE && !o )
        abort();

    o->type->walk( o->value, f );
}


/******************************************************************************/


Type *
object__create_type( const char *name )
{
    int flags = 0;
    Type *type;

    if ( DEBUG__SAFE && !name )
        abort();

    type = type__new( name, flags );

    if ( type )
    {
        type->clone = ( Copy_Cons ) object__clone;
        type->compare_to = ( Comparator ) object__compare_to;
        type->destroy = ( Destructor ) object__delete;
        type->equals = ( Criterion2 ) object__equals;
        type->sort = ( Sort ) object__sort;
        type->walk = ( Walker ) object__walk;
    }

    return type;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
