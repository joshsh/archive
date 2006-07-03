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


Object *
object__new( Type *type, void *value, int flags )
{
    Object *o;

    /* Note: temporary objects with null type/value may be allowed. */
    if ( ( !type || !value ) && DEBUG__SAFE && !PERMIT_TEMPORARY_OBJECTS )
        abort();

    o = new( Object );

    if ( !o )
    {
        ERROR( "object__new: allocation failure" );
        return 0;
    }

    o->type = type;
    o->value = value;
    o->flags = flags;

#if TRIPLES__GLOBAL__IN_EDGES
    o->inbound_edges = 0;
#endif

#if TRIPLES__GLOBAL__OUT_EDGES
    o->outbound_edges = 0;
#endif

#if TRIPLES__GLOBAL__TRANS_EDGES
    o->trans_edges = 0;
#endif

    return o;
}


void
object__delete( Object *o )
{
    Type *t;

    void *helper( void **refp )
    {
        t->destroy( *refp );
        return 0;
    }

    if ( DEBUG__SAFE && !o )
        abort();

#if TRIPLES__GLOBAL__IN_EDGES
    if ( o->inbound_edges )
        hash_map__delete( o->inbound_edges );
#endif

#if TRIPLES__GLOBAL__OUT_EDGES
    if ( o->outbound_edges )
        hash_map__delete( o->outbound_edges );
#endif

#if TRIPLES__GLOBAL__TRANS_EDGES
    if ( o->trans_edges )
        hash_map__delete( o->trans_edges );
#endif

    /* If the object owns its children (and has any), free them. */
    if ( o->type->flags & TYPE__OWNS_DESCENDANTS )
    {
        t = o->type->type_arg;
        o->type->walk( o->value, ( Dist_f ) helper );
    }

    /* Free the object's data. */
    o->type->destroy( o->value );

    free( o );
}


/* Accessors ******************************************************************/


boolean
object__immutable( const Object *o )
{
    return o->flags & OBJECT__IMMUTABLE;
}


Type *
object__type( const Object *o )
{
    if ( DEBUG__SAFE && !o )
        abort();

    return o->type;
}


void *
object__value( const Object *o )
{
    if ( DEBUG__SAFE && !o )
        abort();

    return o->value;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
