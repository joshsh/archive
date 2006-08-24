/*+

  cpp_primitive.cpp

  Defines the SNPL_cpp_primitive structure which permits C or C++ functions to be
  imported into and called within an instance of SNPL.

  author: Josh Shinavier
  last edited: 12/17/04

  Dilemma: what should the registry do with the items returned by primitives?
  I can't assume that they'll always be new items; for instance primitives
  drawn from preserializing types will tend to return items already
  recognized as first-class values.  So it seems I'll just have to register each
  item as it comes in.  For existing items, this will simply re-associate the
  item with its type every time it comes out the other end of a primitive... it's
  a little wasty, but then again so is dynamic typing.

*/

#include "snpl.h"
#include <cstdio>
#include <cstdlib>
using namespace std;



/*
"cstring input_str()"
"garbage_collect()"

Give this to bison...
(NULL|[TYPE]) SPACE NAME '(' ...
*/

SNPL_cpp_primitive *SNPL_cpp_primitive_constructor(
  char *definition,
  void (*location)())
{
  SNPL_cpp_primitive *prim
    = (SNPL_cpp_primitive *) malloc(sizeof(SNPL_cpp_primitive));
  prim->ptr = (void (*)()) location;  //# superfluous cast...

  // Crude, error-intolerant top-down parser for the definition string
  char *s = strdup(definition);//SNPL_copy_cstring(definition);
//XXX cout << "definition = " << definition << endl;
  prim->parameters = 0;
  char *first_space = s-1, *first_paren, *cur = s, *param_begin;

  // Find the position of the opening paren before the list of parameter types,
  // as well as of the space between the return type and the name (if there is a
  // return type).
  while (*cur != '(') {
    if (((int) *cur <= 32)&&(first_space < s))
      first_space = cur;
    cur++;
  }
  first_paren = cur;
  cur++;
  
  // Skip to the first parameter type
  // (If we were to allow primitives with no arguments, this could skip us to the closing paren).
  while ((int) *cur <= 32)
    cur++;
    
//cout << "*cur = " << *cur << endl;
  
  // Count the number of parameters.
  if (*cur != ')')
    prim->parameters++;
  while (*cur != ')') {
    if (*cur == ',')
      prim->parameters++;
    cur++;
  }
  
  prim->parameter_types
    = new SNPL_data_type *[1 + prim->parameters];
  SNPL_data_type **r = prim->parameter_types;
  
  // Record return type (use NORETURN if there is none).
  if (first_space > s) {
    *first_space = '\0';
    if (!strcmp(s,"void")) {
//XXX cout << "return type is VOID_TYPE" << endl;
      *r = (SNPL_data_type *) VOID_TYPE;
    } else {
      *r = SNPL_reference_type(s);
//XXX cout << "return type is " << (int) *r << " (s = " << s << ")" << endl;    
    }
    // Restore the char at first_space (not strictly necessary)
    *first_space = (char) 32;
  } else {
    *r = (SNPL_data_type *) NORETURN;
//XXX cout << "return type is NORETURN" << endl;    
  }
  
  // Ready for parameter types.
  r++;
  
  // Re-use first_space as the beginning of the primitive's name.
  first_space++;
  while ((int) *first_space <= 32)
    first_space++;
    
  // Get the primitive's name.
  *first_paren = '\0';
  prim->name = strdup(first_space);
  *first_paren = '(';  // Restore char at first_paren (not strictly necessary).
//XXX cout << "prim->name = " << prim->name << endl;
//SNPL_copy_cstring(first_space);
  first_paren++;  // first_paren is re-used as a pointer to the beginning of the next arg type
  while ((int) *first_paren <= 32)
    first_paren++;  
  cur = first_paren;
  param_begin = cur;
  
//echo("almostthere");
  char ch;
  while (true) {
    cur++;
    if (*cur == '\0')
      break;
    if ((*cur == ')')||(*cur == ',')) {
      ch = *cur;
      *cur = '\0';
      
      // Record argument type.
      if (!strcmp(param_begin,"void")) {
//XXX cout << "arg type is VOID_TYPE" << endl;
        *r = (SNPL_data_type *) VOID_TYPE;
      } else {
        *r = SNPL_reference_type(param_begin);
//XXX cout << "arg type is " << (int) *r << " (param_begin = " << param_begin << ")" << endl;
      }
      
      r++;
      if (ch == ')')
        break;
      else {
        cur++;
        while ((*cur != '\0')&&((int) *cur <= 32))
          cur++;
        param_begin = cur;
        if (*cur == '\0')
          break;
      }
    }
  }

//echo("finish");
//SNPL_print_cpp_primitive(prim);
//printf("\n");

  delete s;
  return prim;
}



void SNPL_cpp_primitive_destructor(SNPL_cpp_primitive *prim)
{
  delete prim->name;
  delete prim->parameter_types;
  free(prim);
}



/**
 * The switch statement is a bit of a cumbersome way to handle function calls,
 * but this approach allows many more C/C++ functions to be directly wrapped by
 * SNPL_cpp_primitive's (i.e. without the need for function interfaces) than would
 * otherwise be possible.
 */
void SNPL_execute_cpp_primitive(SNPL_cpp_primitive *f, void **parameters)
{
//cout << "Executing primitive \"" << f->name << "\"." << endl; cout.flush();
  // Note: it might be quicker to put these in an array,
  // rather than a switch statement.  How much time, relatively,
  // does it take to call a function?
  switch (f->parameters) {
    case 0:
      parameters[0] = ((void *(*)()) f->ptr)();
      break;
    case 1:
      parameters[0] = ((void *(*)(void *)) f->ptr)(
        parameters[1]);
      break;
    case 2:
      parameters[0] = ((void *(*)(void *, void *)) f->ptr)(
        parameters[1], parameters[2]);
      break;
    case 3:
      parameters[0] = ((void *(*)(void *, void *, void *)) f->ptr)(
        parameters[1], parameters[2], parameters[3]);
      break;
    case 4:
      parameters[0] = ((void *(*)(void *, void *, void *, void *)) f->ptr)(
        parameters[1], parameters[2], parameters[3], parameters[4]);
      break;    
    case 5:
      parameters[0] = ((void *(*)(void *, void *, void *, void *, void *)) f->ptr)(
        parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
      break;     
    default:
      // Exit with error message
      cout << "Error in SNPL_execute_cpp_primitive: primitive \""
           << f->name << "\" takes too many ("
	   << f->parameters << ") arguments." << endl;
      SNPL_failandbail();
  }
}



void SNPL_print_cpp_primitive(SNPL_cpp_primitive *prim)
{
  SNPL_data_type **ptypes = prim->parameter_types;

  // Print return type.
  if (ptypes[0] != (SNPL_data_type *) NORETURN) {
    if (ptypes[0] == (SNPL_data_type *) VOID_TYPE)
	  printf("void ");
	else
	  printf("%s ", ptypes[0]->name);
  }

  // Print the name of the primitive.
  printf("%s(", prim->name);

  // Print the name of the data type of its first parameter (if any).
  if (ptypes[1] != (SNPL_data_type *) VOID_TYPE)
    printf("%s", ptypes[1]->name);
  else
    printf("void");

  // Print the name of the data types of its other parameters (if any).
  for (int i=2; i<=prim->parameters; i++) {
    if (ptypes[i] != (SNPL_data_type *) VOID_TYPE)
      printf(", %s", ptypes[i]->name);
    else
      printf(", void");
  }
  printf(")");
}



/*- end of file --------------------------------------------------------------*/
