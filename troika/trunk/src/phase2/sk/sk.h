/**

\file  sk.h

\brief  A simple utility to represent and reduce impure S,K terms.

Completely type-safe.

\warning  Errors occurring at the level of the imported primitives should be
dealt with at that level.  This library contains only rudimentary exception
handling to catch a few potential logical errors and programming mistakes.

\todo  go back and get "documentation" from old sk.h, sk.c */

/*//////////////////////////////////////////////////////////////////////////////

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

*///////////////////////////////////////////////////////////////////////////////

#ifndef SK_H
#define SK_H

/*  For the debug macro, p2_error and associated functions, and the primitive
    and typing mechanisms. */
#include "../p2_atom.h"
#include "../util/p2_term.h"
#include "../p2.h"


/** If defined, allow non-redex atoms at the beginning of an expression, and
   simply abandon reduction when they are encountered (rather than raising an
   error). */
#define PERMIT_IRREDUCIBLE_TERMS	1


/** The S combinator type. */
p2_type SK_S_type;

/** The K combinator type. */
p2_type SK_K_type;

/** Singleton S combinator. */
p2_atom *SK_S;

/** Singleton K combinator. */
p2_atom *SK_K;


p2_error

    FAILURE,                       /**< Generic failure message. */
    TYPE_MISMATCH,                 /**< Parameter and argument types don't match. */
    ATOM_APPLIED_AS_FUNCTION,      /**< A non-function atom was found at the head of a term. */
    PRIMITIVE_APPLIED_TO_NONATOM,  /**< A function primitive was applied to a combinator or other non-atom. */
    NULL_TERM,                     /**< Null term encountered. */
    NULL_ATOM,                     /**< Null atom encountered. */
    NULL_PRIMITIVE,                /**< Null primitive encountered. */
    EXPIRED_TERM,                  /**< Program tried to re-use a term which has already been passed as an argument to SK_reduce or another function which invalidates its argument. */
    PROGRAM_ERROR;                 /**< Generic error message. */


/** Initializes types, errors, and singleton S, K combinators. */
p2_error SK_init(void (*debug_print)(p2_term *));

/** \brief  Reduce an S,K term recursively according to the rules of combinator
    expressions.

    \param term  the term to reduce.  It is undefined once this function has
    been called, and should not be used again.

    \return  normally, the reduced term. If an error is encountered during
    reduction, the returned term will contain a single p2_atom of type
    p2_error_type and with an error value describing the exception.

    \note  Anything occurring as a leaf-node in a term passed to SK_reduce,
    which is not an S or K combinator or a p2_primitive, is considered a
    non-redex atom.

    \warning  This function makes no attempt to detect infinite loops or memory
    faults due to extremely large S,K terms.
*/
p2_term *SK_reduce(p2_term *term);


#endif  // SK_H

