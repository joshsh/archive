/*+
 * U.h
 * Includes and macro definitions.
 *
 * author: Josh Shinavier
 * last edited: 6/27/04
 */


#define DEBUG
 
#define UNIVERSE_DEBUG
#define CLASSE_DEBUG
#define FUNCTION_DEBUG

#define CASTE_DEBUG

#define ITEM_DEBUG

#define ARRAY_DEBUG
#define ASCII_DEBUG
#define INTEGER_TENSOR_DEBUG
#define REAL_TENSOR_DEBUG
#define LIST_DEBUG

#define COLOR_DEBUG
#define FRAME_DEBUG
#define GRID_DEBUG
#define PAPER_DEBUG

#define FUZZY_SET_DEBUG
#define FUZZY_SET_FAMILY_DEBUG
#define FUZZY_OUTPUT_FAMILY_DEBUG
#define FLE_DEBUG

#define ARGUMENT_DEBUG  //
#define FUNCTION_DEBUG  //
#define PROCEDURE_DEBUG

#define LOOKUP_TABLE_DEBUG
//*/

//---------------------------------

#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

//---------------------------------

#include "include/utilities/void_array.h"
#include "include/utilities/hash_table.h"

void_array *classes, *functions;

class classe *identify_classe(char *);
class function *identify_function(char *);
class function *identify_function(void *);

//---------------------------------

#include "include/item.h"
#include "include/node.h"
#include "include/linked_list.h"
#include "include/list.h"
#include "include/array.h"
#include "include/integer_tensor.h"
#include "include/real_tensor.h"
#include "include/classe.h"
#include "include/ascii.h"
#include "include/universe.h"
#include "include/caste.h"

#include "include/graphics/color24.h"
#include "include/graphics/color.h"
#include "include/frame.h"
#include "include/grid.h"

#include "include/fuzzy/fuzzy_set.h"
#include "include/fuzzy/fuzzy_set_family.h"
#include "include/fuzzy/fuzzy_output_family.h"
#include "include/fuzzy/FLE.h"

#include "include/argument.h"
#include "include/function.h"
#include "include/procedure.h"

#include "include/lookup_table.h"

//---------------------------------

#include "include/item.c"
#include "include/node.c"
#include "include/linked_list.c"
#include "include/list.c"
#include "include/array.c"
#include "include/integer_tensor.c"
#include "include/real_tensor.c"
#include "include/classe.c"
#include "include/ascii.c"
#include "include/universe.c"
#include "include/caste.c"

#include "include/graphics/color24.c"
#include "include/graphics/color.c"
#include "include/frame.c"
#include "include/grid.c"

#include "include/fuzzy/fuzzy_set.c"
#include "include/fuzzy/fuzzy_set_family.c"
#include "include/fuzzy/fuzzy_output_family.c"
#include "include/fuzzy/FLE.c"

#include "include/argument.c"
#include "include/function.c"
#include "include/procedure.c"

#include "include/lookup_table.c"

//---------------------------------

#include "include/basic_functions.h"



classe *identify_classe(char *s)
{
  for (int i=0; i<classes->size(); i++) {
    if (!strcmp(s, ((classe *) classes->get(i))->name()))
      return (classe *) classes->get(i);
  }
  return NULL;
}



function *identify_function(char *s)
{
  for (int i=0; i<functions->size(); i++) {
    if (!strcmp(s, ((function *) functions->get(i))->name()))
      return (function *) functions->get(i);
  }
  return NULL;
}



function *identify_function(void *ptr)
{
  for (int i=0; i<functions->size(); i++) {
    if (ptr == (void *) ((function *) functions->get(i))->ptr())
      return (function *) functions->get(i);
  }
  return NULL;
}



void create_classes()
{
  #ifdef DEBUG
    cout << "<> void create_classes()" << endl;
    cout.flush();
  #endif

  classes = new void_array(16);
  int i=0;
  classes->append((void *) new classe(
    i++,
    "item",
    (item*(*)(int)) item::constructor_wrapper,
    (void (*)(classe *)) item::set_classe));
  classes->append((void *) new classe(
    i++,
    "list",
    (item*(*)(int)) list::constructor_wrapper,
    (void (*)(classe *)) list::set_classe));
  classes->append((void *) new classe(
    i++,
    "array",
    (item*(*)(int)) array::constructor_wrapper,
    (void (*)(classe *)) array::set_classe));
  classes->append((void *) new classe(
    i++,
    "integer_tensor",
    (item*(*)(int)) integer_tensor::constructor_wrapper,
    (void (*)(classe *)) integer_tensor::set_classe));
  classes->append((void *) new classe(
    i++,
    "real_tensor",
    (item*(*)(int)) real_tensor::constructor_wrapper,
    (void (*)(classe *)) real_tensor::set_classe));
  classes->append((void *) new classe(
    i++,
    "ascii",
    (item*(*)(int)) ascii::constructor_wrapper,
    (void (*)(classe *)) ascii::set_classe));
  classes->append((void *) new classe(
    i++,
    "color",
    (item*(*)(int)) color::constructor_wrapper,
    (void (*)(classe *)) color::set_classe));
  classes->append((void *) new classe(
    i++,
    "frame",
    (item*(*)(int)) frame::constructor_wrapper,
    (void (*)(classe *)) frame::set_classe));
  classes->append((void *) new classe(
    i++,
    "grid",
    (item*(*)(int)) grid::constructor_wrapper,
    (void (*)(classe *)) grid::set_classe));
  classes->append((void *) new classe(
    i++,
    "fuzzy_set",
    (item*(*)(int)) fuzzy_set::constructor_wrapper,
    (void (*)(classe *)) fuzzy_set::set_classe));
  classes->append((void *) new classe(
    i++,
    "fuzzy_set_family",
    (item*(*)(int)) fuzzy_set_family::constructor_wrapper,
    (void (*)(classe *)) fuzzy_set_family::set_classe));
  classes->append((void *) new classe(
    i++,
    "fuzzy_output_family",
    (item*(*)(int)) fuzzy_output_family::constructor_wrapper,
    (void (*)(classe *)) fuzzy_output_family::set_classe));
  classes->append((void *) new classe(
    i++,
    "FLE",
    (item*(*)(int)) FLE::constructor_wrapper,
    (void (*)(classe *)) FLE::set_classe));
  classes->append((void *) new classe(
    i++,
    "universe",
    (item*(*)(int)) universe::constructor_wrapper,
    (void (*)(classe *)) universe::set_classe));
  classes->append((void *) new classe(
    i++,
    "lookup_table",
    (item*(*)(int)) lookup_table::constructor_wrapper,
    (void (*)(classe *)) lookup_table::set_classe));  
  classes->append((void *) new classe(
    i++,
    "procedure",
    (item*(*)(int)) procedure::constructor_wrapper,
    (void (*)(classe *)) procedure::set_classe));
  classes->append((void *) new classe(
    i++,
    "caste",
    (item*(*)(int)) caste::constructor_wrapper,
    (void (*)(classe *)) caste::set_classe));  
  //return classes;
}



void create_functions()
{
  #ifdef DEBUG
    cout << "<> void create_functions()" << endl;
    cout.flush();
  #endif

  functions = new void_array(1);
  functions->append((void *) new function(input_new_ascii,
                                          "input_new_ascii",
		                          "r:universe w:ascii"));
}



/*- end of file --------------------------------------------------------------*/
