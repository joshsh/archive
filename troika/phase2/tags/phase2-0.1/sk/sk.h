/*+

  SK.h

  A simple library to represent and reduce impure S,K terms.  Completely
  type-safe.

  Caution: errors occurring at the level of the imported primitives should be
  dealt with at that level.  This library contains only rudimentary exception
  handling to catch a few possible logical errors and programming mistakes.

  author: Joshua Shinavier
  last edited: 5/5/05

 *//////////////////////////////////////////////////////////////////////////////

#ifndef _SK_H
#define _SK_H	1



#include <stdlib.h>  // For the NULL macro.
#include "../include/util/array.h"
#include "SK_debug.h"



#define ATOM_REFERENCE	void *
#define TYPE_REFERENCE	void *
#define PRIM_REFERENCE	void *



enum SK_evaluation_outcome
{
  SUCCESS,  // SUCCESS == 0.
  FAILURE,
  TYPE_MISMATCH,
  ATOM_APPLIED_AS_FUNCTION,
  PRIMITIVE_APPLIED_TO_NONATOM,
  TOO_MANY_PARAMETERS,
  NULL_TERM,
  EXPIRED_TERM,
  PROGRAM_ERROR
};



#ifdef SK_DEBUG
char *term_types [6];
char *outcome_codes [9];
#endif



/*+ struct definitions *////////////////////////////////////////////////////////



/**

  An SK_primitive is a wrapper for an imported C function.

  Restrictions:

    * The function must have at least one input parameter and exactly one output
      parameter (i.e. no void functions and no functions with void input).

    * Each parameter must be of a pointer type (i.e. parameters are strictly
      pass-by-reference).

*/
struct SK_primitive
{
  char *name;

  // Physical location of the C function.
  PRIM_REFERENCE value;

  // Number of input parameters.
  int parameters;

  // An array of length {parameters} containing the data type of all input
  // parameters in the appropriate order.
  TYPE_REFERENCE *parameter_types;

  TYPE_REFERENCE return_type;
};



/**

  An SK_atom associates a data element with its data type.

*/
struct SK_atom
{
  // Physical location of the data element.
  ATOM_REFERENCE value;

  // Internal type of the atom (e.g. integer, float).
  TYPE_REFERENCE type;
};



/**

  An SK_term is either a variable (primitive or atom), an S or a K combinator,
  or compound S,K term.  Other combinators must be derived from S and K.

*/
struct SK_term
{
  enum
  {
    S_type, K_type,
    atom_type,  // : atom_value
    primitive_type,  // : primitive_value
    compound_type,  // : array_value
    expired_type
  } type;

  // Note: S and K combinators are never associated with a value;
  // their behavior is "hard wired".
  union
  {
    // An array of other SK_terms.
    ARRAY *array_value;

    struct SK_primitive *primitive_value;

    struct SK_atom *atom_value;

  } value;
};



/*- end struct definitions */

/*+ Prototypes (see SK.c) */////////////////////////////////////////////////////



/**
  Make an SK_term out of an atomic value and its type.
*/
struct SK_term *SK_atom_term(ATOM_REFERENCE value, TYPE_REFERENCE type);

/**
  Make an SK_term out of an existing SK_primitive.
*/
struct SK_term *SK_primitive_term(struct SK_primitive *primitive);

/**
  Make a term consisting of the S combinator.
*/
struct SK_term *SK_S_term();

/**
  Make a term consisting of the K combinator.
*/
struct SK_term *SK_K_term();

/**
  Copy a term in its entirety (Caution: DFS recursion overhead).
*/
struct SK_term *SK_copy(struct SK_term *term);

/**
  Garbage collect an end term.  Keep in mind that a term passed as {argument} to
  SK_apply is automatically garbage collected or becomes the property of another
  term (ergo, NEVER attempt to garbage collect it).
*/
void SK_destroy_term(struct SK_term *term);

/**
  Apply a function to an argument.  The terms are concatenated and then
  reduced as much as possible.  The return enum tells you whether an error was
  encountered during reduction.
  Caution: SK_apply makes no attempt to detect infinite loops or memory
  faults due to extremely large S,K terms.
*/
enum SK_evaluation_outcome SK_apply(struct SK_term *function, struct SK_term *argument);



/*- end prototypes */

#endif

/*- end of file */
