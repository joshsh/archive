/*+

  SK.c

  A simple library to represent and reduce impure S,K terms.  Completely
  type-safe.

  Caution: errors occurring at the level of the imported primitives should be
  dealt with at that level.  This library contains only rudimentary exception
  handling to catch a few possible logical errors and programming mistakes.

  author: Joshua Shinavier
  last edited: 5/5/05

 *//////////////////////////////////////////////////////////////////////////////



#include "SK.h"



#define TERM_ALLOC	(struct SK_term *) malloc(sizeof(struct SK_term))
#define ATOM_ALLOC	(struct SK_atom *) malloc(sizeof(struct SK_atom))

#define ARRAY_BUFFER_EXPANSION	2.0

#define FALSE	0
#define TRUE	1



// Prototypes for hidden functions.
enum SK_evaluation_outcome SK_reduce(struct SK_term *term);
struct SK_term *SK_singleton_term(struct SK_term *term);
void SK_precat(struct SK_term *left, struct SK_term *right);



#ifdef SK_DEBUG
char *term_types [6] = {
  "S_type", "K_type",
  "atom_type",  // : atom_value
  "primitive_type",  // : primitive_value
  "compound_type",  // : array_value
  "expired_type"
};

char *outcome_codes [9] = {
  "SUCCESS",
  "FAILURE",
  "TYPE_MISMATCH",
  "ATOM_APPLIED_AS_FUNCTION",
  "PRIMITIVE_APPLIED_TO_NONATOM",
  "TOO_MANY_PARAMETERS",
  "NULL_TERM",
  "EXPIRED_TERM",
  "PROGRAM_ERROR"
};
#endif



/**
  Make an SK_term out of an atomic value and its type.
*/
struct SK_term *SK_atom_term(ATOM_REFERENCE value, TYPE_REFERENCE type)
{
  struct SK_term *term;
  struct SK_atom *atom;
  if ((value == NULL)||(type == NULL))
    return NULL;
  else {
    atom = ATOM_ALLOC;
    atom->value = value;
    atom->type = type;
    term = TERM_ALLOC;
    term->type = atom_type;
    term->value.atom_value = atom;
    return term;
  }
}



/**
  Make an SK_term out of an existing SK_primitive.
*/
struct SK_term *SK_primitive_term(struct SK_primitive *primitive)
{
  struct SK_term *term;
  if (primitive == NULL)
    return NULL;
  else
  {
    term = TERM_ALLOC;
    term->type = primitive_type;
    term->value.primitive_value = primitive;
    return term;
  }
}



/**
  Make a term consisting of the S combinator.
*/
struct SK_term *SK_S_term()
{
  struct SK_term *term = TERM_ALLOC;
  term->type = S_type;
  return term;
}



/**
  Make a term consisting of the K combinator.
*/
struct SK_term *SK_K_term()
{
  struct SK_term *term = TERM_ALLOC;
  term->type = K_type;
  return term;
}



/**
  Copy a term in its entirety (Caution: DFS recursion overhead).
*/
struct SK_term *SK_copy(struct SK_term *term)
{
  int i;
  ARRAY *array, *new_array;
  struct SK_term *new_term;
  struct SK_atom *atom, *new_atom;

  if (term != NULL)
  {
    new_term = TERM_ALLOC;
    new_term->type = term->type;

    switch (term->type)
    {
      // Important note: when an SK_atom is copied, the copy's {value} member
      // points to the same data element as that of the original SK_atom.
      case atom_type:
        atom = term->value.atom_value;
        new_atom = ATOM_ALLOC;
        new_atom->value = atom->value;
	new_atom->type = atom->type;
	new_term->value.atom_value = new_atom;
        break;

      case primitive_type:
        new_term->value.primitive_value = term->value.primitive_value;
        break;

      // Note: nothing special to be done for S_type or K_type.

      case compound_type:
        array = term->value.array_value;
	new_array = array__new(array->size, array->expansion);
	for (i=0; i<array->size; i++)
	  array__enqueue(new_array, (void *) SK_copy((struct SK_term *) array__get(array, i)));
	new_term->value.array_value = new_array;
        break;
    }

    return new_term;
  }
}



/**
  Garbage collect an end term.  Keep in mind that a term passed as {argument} to
  SK_apply is either garbage collected then and there or becomes the property of
  another term (ergo, NEVER attempt to re-use or garbage collect it).
*/
void SK_destroy_term(struct SK_term *term)
{
  int i;
  ARRAY *array;

  if (term != NULL)
  {
    switch (term->type)
    {
      case atom_type:
        free(term->value.atom_value);
        break;

      // Note: nothing special to be done for primitive_type, S_type or K_type.

      case compound_type:
        array = term->value.array_value;
	for (i=0; i<array->size; i++)
	  SK_destroy_term((struct SK_term *) array__pop(array));
	free(array);
        break;
    }

    term->type = expired_type;
    free(term);
  }
}



/**
  Apply a function to an argument.  The terms are concatenated and then
  reduced as much as possible.  The return enum tells you whether an error was
  encountered during reduction.
  Caution: SK_apply makes no attempt to detect infinite loops or memory
  faults due to extremely large S,K terms.
*/
enum SK_evaluation_outcome SK_apply(struct SK_term *function, struct SK_term *argument)
{
  ARRAY *term_array;

  // Check for NULL or expired terms (reduction does NOT check for these).
  if (argument == NULL)
    return NULL_TERM;
  else if (function == NULL) {
    SK_destroy_term(argument);
    return NULL_TERM;
  } else if (argument->type == expired_type)
    return EXPIRED_TERM;
  else if (function->type == expired_type) {
    SK_destroy_term(argument);
    return EXPIRED_TERM;
  }

  // Else proceed with evaluation.

  // If {function} is not a compound term, make it into one.
  if (function->type != compound_type)
  {
    term_array = array__new(2, ARRAY_BUFFER_EXPANSION);
    array__push(term_array, (void *) SK_copy(function));
    function->type = compound_type;
    function->value.array_value = term_array;
  }

  // Compose {function} and {argument}
  array__enqueue(function->value.array_value, (void *) argument);

  // Attempt to reduce.
  return SK_reduce(function);

}



/**

  Note: SK_reduce ASSUMES that {term} is initially of type compound_type, and
  that it is a proper compound term (i.e. it contains a left and a right subterm,
  i.e. its subterm array contains at least two elements).
  Its type may change as the last step in the reduction process.
*/
enum SK_evaluation_outcome SK_reduce(struct SK_term *term)
{
  int reduce = TRUE, i;//, size;
  enum SK_evaluation_outcome outcome;
  struct SK_term *function, *arg_x, *arg_y, *arg_z, *temp, *temp2;
  ARRAY *term_array, *subterm_array;
  struct SK_primitive *primitive;
  TYPE_REFERENCE type;
  ATOM_REFERENCE atomval;
  struct SK_atom *atom;

  // Iterate while reduce == TRUE.
  do
  {
    // This may change from one cycle to the next.
    term_array = term->value.array_value;

    // In general this changes for every cycle.
    function = array__peek(term_array);

    // Attempt to apply the left subterm to the right.
    switch (function->type)
    {
      // Atoms cannot be used as functions.
      case atom_type:

        // (no garbage-collection necessary)
        return ATOM_APPLIED_AS_FUNCTION;
        break;

      // If there are enough arguments, attempt to execute the primitive.
      // It's probably worth trying to find a way to consolidate the type
      // checking and garbage collection of arguments.
      case primitive_type:

        primitive = function->value.primitive_value;

	// If not enough arguments, abandon reduction.
	if (term_array->size <= primitive->parameters)
	  reduce = FALSE;

	else
	{
	  // Throw away the primitive term {function}.
	  SK_destroy_term(array__pop(term_array));

	  // Type-check the arguments, bail if there is a mismatch.
	  for (i=0; i<primitive->parameters; i++)
	  {
	    temp = (struct SK_term *) array__get(term_array, i);
	    if (temp->type != atom_type)
	      return PRIMITIVE_APPLIED_TO_NONATOM;  // Note: primitives are strictly separate from atoms.
	    else if (temp->value.atom_value->type != primitive->parameter_types[i])
	      return TYPE_MISMATCH;
	  }

      // I'm sure this is the stupid way to type-cast a function of a
	  // variable number of parameters... but it's handy enough for now.
      #define PARAM(index)	(void *) (((struct SK_term *) array__get(term_array, index))->value.atom_value->value)
	  switch (primitive->parameters)
	  {
            // Note: no 0 case.
            case 1:
              atomval = (ATOM_REFERENCE) ((void *(*)(void *)) primitive->value)(
                PARAM(0));
              break;
            case 2:
              atomval = (ATOM_REFERENCE) ((void *(*)(void *, void *)) primitive->value)(
                PARAM(0), PARAM(1));
              break;
            case 3:
              atomval = (ATOM_REFERENCE) ((void *(*)(void *, void *, void *)) primitive->value)(
                PARAM(0), PARAM(1), PARAM(2));
              break;
            case 4:
              atomval = (ATOM_REFERENCE) ((void *(*)(void *, void *, void *, void *)) primitive->value)(
                PARAM(0), PARAM(1), PARAM(2), PARAM(3));
              break;
            case 5:
              atomval = (ATOM_REFERENCE) ((void *(*)(void *, void *, void *, void *, void *)) primitive->value)(
                PARAM(0), PARAM(1), PARAM(2), PARAM(3), PARAM(4));
              break;
            default:
              return TOO_MANY_PARAMETERS;
          }
          #undef PARAM

	  // Remove arguments.
	  for (i=0; i<primitive->parameters; i++)
	    SK_destroy_term(array__pop(term_array));

	  // Put the primitive's return value at the head of the expression.
	  atom = ATOM_ALLOC;
	  atom->value = atomval;
	  atom->type = primitive->return_type;
	  temp = TERM_ALLOC;
	  temp->type = atom_type;
	  temp->value.atom_value = atom;
	  array__push(term_array, (void *) temp);
	}
        break;

      // Sxyz --> xz(yz)
      case S_type:

        // If not enough arguments, abandon reduction.
        if (term_array->size < 4)
	  reduce = FALSE;

	else
	{
	  // Throw away the S combinator term {function}.
	  SK_destroy_term(array__pop(term_array));

	  // Remove the first three arguments.
	  arg_x = array__pop(term_array);
	  arg_y = array__pop(term_array);
	  arg_z = array__pop(term_array);

	  // Preserve a copy of arg_z.
	  temp2 = SK_copy(arg_z);

	  // Build and reduce (arg_y arg_z) --> temp
	  if (arg_z->type == compound_type)
	    temp = arg_z;
	  else
	    temp = SK_singleton_term(arg_z);
	  SK_precat(arg_y, temp);
	  outcome = SK_reduce(temp);
	  if (outcome)
	  {
	    //? garbage collection?
	    return outcome;
	  }  // (else proceed with reduction)

	  // Push temp to the expression.
	  array__push(term_array, (void *) temp);

	  // Push the copy of arg_z to the expression.
	  array__push(term_array, (void *) temp2);

	  // Restore the first argument to the expression.
	  SK_precat(arg_x, term);
	}
        break;

      // Kxy --> x
      case K_type:

        // If not enough arguments, abandon reduction.
        if (term_array->size < 3)
	  reduce = FALSE;

	else
	{
	  // Throw away the K combinator term {function}.
	  SK_destroy_term(array__pop(term_array));

	  // Throw away the second argument (what would be arg_y), as per the
	  // reduction rule of K.
	  arg_x = array__pop(term_array);
	  SK_destroy_term(array__pop(term_array));

	  // Restore the first argument to the expression.
	  SK_precat(arg_x, term);
	}
        break;

      // The leftmost subterm should NOT be a compound term, as the array
      // representation of terms is left-associative.
      case compound_type:

        return PROGRAM_ERROR;
        break;

      default:

        return PROGRAM_ERROR;
    }

    // Warning: term_array->size has changed.
    if (term_array->size < 2)
    {
      // Pull up the singleton array element.
      temp = array__pop(term_array);
      free(term_array);
      term->type = temp->type;
      term->value = temp->value;
      free(temp);

      // Expression has been reduced to a simple term.  Stop.
      reduce = FALSE;
    }

  } while (reduce);

  return SUCCESS;
}



/**
  A shortcut to nest a term within a compound term.  You must then add other
  elements to the term's array to form a proper compound term.
*/
struct SK_term *SK_singleton_term(struct SK_term *term)
{
  ARRAY *array = array__new(2, ARRAY_BUFFER_EXPANSION);

  struct SK_term *new_term = TERM_ALLOC;
  array__push(array, (void *) term);
  new_term->type = compound_type;
  new_term->value.array_value = array;
  return new_term;
}



/**
  Form an expression in which {left} is a function and {right} is a sequence
  of arguments, taking into account the left-associativity of the array
  representation of terms.
  {right} MUST be a compound term, even if it is empty or contains only a single
  element.
*/
void SK_precat(struct SK_term *left, struct SK_term *right)
{
  int i, size;
  ARRAY *right_array = right->value.array_value, *left_array;

  if (left->type == compound_type)
  {
    left_array = left->value.array_value;
    size = left_array->size;

    // Pull out the contents of {left} and prepend them to {right}.
    for (i=0; i<size; i++)
      array__push(right_array, array__dequeue(left_array));

    // Destroy the subterm.
    free(left_array);
    free(left);
  }
  else
    array__push(right_array, (void *) left);
}



/*- end of file */
