/*+

  sk.h

  A simple library to represent and reduce impure S,K terms.  Completely
  type-safe.

  Caution: errors occurring at the level of the imported primitives should be
  dealt with at that level.  This library contains only rudimentary exception
  handling to catch a few potential logical errors and programming mistakes.

  last edited: 7/31/05

*//*/////////////////////////////////////////////////////////////////////////////

Phase2 version 0.4, Copyright (C) 2005 Joshua Shinavier.

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

Joshua Shinavier
parcour@gmail.com
+1 509 747-6051

 *//////////////////////////////////////////////////////////////////////////////

/*
[!] go back and get "documentation" from old sk.h, sk.c
*/

#ifndef SK_H
#define SK_H    1



/*
   If defined, allow non-redex atoms at the beginning of an expression, and
   simply abandon reduction when they are encountered (rather than raising an
   error).
*/
#define PERMIT_IRREDUCIBLE_TERMS	1

// For the debug macro, P2_error and associated functions, and the primitive
// and typing mechanisms.
#include "../p2_atom.h"
#include "../util/term.h"
#include "../p2.h"



// The only two additional atomic types.  Anything occurring as a leaf-node in a
// sequence passed to SK_reduce, which is not an S or K combinator or a
// P2_primitive, is considered a non-redex atom.
P2_type SK_S_type, SK_K_type;

// S and K combinators.
P2_atom *SK_S, *SK_K;

P2_error

    FAILURE,
    TYPE_MISMATCH,
    ATOM_APPLIED_AS_FUNCTION,
    PRIMITIVE_APPLIED_TO_NONATOM,
    TOO_MANY_PARAMETERS,
    NULL_TERM,
    NULL_ATOM,
    NULL_PRIMITIVE,
    EXPIRED_TERM,
    PROGRAM_ERROR;



P2_error SK_init(void (*debug_print)(P2_term *));

/**
  Reduce an S,K term recursively according to the rules of combinator
  expressions.  If an error is encountered during reduction, the returned
  P2_term will contain a single P2_atom of type P2_error_type and
  with an error value describing the exception.
  Caution: SK_reduce makes no attempt to detect infinite loops or memory
  faults due to extremely large S,K terms.
*/
P2_term *SK_reduce(P2_term *term);



#endif

/*- end of file */
