/*+

  primive_shell.cpp

  Defines a structure SNPL_primitive_shell for embedding partially-resolved
  SNPL_cpp_primitives in lambda expressions.  As arguments are passed to a shell,
  they associate to the primitives parameters from "back to front", e.g.
  for a shell wrapping primitive "foo(int, double)", the first argument passed is
  expected to be of type "double" (if it isn't SNPL will immediately call its
  garbage collector and terminate).

  author: Josh Shinavier
  last edited: 12/14/04

*/

#include "snpl.h"
#include <cstdio>
#include <iostream>
using namespace std;

//SNPL_data_type *expression_type_PRIMITIVE_SHELL_CPP;



void *SNPL_apply_primitive(void *arg, SNPL_cpp_primitive *prim)
{
  SNPL_primitive_shell *shell
    = (SNPL_primitive_shell *) malloc(sizeof(SNPL_primitive_shell));

  shell->prim = prim;
  shell->filled = 0;
  shell->arguments = (void **) malloc((1+prim->parameters)*sizeof(void *));
//cout << "bottom." << endl; cout.flush();
  return SNPL_apply_shell(arg, shell);
}



/*
  Possible outcomes of applying a shell to an argument:

  1) The argument is of the wrong type
        Output an error message and bail.

  2) The argument resolve's the shell's primitive's last parameter
        Return the result of applying the primitive to its complete argument
	    array.

  3) The argument does not resolve the primitive's last parameter
        Return an expression containing a new shell in which one additional
	    parameter has been resolved.
*/
void *SNPL_apply_shell(void *arg, SNPL_primitive_shell *shell)
{
  void *r = NULL;

  SNPL_cpp_primitive *prim = shell->prim;
  int i = prim->parameters - shell->filled;

  // Check whether the argument's type agrees with the primitive's corresponding parameter type.
  SNPL_data_type *dt = SNPL_find_type(arg);
//cout << "pfui: " << dt->name << endl; cout.flush();
//cout << "wie: " << prim->parameter_types[i]->name << endl; cout.flush();
  // If so, add the argument to the shell's parameter array.
  if ((prim->parameter_types[i] == (SNPL_data_type *) VOID_TYPE)||(dt == prim->parameter_types[i]))
  {
    shell->arguments[i] = arg;
    shell->filled++;

    // If, furthermore, this completes the primitives parameter array, then
    // execute it.
    if (shell->filled == prim->parameters)
    {
//cout << "Executing primitive (filled = " << shell->filled << ")" << endl; cout.flush();
      SNPL_execute_cpp_primitive(prim, shell->arguments);
      SNPL_data_type *return_type
        = prim->parameter_types[0];
      if (return_type == (SNPL_data_type *) NORETURN)
        r = shell->arguments[1];
      else {
        r = shell->arguments[0];
	// If the return type is (void *), the returned item is assumed to be a
	// pre-existing, registered item from the data set.
	if (return_type != (SNPL_data_type *) VOID_TYPE)
	  SNPL_register_item(r, return_type);  
      }
    }

    // If we don't obtain a result, copy the more complete shell and make it into
    // an expression.
    else {
      SNPL_primitive_shell *newshell
        = (SNPL_primitive_shell *) malloc(sizeof(SNPL_primitive_shell));
      newshell->prim = shell->prim;
      newshell->filled = shell->filled;
      newshell->arguments = (void **) malloc((1+newshell->prim->parameters)*sizeof(void *));
      memcpy(newshell->arguments, shell->arguments, sizeof(void *)*(1+shell->prim->parameters));
      SNPL_expression *e
        = (SNPL_expression *) malloc(sizeof(SNPL_expression));
      e->body = (char *) malloc(5);
      e->length = 5;
      e->bound_variables = newshell->prim->parameters - newshell->filled;
      char *s = e->body;
      *s = (char) REFERENCE;
      s++;
      char *s2 = (char *) &newshell;
      for (int i=0; i<4; i++)
      {
        *s = *s2;
        s++; s2++;
      }
      // Register the new expression.
      r = (void *) e;
      //SNPL_register_item(r, expression_type_PRIMITIVE_SHELL_CPP);    
      /* wasty! */ SNPL_register_item(r, SNPL_reference_type("expression"));
    }

    // Either way, rewind the original shell's argument cursor.
    shell->filled--;
  }

  // If not, handle the exception.
  else {
cout << "dt = " << (int) dt << endl; cout.flush();
cout << "prim->parameter_types[" << i << "] = " << (int) prim->parameter_types[i] << endl;  cout.flush();
    char *name1 = SNPL_get_type_name(dt), 
         *name2 = SNPL_get_type_name(prim->parameter_types[i]);
    cout << "Fatal error in SNPL_apply_shell(...):"
         << " type mismatch in application of primitive \""
         << prim->name << "\" (received an item of type \""
         << name1 << "\", was expecting \""
         << name2 << "\")." << endl;
    delete name1, name2;
    SNPL_failandbail();
  }

  return r;
}



void SNPL_print_shell(SNPL_primitive_shell *shell, SNPL_associative_array *rev_dict)
{
  void *arg;
  char *name;
  int i, params = shell->prim->parameters;
//cout << "SNPL_print_shell:\n\tparams = "
//     << params << "\n\tfilled = "
//     << shell->filled << endl;
  for (i=params-shell->filled+1; i<=params; i++)
  {
    arg = shell->arguments[i];
    if (i != (params-shell->filled+1))
      //printf("(");
    //else
      printf(" (");
    SNPL_print(arg, rev_dict);
  }
  printf(" %s", shell->prim->name);
  for (int i=0; i<shell->filled; i++)
    printf(")");
}



void SNPL_shell_destructor(SNPL_primitive_shell *shell)
{
  free(shell->arguments);
  free(shell);
}



/*- end of file --------------------------------------------------------------*/

