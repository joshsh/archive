/**

\file  compiler.h

\brief  A simple utility to represent and reduce impure S,K terms.

Completely type-safe and thread-safe.

\warning  Errors occurring at the level of the imported primitives should be
dealt with at that level.  This library contains only rudimentary exception
handling to catch a few potential logical errors and programming mistakes.

\todo  go back and get "documentation" from old sk.h, sk.c */

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

#ifndef COMPILER_H
#define COMPILER_H


#include <Memory_Manager.h>
#include <collection/Array.h>
#include <collection/Term.h>
#include <Combinator.h>


extern Type *apply_type, *indirection_type;


extern void
graph_init( Type *combinator_t, Type *primitive_t );

extern void
graph_end();

extern Object *
reduce__graph_lazy( Object *o, Array *spine, Memory_Manager *m );


/** \brief  Reduce an S,K term according to the rules of combinator expressions.
    \param term  the term to reduce.  It must be in strictly left-associative
    form, and is undefined once it has been passed as an argument to this
    function.
    \param m  memory manager which will claim ownership of any
    new objects
    \param term_type  type of impure combinator terms
    \param primitive_type  type of an embedded primitive function
    \param combinator_type  type of S,K combinators
    \param set_type  type of object sets
    \return  the reduced term, or 0 if an exception has occurred
    \note  Anything occurring as a leaf-node in a term passed to this function,
    which is not an S or K combinator or a primitive, is considered a
    non-redex object. */
extern Term *
term__reduce(
    Term *term,
    Memory_Manager *m,
    Type *term_type,
    Type *primitive_type,
    Type *combinator_type,
    Type *set_type );


#endif  /* COMPILER_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
