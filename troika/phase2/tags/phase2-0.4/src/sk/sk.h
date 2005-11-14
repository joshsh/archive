/*+

  sk.h

  A simple library to represent and reduce impure S,K terms.  Completely
  type-safe.

  Caution: errors occurring at the level of the imported primitives should be
  dealt with at that level.  This library contains only rudimentary exception
  handling to catch a few potential logical errors and programming mistakes.

  last edited: 6/3/05

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
//#define PERMIT_IRREDUCIBLE_TERMS	1

// For the debug macro, P2_error and associated functions, and the primitive
// and typing mechanisms.
#include "../p2.h"

#include "../util/sequence.h"



// The only two additional atomic types.  Anything occurring as a leaf-node in a
// sequence passed to SK_reduce, which is not an S or K combinator or a
// P2_primitive, is considered a non-redex atom.
P2_type SK_S_type, SK_K_type;

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



/**
  If constant values are to be "registered" in any way when they are created by a
  primitive, you should pass a pointer to the registering function with the call
  to SK_init.  For instance, you might want to keep track of all new data created
  during the course of term reduction so you can garbage-collect it afterwards.
*/
P2_error SK_init(void (*register_constant)(SEQUENCE *), void (*debug_print)(SEQUENCE *));



//P2_error SK_set_constant_registration(void (*register_constant)(struct P2_typed_constant *));



/**
  Reduce an expression recursively according to the rules of S,K combinator
  calculus.  If an error is encountered during reduction, it will propagate
  upward to the returned sequence in the form of a P2_error atom, and the term
  itself will be deallocated.
  Caution: SK_reduce makes no attempt to predict infinite loops or memory
  faults due to extremely large S,K terms.
*/
SEQUENCE *SK_reduce(SEQUENCE *term);



#endif

/*- end of file */
