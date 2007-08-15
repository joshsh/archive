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

#include <sk/sk.h>

#include "Environment-impl.h"


void
add_combinators( Environment *env )
{
    Object *o;
    Combinator *sk_s, *sk_k;
    Memory_Manager *m = env->manager;

    Type *t = environment__resolve_type( env, COMBINATOR__NAME )->value;

    sk_s = new( Combinator );
    sk_k = new( Combinator );

    *sk_s = S_combinator;
    *sk_k = K_combinator;

    o = object__new( t, sk_s, OBJECT__IMMUTABLE );
    memory_manager__add( m, o );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "S", o );

    o = object__new( t, sk_k, OBJECT__IMMUTABLE );
    memory_manager__add( m, o );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "K", o );
}


Combinator_o *
environment__resolve_combinator( Environment *env, const char *name )
{
    return namespace__lookup_simple( env->combinators->value, name );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
