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

#include "Apply.h"
#include "graph.h"

#include "../object/Object-impl.h"


Apply *
apply__new( Object *function, Object *operand )
{
    Apply *a;

    /* Apply objects with null members are allowed if PERMIT_NULLS. */
    if ( !PERMIT_NULLS && DEBUG__SAFE && ( !function || !operand ) )
        abort();

    a = new( Apply );

    if ( !a )
        return 0;

    a->function = function;
    a->operand = operand;

    return a;
}


void
apply__delete( Apply *a )
{
    if ( DEBUG__SAFE && !a )
        abort();

    free( a );
}


void
apply__walk( Apply *a, Dist_f f )
{
    if ( f( ( void** ) &a->function ) == walker__break
      || f( ( void** ) &a->operand ) == walker__break )
        return;
}


static Term *
subterm( Object *o )
{
    Type *t = object__type( o );
    if ( t == apply_type )
        return apply__as_term( object__value( o ) );
    else if ( t == indirection_type )
        return subterm( object__value( o ) );
    else
        return term__new( o, 0 );
}


Term *
apply__as_term( Apply *a )
{
    if ( DEBUG__SAFE && !a )
        abort();

    return term__merge_la( subterm( a->function ), subterm( a->operand ) );
}


Object *
term__to_apply_tree( Term *t, Memory_Manager *m )
{
    unsigned int l, i;
    Object *o;

    if ( DEBUG__SAFE && ( !t || !m ) )
        abort();

    l = term__length( t );

    /* Assumed: physical length of term is two cells. */
    if ( l == 1 )
        o = *( t->head + 1 );

    else
    {
        o = term__to_apply_tree( term__subterm_at( t, 0 ), m );

        for ( i = 1; i < l; i++ )
            o = memory_manager__object( m, apply_type,
                    apply__new( o, term__to_apply_tree( term__subterm_at( t, i ), m ) ),
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

    o = a->function;
    object__encode( o, buffer );
    buffer += strlen( buffer );

    sprintf( buffer, " " );
    buffer++;

    o = a->operand;
    if ( ( PERMIT_NULLS && o ) && object__type( o ) == apply_type )
    {
        sprintf( buffer, "(" );
        buffer++;
        object__encode( o, buffer );
        buffer += strlen( buffer );
        sprintf( buffer, ")" );
    }

    else
        object__encode( o, buffer );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
