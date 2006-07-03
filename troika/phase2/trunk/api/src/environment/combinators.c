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

#include <sk.h>

#include "Environment-impl.h"


void
add_combinators( Environment *env )
{
    Object *o;
    Combinator *sk_B, *sk_C, *sk_I, *sk_K, *sk_L, *sk_O, *sk_R, *sk_S, *sk_T, *sk_U, *sk_V, *sk_W, *sk_w, *sk_Y;
    Memory_Manager *m = env->manager;

    Type *t = environment__resolve_type( env, COMBINATOR__NAME )->value;

    sk_B = new( Combinator );
    sk_C = new( Combinator );
    sk_I = new( Combinator );
    sk_K = new( Combinator );
    sk_L = new( Combinator );
    sk_O = new( Combinator );
    sk_R = new( Combinator );
    sk_S = new( Combinator );
    sk_T = new( Combinator );
    sk_U = new( Combinator );
    sk_V = new( Combinator );
    sk_W = new( Combinator );
    sk_w = new( Combinator );
    sk_Y = new( Combinator );

    *sk_B = B_combinator;
    *sk_C = C_combinator;
    *sk_I = I_combinator;
    *sk_K = K_combinator;
    *sk_L = L_combinator;
    *sk_O = O_combinator;
    *sk_R = R_combinator;
    *sk_S = S_combinator;
    *sk_T = T_combinator;
    *sk_U = U_combinator;
    *sk_V = V_combinator;
    *sk_W = W_combinator;
    *sk_w = w_combinator;
    *sk_Y = Y_combinator;

    o = memory_manager__object( m, t, sk_B, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "B", o );

    o = memory_manager__object( m, t, sk_C, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "C", o );

    o = memory_manager__object( m, t, sk_I, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "I", o );

    o = memory_manager__object( m, t, sk_K, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "K", o );

    o = memory_manager__object( m, t, sk_L, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "L", o );

    o = memory_manager__object( m, t, sk_O, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "O", o );

    o = memory_manager__object( m, t, sk_R, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "R", o );

    o = memory_manager__object( m, t, sk_S, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "S", o );

    o = memory_manager__object( m, t, sk_T, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "T", o );

    o = memory_manager__object( m, t, sk_U, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "U", o );

    o = memory_manager__object( m, t, sk_V, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "V", o );

    o = memory_manager__object( m, t, sk_W, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "W", o );

    o = memory_manager__object( m, t, sk_w, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "w", o );

    o = memory_manager__object( m, t, sk_Y, OBJECT__IMMUTABLE );
    namespace__add_simple( ( Namespace* ) env->combinators->value, "Y", o );
}


Combinator_o *
environment__resolve_combinator( Environment *env, const char *name )
{
    return namespace__lookup_simple( env->combinators->value, name );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
