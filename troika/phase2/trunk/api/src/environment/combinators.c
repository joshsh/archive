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

#include <Combinator.h>

#include "Environment-impl.h"


static Object *
add_single_combinator( Manager *m,
                       Namespace *ns,
                       Type *t,
                       Combinator c,
                       const char *name )
{
    Object *o;
    Combinator *cp = NEW( Combinator );

    if ( cp )
    {
        *cp = c;
        o = manager__object( m, t, cp, OBJECT__IMMUTABLE );

        if ( o )
            o = namespace__add_simple( ns, name, o );
    }

    else
        o = 0;

    return o;
}


#define add( x )  add_single_combinator( m, ns, t, x##_combinator, #x )


void
add_combinators( Environment *env )
{
    Manager *m = env->manager;
    Namespace *ns = env->combinators->value;
    Type *t = environment__resolve_type( env, NAMEOF( COMBINATOR ), FALSE )->value;

    if ( !(
            add( B )
         && add( C )
         && add( I )
         && add( K )
         && add( L )
         && add( O )
         && add( R )
         && add( S )
         && add( T )
         && add( U )
         && add( V )
         && add( W )
         && add( w )
         && add( Y ) ) )

        WARNING__ALLOC( "failed to create combinators" );
}


OBJ( COMBINATOR ) *
environment__resolve_combinator( Environment *env, const char *name )
{
    return namespace__lookup_simple( env->combinators->value, name );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
