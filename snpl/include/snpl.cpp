/*+

  SNPL.cpp

  Additional includes, macro definitions and global functions.

  author: Josh Shinavier
  last edited: 12/14/04

*/

#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdio>
#include "snpl.h"
using namespace std;



void echo(char *s)
{
#ifdef DEBUG
  cout << s << endl;
  cout.flush();
#endif
}



char *SNPL_get_type_name(SNPL_data_type *dt)
{
  if (dt != NULL)
    return strdup(dt->name);
  else {
    cout << "Error in SNPL_get_type_name: null type reference." << endl;
    SNPL_failandbail();
  }
}



//===================
SNPL_ternary_associative_array *global_associative_array;

SNPL_ternary_associative_array *SNPL_get_global_associative_array()
{
  return global_associative_array;
}

void *SNPL_multiply(void *a, void *b)
{
  return global_associative_array->lookup(a,b);
}
void *SNPL_associate(void *a, void *b, void *c)
{
  global_associative_array->add(a, b, c);
}
//===================



SNPL_associative_array *serial_type_precedence;

SNPL_data_type *expression_type_SNPL_CPP,
	       *cpp_primitive_type_SNPL_CPP;



//### This will probably never be used.
// Note: eventually this should be made to apply only to external types.
// Right now it is serving the graph destructor in regard to the "children" of
// preserializing data types.
void SNPL_delete(void *p)
{
  SNPL_data_type *dt = SNPL_find_type(p);
  if (dt != NULL) {
    SNPL_unregister_item(p);
    dt->destructor(p);
  } else {
    //... destroy expression
  }
}



/*
 * It's important in what order the preserializing data types are handled
 * by the graph serializer.  A higher precedence indicates that items of a
 * particular data type should be listed out later rather than sooner
 * (otherwise when the file is loaded they might try to reference an item of a
 * type which has not yet been deserialized).
 * Note: the function will return -1 for non-preserializing data types.
 */
int SNPL_find_type_precedence(SNPL_data_type *dt)
{
  return (int) serial_type_precedence->lookup((void *) dt) - 1;
}



int stpindex = 0;

/*
 * Call this for each new preserializing data type in ascending order of
 * dependency.
 */
void SNPL_assign_type_precedence(SNPL_data_type *dt)
{
  serial_type_precedence->add(
    (void *) dt, (void *) (++stpindex));
}



/*
  This function MUST be called before any other function in the SNPL module.
*/
void SNPL_constructor()
{
  SNPL_registry_constructor();
  serial_type_precedence
    = new SNPL_associative_array(10,2,2);
  SNPL_construct_types();
  cpp_primitive_type_SNPL_CPP = SNPL_reference_type("cpp_primitive");
  expression_type_SNPL_CPP = SNPL_reference_type("expression");
  SNPL_construct_primitives();
  SNPL_executor_constructor(); // Must come after the type constructors.
  global_associative_array
    = new SNPL_ternary_associative_array(100,2,2);
}



/*
  End-of-execution garbage collector
*/
void SNPL_destructor()
{
  delete global_associative_array;
  delete serial_type_precedence;
  SNPL_executor_destructor();
  SNPL_registry_destructor();
}



void *SNPL_reduce(void *arg, void *op)
{
//XXX cout << "SNPL_reduce: op = " << (int) op << endl;
  SNPL_data_type *dt = SNPL_find_type(op);
//XXX cout << "SNPL_reduce: op type = " << SNPL_get_type_name(dt) <<" (" << (int) dt << ")" << endl;
  if (dt == expression_type_SNPL_CPP) {
//XXX cout << "SNPL_reduce: reducing expression." << endl;
    return SNPL_reduce_expression(
      arg, (SNPL_expression *) op);
  } else if (dt == cpp_primitive_type_SNPL_CPP) {
//XXX cout << "SNPL_reduce: applying primitive." << endl;
    return SNPL_apply_primitive(
      arg, (SNPL_cpp_primitive *) op);
  } else {
//XXX cout << "SNPL_reduce: applying associative product." << endl;
    return SNPL_multiply(arg, op);
  }
}



//! WARNING: Here we're using {malloc} whereas the deserializing constructors use {new}

//! Temporary
void *SNPL_bool_constructor(bool b)
{
  void *p = malloc(sizeof(bool));
  *((bool *) p) = b;
  SNPL_register_item(p, SNPL_reference_type("bool"));
  return p;
}
//! Temporary
void *SNPL_char_constructor(char ch)
{
  void *p = malloc(sizeof(char));
  *((char *) p) = ch;
  SNPL_register_item(p, SNPL_reference_type("char"));
  return p;
}
//! Temporary
void *SNPL_int_constructor(int i)
{
  void *p = malloc(sizeof(int));
  *((int *) p) = i;
  SNPL_register_item(p, SNPL_reference_type("int"));
  return p;
}
//! Temporary
void *SNPL_double_constructor(double d)
{
  void *p = malloc(sizeof(double));
  *((double *) p) = d;
  SNPL_register_item(p, SNPL_reference_type("double"));
  return p;
}
//! Temporary
void *SNPL_cstring_constructor(char *s)
{
  char *s2 = (char *) malloc(1+strlen(s));
  //SNPL_data_type *dt = SNPL_reference_type("cstring");
  SNPL_register_item((void *) s2, SNPL_reference_type("cstring"));
  return (void *) strcpy(s2, s);
}



// Temporary
void *SNPL_import_graph(char *filename)
{
  ifstream in;
  in.open(filename);
  void *r = SNPL_graph_deserializer(in);
  in.close();
  return r;
}
// Temporary
void SNPL_export_graph(void *p, char *filename)
{
  ofstream out;
  out.open(filename);
  SNPL_graph_serializer(p, out);
  out.close();
}



// Temporary
void SNPL_print(void *p, SNPL_associative_array *rev_dict)
{
//echo("anethole");
  char *name = (char *) rev_dict->lookup(p);
//echo("Markov");
  if (name != NULL) {
    printf("%s", name);
  } else {
    SNPL_data_type *dt = SNPL_find_type(p);
//echo("swell");
    if (dt == SNPL_reference_type("expression"))
      print_expression((SNPL_expression *) p, rev_dict);
    else if (dt == SNPL_reference_type("cstring"))
      cout << (char *) p;
    else if (dt == SNPL_reference_type("int"))
      cout << "#" << *((int *) p);
    else if (dt == SNPL_reference_type("double"))
      cout << "#d" << *((double *) p);
    else if (dt == SNPL_reference_type("char")) {
      char ch = *((char *) p);
      if (ch < 33)
        cout << "ascii(" << (int) ch << ")";
      else
        cout << "'" << ch << "'";
    }
    //#ifdef SDL_GRAPHICS
      else if (dt == SNPL_reference_type("color")) {
        char *s = ((SNPL_color *) p)->encode();
        cout << s;
        delete s;
      }
    //#endif
    else
      printf("%d", (int) p);
//echo("Dentrassi");
  }
}
// Temporary
/*void *SNPL_find_data_type(void *p)
{
  return (void *) SNPL_find_type(p);
}*/
// Temporary
char *ptr_to_hex(void *p)
{
  int n = (int) p;
  char *s = new char[11], *s2 = s+1;
  *s = '0'; *s2 = 'x'; s2++;
  int base = 0x10000000, div;
  while (base) {
    div = n/base;
    if (div < 10)
      *s2 = (char) div+48;
    else
      *s2 = (char) div-10+65;
    s2++;
    n -= div*base;
    base /= 16;
  }
  *s2 = '\0';
  return s;
}
// Like SNPL_print, but with additional type and location info
void SNPL_print_iteminfo(void *p, SNPL_associative_array *rev_dict)
{
//echo("zagreb");
  if (p == NULL)
    cout << "[0]";
  else {
//echo("yeoman");
    cout << "<";
//cout << "value = " << (char *) p << endl; cout.flush();
    SNPL_data_type *dt = SNPL_find_type(p);
//echo("firth");
    if (dt == SNPL_reference_type("expression")) {
      cout << "expression|"
           << ((SNPL_expression *) p)->bound_variables;
    } else
      cout << dt->name;
    cout << ">";
    char *name = (char *) rev_dict->lookup(p);
    if (name != NULL)
      cout << " " << name;
    #ifdef PRINT_LOCATION_INFO
    cout << " @";// << (int) p;
    char *hex = ptr_to_hex(p);
    cout << hex;
    delete hex;
    #endif
//echo("zymurgy");

    if (dt == SNPL_reference_type("expression")) {
      cout << ":\t";
      print_expression((SNPL_expression *) p, rev_dict);
    } else if (dt == SNPL_reference_type("cstring"))
      cout << ":\t" << (char *) p;
    else if (dt == SNPL_reference_type("int"))
      cout << ":\t#" << *((int *) p);
    else if (dt == SNPL_reference_type("double"))
      cout << ":\t#d" << *((double *) p);
    else if (dt == SNPL_reference_type("cpp_primitive")) {
      cout << ":\t";
      SNPL_print_cpp_primitive(
        (SNPL_cpp_primitive *) p);
    } else if (dt == SNPL_reference_type("bool"))
      cout << ":\t" << *((bool *) p);
    else if (dt == SNPL_reference_type("char")) {
      cout << ":\t";
      char ch = *((char *) p);
      if (ch < 33)
        cout << "ascii(" << (int) ch << ")";
      else
        cout << "'" << ch << "'";
    }
    //#ifdef SDL_GRAPHICS
      else if (dt == SNPL_reference_type("color")) {
        char *s = ((SNPL_color *) p)->encode();
        cout << ":\t" << s;
        delete s;
      }
    //#endif

//echo("zymotoxin");
  }
}




void SNPL_failandbail()
{
  SNPL_destructor();
  cout << "SNPL exited abnormally." << endl;
  exit(1);
}



/*
  A place to experiment with new primitives before actually importing them.
*/
void *SNPL_scratch(void *p)
{
  char ch;
  cout << "Input char: ";
  ch = fgetc(stdin);  //cin >> ch;
  cout << "." << endl;
  cout << "ch = " << ch << "." << endl;
  return p;
}



/*- end of file --------------------------------------------------------------*/
