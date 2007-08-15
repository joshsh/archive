/*+

  expression.cpp

  SNPL's concession to SParse.  Contains procedures for interpreting, printing and
  beta-reducing expressions which may not be in normal form, as well as the
  definition of the SNPL_expression structure which is needed by executor.h.

  Note: includes primitive_shell.h

  author: Josh Shinavier
  last edited: 12/17/04

  Structure of an expression:
  [...]

*/

#include "snpl.h"
#include <cstdio>

#include <iostream> //for debugging
//#include "util/strutil.h"

using namespace std;



void print_expression(SNPL_expression *exp, SNPL_associative_array *rev_dict)
{
  char *p = exp->body;
  void *pointer;
  char *name;

  // This expression holds nothing but a single reference to a SNPL_primitive_shell.
  if (*p == (char) REFERENCE) {
    printf("[");
    SNPL_print_shell(
      (SNPL_primitive_shell *) *((void **) ++p), rev_dict);
    printf("]");
    return;
  }
//cout << "print_expression:" << endl;
//view_bytes((void *) exp->body, exp->length);
  p++;
  printf("[");

  char *terminus = exp->body + exp->length - 1;
  int first_item = true;
  while (p < terminus)
  {
    switch ((int) *p)
    {
      case OPEN_SUBTERM:
        if (!first_item)
        printf(" ");
//echo("OPEN_SUBTERM");
        printf("(");

        first_item = true;
        p++;  // Get the cursor past this token.
        break;

      case CLOSE_SUBTERM:
        printf(")");
//echo("CLOSE_SUBTERM");
        first_item = false;
        p++;  // Get the cursor past this token.
        break;

      case SELF_REFERENCE:
        if (!first_item)
          printf(" ");

	printf("@");

	first_item = false;
        p++;  // Get the cursor past this token.
        break;

      case REFERENCE:
        if (!first_item)
          printf(" ");
//echo("REFERENCE");
        SNPL_print(*((void **) ++p), rev_dict);

        first_item = false;
        p += SIZEOF_POINTER;  // Get the cursor past the item reference.
        break;

      case FIRST_PARAMETER:
        if (!first_item)
        printf(" ");
//echo("FIRST_PARAMETER");
        printf("%d", FIRST_PARAMETER + (exp->bound_variables-1) - (int) *p);

        first_item = false;
        p++;  // Get the cursor past this token.
        break;

      default:
        if (!first_item)
        printf(" ");
//echo("default");
        printf("%d", FIRST_PARAMETER + (exp->bound_variables-1) - (int) *p);

        first_item = false;
        p++;  // Get the cursor past this token.
    }
  }
  printf("]");
}


/*
  Pass it a valid expression string and it will give you a SNPL_expression.
  An invalid expression string will result in a return value of NULL.
  Note: the parameter references in the original string are assumed to be
  "inverted" and offset, e.g. of params "5" through "9", param "9" will be
  the first to be resolved).
*/
SNPL_expression *SNPL_construct_expression(
  char *body,
  int length,
  int bound_variables)
{
//XXXcout << "length = " << length << endl;
//XXXcout << "bound vars = " << bound_variables <<endl;
//XXXview_bytes(body, length);

  char *newbody = new char[length+1], *p = newbody, *p2 = body,
    *terminus;
  int i, paren_depth = 0, good = 1, param, highest_param = -1;
  int *bound_vars = NULL;
    
  for (i=0; i<length; i++) {
    *p = *p2;
    p++; p2++;
  }
  *p = '\0';
  terminus = p;
  p = newbody;

  // Is it alright (by malloc) that bound_variables sometimes == 0?
  if (bound_variables) {
    bound_vars = (int *) malloc(bound_variables*sizeof(int));
    for (i=0; i<bound_variables; i++)
      bound_vars[i] = 0;
  }

  while ((p < terminus) && good)
  {
  //printf(".");
//XXXcout << "p = " << length-(terminus-p) << ", *p = " << (int) *p << endl;
    switch ((int) *p) {

      case OPEN_SUBTERM:
//XXXcout << "OPEN_SUBTERM" << endl;
        paren_depth++;
        p++;  // Get the cursor past this token.
        break;

      case CLOSE_SUBTERM:
//XXXcout << "CLOSE_SUBTERM" << endl;
        paren_depth--;
        p++;  // Get the cursor past this token.
        break;

      case SELF_REFERENCE:
//XXXcout << "SELF_REFERENCE" << endl;
        p++;
	    break;

      // Note: does not check whether the reference is to an actual item.
      case REFERENCE:
//XXXcout << "REFERENCE" << endl;
        p += (1+SIZEOF_POINTER);  // Get the cursor past the item reference.
        break;

      default:
//XXXcout << "[default]" << endl;
        param = (int) *p - FIRST_PARAMETER;
        if ((param < 0)||(param >= bound_variables)) {
          good = 0;
//XXXcout << "bad parameter: " << param << endl;
        } else {
          bound_vars[param] = 1;
          if (param > highest_param)
            highest_param = param;
            *p = (char) (FIRST_PARAMETER + (bound_variables-(param+1)));
            p++;  // Get the cursor past this token.
        }
    }
  }
//printf("expression_constructor() #2:\n");
//view_bytes((void *) body, length);
  if (good) {
    if (paren_depth)
      good = 0;
    else if (highest_param+1 != bound_variables)
      good = 0;
    else {
      for (i=0; i<=highest_param; i++) {
        if (!bound_vars[i]) {
          good = 0;
          break;
        }
      }
    }
  }

  if (bound_vars != NULL)
    free(bound_vars);

  if (!good) {
    printf("Error in SNPL_construct_expression(): bad expression.\n");
    return NULL;
  } else {
    SNPL_expression *exp = (SNPL_expression *) malloc(sizeof(SNPL_expression));
    exp->body = newbody;
    exp->length = length;
    exp->bound_variables = bound_variables;
    SNPL_register_item(exp, SNPL_reference_type("expression"));  //~ wasty
//XXX cout << "returning constructed expression." << endl;
    return exp;
  }
}



void *SNPL_interpret_expression(
  char *body,
  int length,
  int bound_variables)
{
//XXX cout << "bar: " << endl;
//XXX view_bytes(body, length);  cout.flush();
  SNPL_expression *exp
    = SNPL_construct_expression(body, length, bound_variables);
//XXX cout << "interpret_expression:" << endl;
//XXX view_bytes((void *) exp->body, exp->length);
  if (exp != NULL)
    return SNPL_reduce(NOARG, exp);
  else
    return NULL;
}



void SNPL_expression_destructor(SNPL_expression *exp)
{
  // Free SNPL_primitive_shell (if any)
  if (*exp->body == (char) REFERENCE)
    free(*((void **) (exp->body+1)));

  free(exp->body);
  free(exp);
}



/*- end of file --------------------------------------------------------------*/
