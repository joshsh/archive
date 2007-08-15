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

#include <phase2/compiler.h>
#include "Apply.h"

#include "../object/Object-impl.h"


Apply *
apply__new( Object *function, Object *operand )
{
    Apply *a;

    /* Apply objects with null members are allowed if FIRST_CLASS_NULL. */
    if ( DEBUG__SAFE && !FIRST_CLASS_NULL && ( !function || !operand ) )
        abort();

    a = NEW( Apply );

    if ( !a )
        return 0;

    a->function = function;
    a->operand = operand;

    return a;
}


void
apply__free( Apply *a )
{
    if ( DEBUG__SAFE && !a )
        abort();

    free( a );
}


void
apply__walk( Apply *a, Visitor f )
{
    if ( DEBUG__SAFE && ( !a || !f ) )
        abort();

    if ( f( ( void** ) &a->function ) == BREAK
      || f( ( void** ) &a->operand ) == BREAK )
        return;
}


Term *
apply__as_term( Apply *a, Type *apply_type, Type *indirection_type )
{
    Term *subterm( Object *o )
    {
        Type *t = object__type( o );
        if ( t == apply_type )
            return apply__as_term( object__value( o ), apply_type, indirection_type );
        else if ( t == indirection_type )
            return subterm( object__value( o ) );
        else
            return term__new( o, 0 );
    }

    if ( DEBUG__SAFE && !a )
        abort();

    return term__merge_la( subterm( a->function ), subterm( a->operand ) );
}


Object *
term__to_apply_tree( Term *t, Manager *m, Type *apply_type )
{
    unsigned int l, i;
    Object *o;

    if ( DEBUG__SAFE && ( !t || !m ) )
        abort();

    l = term__length( t );

    /* Assumed: physical length of term is two cells. */
    if ( l == 1 )
        o = term__head( t );

    else
    {
        o = term__to_apply_tree( term__subterm_at( t, 0 ), m, apply_type );

        for ( i = 1; i < l; i++ )
            o = manager__object( m, apply_type,
                    apply__new( o, term__to_apply_tree( term__subterm_at( t, i ), m, apply_type ) ),
                    NOFLAGS );
    }

    return o;
}


void
apply__encode( Apply *a, char *buffer )
{
    Object *o;

    if ( DEBUG__SAFE && ( !a || !buffer ) )
        abort();

    sprintf( buffer, "!" );
/*
    o = a->function;
    object__encode( o, buffer );
    buffer += strlen( buffer );

    sprintf( buffer, " " );
    buffer++;

    o = a->operand;
    if ( ( FIRST_CLASS_NULL && o ) && object__type( o ) == apply_type )
    {
        sprintf( buffer, "(" );
        buffer++;
        object__encode( o, buffer );
        buffer += strlen( buffer );
        sprintf( buffer, ")" );
    }

    else
        object__encode( o, buffer );
*/
}


Type *
apply__create_type( const char *name, int flags )
{
    Type *type;

    if ( DEBUG__SAFE && ( !name ) )
        abort();

    type = type__new( name, flags );

    if ( type )
    {
        type->destroy = ( Destructor ) apply__free;
        type->encode = ( Encoder ) apply__encode;
        type->walk = ( Walker ) apply__walk;
    }

    return type;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
