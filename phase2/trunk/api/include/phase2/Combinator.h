/**

\file  Combinator.h

\brief  Defines the Combinator enumeration.

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

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

#ifndef COMBINATOR_H
#define COMBINATOR_H


/** A value representing any of a number of basic, built-in combinators. */
typedef enum Combinator Combinator;

enum Combinator
{
    B_combinator,
    C_combinator,
    I_combinator,
    K_combinator,
    L_combinator,
    O_combinator,
    R_combinator,
    S_combinator,
    T_combinator,
    U_combinator,
    V_combinator,
    W_combinator,
    w_combinator,
    Y_combinator
};


#endif  /* COMBINATOR_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
