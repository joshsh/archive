/**

\file  sk.h

\brief  A simple utility to represent and reduce impure S,K terms.

Completely type-safe and thread-safe.

\warning  Errors occurring at the level of the imported primitives should be
dealt with at that level.  This library contains only rudimentary exception
handling to catch a few potential logical errors and programming mistakes.

\todo  go back and get "documentation" from old sk.h, sk.c */

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

#ifndef SK_H
#define SK_H


#include "../p2_memory_manager.h"
#include "../util/p2_term.h"


typedef enum _combinator
{
    S_combinator,
    K_combinator

} combinator;


/** \brief  Reduce an S,K term according to the rules of combinator expressions.

    \param term  the term to reduce.  It must be in strictly left-associative
    form, and is undefined once it has been passed as an argument to this
    function.

    \param p2_memory_manager  memory manager which will claim ownership of any
    new objects

    \param primitive_type  type of an embedded primitive function

    \param S_type  type of the S combinator

    \param K_type  type of the K combinator

    \param for_each_iteration  a function to apply to the term (e.g. a debug
    print function) once per iteration of the reduction algorithm.  Only has
    effect if DEBUG__SK is nonzero.

    \return  the reduced term, or 0 if an exception has occurred

    \note  Anything occurring as a leaf-node in a term passed to SK_reduce,
    which is not an S or K combinator or a primitive, is considered a
    non-redex object. */
p2_term *SK_reduce(
    p2_term *term,
    p2_memory_manager *m,
    p2_type *primitive_type,
    p2_type *combinator_type,
    void (*for_each_iteration)(p2_term*) );


#endif  /* SK_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
