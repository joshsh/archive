/*+

  snpl.h

  Structure definitions, function prototypes and interspersed macro definitions.

  author: Josh Shinavier
  last edited: 1/7/04

*/

#ifndef SNPL_H
#define SNPL_H



#include "debug.h"

#include <fstream>
#include <iostream>
#include <cstring>

// for SNPL.cpp's print functions
#include "type/color.h"

/*
  Include all utility libraries.
*/
#include "util/array.h"
#include "util/associative_array.h"
#include "util/expanding_string.h"
#include "util/lookup_table.h"
#include "util/strutil.h"
#include "util/ternary_associative_array.h"

void *SNPL_ternary_associative_array_deserializing_constructor(ifstream &);

/**
  NO LONGER NECESSARY.  Just keep these notes about how to enable/disable SDL graphics.

  Note: if SDL is not available, it is also necessary to comment out the
  graphics types {color}, {frame}, {bitmap}, {key_map}, {SDL_Surface} and {GUI}
  in import.txt as well as all primitives which involve these types.
  Furthermore, the two lines for the "sparse" target in the Makefile must be
  truncated to exclude the graphics modules and the SDL binding.
*/
#define SDL_GRAPHICS



// STRUCTURE DEFINITIONS ########################################################

/**
  A SNPL_data_type is a collection of pointers to serialization and
  garbage-collection procedures for a data type.  It is bound up with the
  "CPP interface" scheme for importing data types from C/C++ to SNPL.
*/
struct SNPL_data_type
{
  char *name;
  void (*serializer)(void *, SNPL_associative_array *, SNPL_associative_array *,
ofstream &);  void *(*deserializing_constructor)(ifstream &);
  void (*destructor)(void *);
  void (*preserializer)(void *, SNPL_array *);
};



/**
  A SNPL_expression is a restricted lambda expression with a simple substitution
  mechanism and no scope.  Opening and closing "parentheses" are represented
  literally in an expression string, and control stack operations during beta-
  reduction.
*/
struct SNPL_expression
{
  int length;
  int bound_variables;
  char *body;
};



/**
  A SNPL_cpp_primitive ties a function pointer to an identifying "name" and an
  exact description of the parameter types and return type of an imported C or
  C++ function.

  Note: the first "parameter" is actually a place-holder for the function's
  return value, so the dimension of the {parameter_types} array is parameters+1.

  parameter_types[0] has the value NORETURN for void functions.  A value of
  VOID_TYPE as a return or parameter type anticipates a void pointer.
*/
struct SNPL_cpp_primitive
{
  char *name;
  void (*ptr)();  // Will be re-cast for each execution call.
  int parameters;

  SNPL_data_type **parameter_types;
};



/**
  A SNPL_primitive_shell provides the means for embedding partially-resolved
  SNPL_cpp_primitives in lambda expressions.  As arguments are passed to a shell,
  they associate to the primitives parameters from "back to front", e.g.
  for a shell wrapping primitive "foo(int, double)", the first argument passed is
  expected to be of type "double" (if it isn't SNPL will immediately call its
  garbage collector and terminate).
*/
struct SNPL_primitive_shell
{
  SNPL_cpp_primitive *prim;
  int filled;
  void **arguments;
};



// cpp_primitive.cpp ###########################################################

/**
  A value of NORETURN as the first entry in a cpp_primitive's parameter type
  array indicates a void function.
  A value of VOID_TYPE in a cpp_primitive's parameter type array indicates a
  void pointer as parameter or return type.
*/
#define NORETURN 1
#define VOID_TYPE 2

SNPL_cpp_primitive *SNPL_cpp_primitive_constructor(char *, void (*)());
void SNPL_cpp_primitive_destructor(SNPL_cpp_primitive *);

void SNPL_execute_cpp_primitive(SNPL_cpp_primitive *, void **);

void SNPL_print_cpp_primitive(SNPL_cpp_primitive *);



// data_type.cpp ###############################################################

SNPL_data_type *SNPL_data_type_constructor(
  char *,
  void (*)(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream
&),  void *(*)(ifstream &),
  void (*)(void *),
  void (*)(void *, SNPL_array *));

//void SNPL_data_type_destructor(SNPL_data_type *);



// executor.cpp ################################################################

/* Initial dimension and expansion factor for expression string builders */
#define EXPR_STRING_INITSIZE 16
#define EXPR_STRING_EXPANSION 2

void SNPL_executor_constructor();
void SNPL_executor_destructor();
SNPL_expanding_string *SNPL_reduce_subexpression(void *, char **);
void *SNPL_reduce_expression(void *, SNPL_expression *);



// expression.cpp ##############################################################

#define SIZEOF_POINTER 4

/*
  Should be made large enough to satisfy your most argument-hungry primitive.
*/
#define MAX_PARAMETERS 10

/* Expression string tokens (values must be unique, and the value of
   FIRST_PARAMETER must be highest) */
#define REFERENCE_HOLE 0  // Used in expression_itf.h
#define OPEN_SUBTERM 1
#define CLOSE_SUBTERM 2
#define REFERENCE 3
#define SELF_REFERENCE 4
#define FIRST_PARAMETER 5

/*
  flag value for SNPL_reduce_expression (in place of pointers):
  NOARG --> simplify the expression without resolving a parameter
    (for simplifying interpreted, user-inputted expressions which may or may not
     be functions)
*/
#define NOARG (void *) 1

struct SNPL_expression;
void print_expression(SNPL_expression *, SNPL_associative_array *);
SNPL_expression *SNPL_construct_expression(char *, int, int);
void *SNPL_interpret_expression(char *, int, int);
void SNPL_expression_destructor(SNPL_expression *);



// import.cpp ##################################################################

void SNPL_construct_types();
void SNPL_construct_primitives();



// primitive_shell.cpp #########################################################

void *SNPL_apply_primitive(void *, SNPL_cpp_primitive *);
void *SNPL_apply_shell(void *, SNPL_primitive_shell *);
void SNPL_print_shell(SNPL_primitive_shell *, SNPL_associative_array *);
void SNPL_shell_destructor(SNPL_primitive_shell *);



// registry.cpp ################################################################

#define REGISTRY_STARTSIZE	10
#define REGISTRY_EXPANSION_FACTOR	2
#define REGISTRY_SPARSITY_FACTOR	2

void SNPL_registry_constructor();
void SNPL_registry_destructor();

void SNPL_register_type(SNPL_data_type *);
SNPL_data_type *SNPL_reference_type(char *);

void SNPL_register_primitive(SNPL_cpp_primitive *);
SNPL_cpp_primitive *SNPL_reference_primitive(char *);

void SNPL_register_item(void *, SNPL_data_type *);
void SNPL_register_item(void *, char *);
SNPL_data_type *SNPL_find_type(void *);
void SNPL_unregister_item(void *);

void SNPL_register_all_types();

// Temporary
void SNPL_add_all_primitives(void *);
void SNPL_add_all_data_types(void *);



// serial.cpp ##################################################################

//~
#define MAX_STRLEN 100

/**
  A breadth-first traversal of the data graph has the advantage of an
  intuitive FIFO ordering of items in the output.  This may be commented
  out if you expect to be very tight on memory.
*/
#define BREADTH_FIRST_SERIALIZATION

void *SNPL_graph_deserializer(ifstream &);
void SNPL_graph_serializer(void *, ofstream &);
//void SNPL_graph_destructor(void *, SNPL_data_type *);

void *SNPL_decode_item_reference(char *);
void *SNPL_decode_item_reference(SNPL_data_type *, int);

void SNPL_add_reference_hole(char *, void **);
void SNPL_add_reference_hole(SNPL_data_type *, int, void **);



// SNPL.cpp ####################################################################

//! Housecleaning needed...

char *SNPL_get_type_name(SNPL_data_type *dt);

void echo(char *);
void SNPL_delete(void *);
void SNPL_failandbail();
int SNPL_find_type_precedence(SNPL_data_type *);
void SNPL_assign_type_precedence(SNPL_data_type *);

void SNPL_constructor();
void SNPL_destructor();

SNPL_ternary_associative_array *SNPL_get_global_associative_array();
void *SNPL_multiply(void *, void *);
void *SNPL_associate(void *, void *, void *);
void *SNPL_reduce(void *, void *);

//!
void SNPL_print(void *, SNPL_associative_array *);
void SNPL_print_iteminfo(void *, SNPL_associative_array *);

//!
void *SNPL_bool_constructor(bool);
void *SNPL_cstring_constructor(char *);
void *SNPL_char_constructor(char);
void *SNPL_int_constructor(int);
void *SNPL_double_constructor(double);

//!
void *SNPL_import_graph(char *);
void SNPL_export_graph(void *, char *);

//!
void *SNPL_scratch(void *);



#endif

/*- end of file --------------------------------------------------------------*/
