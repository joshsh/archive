/*+
 * registry.cpp
 *
 * Contains objects and functions to keep track of the various runtime data
 * types, function primitives, and first-class values.
 *
 * author: Josh Shinavier
 * last edited: 12/9/04
 */

#include "snpl.h"
#include <cstring>
using namespace std;



/** Global variables ######################################################### */

SNPL_data_type *expression_type_REGISTRY_CPP,
               *data_type_type_REGISTRY_CPP,
	       *cpp_primitive_type_REGISTRY_CPP;

/*
  These are for referencing a type by its id or a primitive
  by its address.
*/
SNPL_lookup_table *type_registry,
                  *primitive_registry ;

/*
  A monster associative array which binds each first-class item in the data set
  with its type.
  This is a temporary solution; for large data sets it may not be possible to
  find a single block of memory large enough to hold an eight-byte field for
  every data item, so I'll have to break this down into a tree of some sort.
*/
SNPL_associative_array *item_to_type;

/*
  These serve the registry destructor, and allow the creation of type and
  primitives dictionaries.
*/
SNPL_array *registered_types;
SNPL_array *registered_primitives;



/** Function definitions ##################################################### */



void SNPL_registry_constructor()
{
  type_registry
    = new SNPL_lookup_table(REGISTRY_STARTSIZE,
                               REGISTRY_EXPANSION_FACTOR);
  primitive_registry
    = new SNPL_lookup_table(REGISTRY_STARTSIZE,
                               REGISTRY_EXPANSION_FACTOR);
  registered_types
    = new SNPL_array(REGISTRY_STARTSIZE,
                     REGISTRY_EXPANSION_FACTOR);
  registered_primitives
    = new SNPL_array(REGISTRY_STARTSIZE,
                     REGISTRY_EXPANSION_FACTOR);
  item_to_type
    = new SNPL_associative_array(REGISTRY_STARTSIZE,
                                 REGISTRY_EXPANSION_FACTOR,
				 REGISTRY_SPARSITY_FACTOR);
}



/*
  To be called upon termination of the program.
*/
void SNPL_registry_destructor()
{
  /*
    Destroy all items in the data set, including all primitives and data types.
  */
  void **all_items = item_to_type->condense(); 
  for (int i=0; i<item_to_type->size(); i++)
  {
    SNPL_data_type *dt = (SNPL_data_type *) all_items[(i*2)+1];

    // Save data types for last, else we'll lose their destructors.
    if (dt != data_type_type_REGISTRY_CPP)
        dt->destructor(all_items[i*2]);
  }
  delete all_items;
  
  /*
    Destroy data types.
  */
  for (int i=0; i<registered_types->size(); i++)
    data_type_type_REGISTRY_CPP->destructor(registered_types->get(i));
    //SNPL_data_type_destructor(
      //(SNPL_data_type *) registered_types->get(i));
  delete registered_types;

  /*
    Destroy primitives.
  */
  //for (int i=0; i<registered_primitives->size(); i++)
  //  SNPL_cpp_primitive_destructor(
  //    (SNPL_cpp_primitive *) registered_primitives->get(i));
  delete registered_primitives;

  /*
    Destroy type, primitive and data registries.
  */
  delete type_registry;
  delete primitive_registry;
  delete item_to_type;
}



void SNPL_register_type(SNPL_data_type *dt)
{
  if (dt == NULL) {
    cout << "Error in SNPL_register_type: null type reference." << endl;
    SNPL_failandbail();
  }
  type_registry->add((void *) dt, dt->name);
  registered_types->enqueue((void *) dt);
//cout << "registering " << dt->name << " (" << (int) dt << ")" << endl;  
  if (dt->preserializer != NULL)
    SNPL_assign_type_precedence(dt);
    
  if (!strcmp(dt->name, "expression"))
    expression_type_REGISTRY_CPP = dt;
  else if (!strcmp(dt->name, "data_type"))
    data_type_type_REGISTRY_CPP = dt;
  else if (!strcmp(dt->name, "cpp_primitive"))
    cpp_primitive_type_REGISTRY_CPP = dt;
}



// Note: the cpp_primitive type needs to be registered before any primitives.
void SNPL_register_primitive(SNPL_cpp_primitive *prim)
{
  if (prim == NULL) {
    cout << "Error in SNPL_register_primitive: null primitive reference." << endl;
    SNPL_failandbail();
  }
  primitive_registry->add((void *) prim, prim->name);
  registered_primitives->enqueue((void *) prim);

  item_to_type->add((void *) prim,
    (void *) cpp_primitive_type_REGISTRY_CPP);
}



void SNPL_register_item(void *p, SNPL_data_type *dt)
{
//cout << "dt = " << (int) dt << endl;
  if (p == NULL) {
    cout << "Error in SNPL_register_item: null item reference." << endl;
    SNPL_failandbail();
  }
  if (dt == NULL) {
    cout << "Error in SNPL_register_item: null type reference." << endl;
    SNPL_failandbail();
  }
  //if (dt != GENERIC_TYPE)
    item_to_type->add(p, (void *) dt);
  //else if (item_to_type->lookup(p) == NULL)
  //  item_to_type->add(p, GENERIC_TYPE);
}



/*
  An alternative provided for external modules to which we don't want to grant
  direct access to the data types.
*/
void SNPL_register_item(void *p, char *type_name)
{
  if (p == NULL) {
    cout << "Error in SNPL_register_item: null item reference." << endl;
    SNPL_failandbail();
  }
//cout << "Type name: " << type_name << endl; cout.flush();
  SNPL_data_type *dt = SNPL_reference_type(type_name);
  if (dt == NULL) {
    cout << "Error in SNPL_register_item: type does not exist." << endl;
    SNPL_failandbail();
  }
//cout << "here" << endl; cout.flush();
  item_to_type->add(p, (void *) dt);
//cout << "whee" << endl; cout.flush();
}



SNPL_data_type *SNPL_reference_type(char *name)
{
  return (SNPL_data_type *) type_registry->lookup(name);
}



SNPL_cpp_primitive *SNPL_reference_primitive(char *name)
{
  return (SNPL_cpp_primitive *) primitive_registry->lookup(name);
}



/*
  Look up an item's data type.
*/
SNPL_data_type *SNPL_find_type(void *p)
{
  SNPL_data_type *dt = (SNPL_data_type *) item_to_type->lookup(p);
  if (dt != NULL)
    return dt;
  else {
    cout << "Error in SNPL_find_type: there is no type associated with address " << (int) p << "." << endl;
    SNPL_failandbail();
  }
}



void SNPL_unregister_item(void *p)
{
  item_to_type->remove(p);
}



void SNPL_register_all_types()
{
  //SNPL_data_type *dt = SNPL_reference_type("data_type");
  for (int i=0; i<registered_types->size(); i++)
    SNPL_register_item(
      registered_types->get(i), data_type_type_REGISTRY_CPP);
}



// Temporary
void SNPL_add_all_primitives(void *dict0)
{
  SNPL_lookup_table *dictionary
    = (SNPL_lookup_table *) dict0;
  for (int i=0; i<registered_primitives->size(); i++) {
    SNPL_cpp_primitive *prim
      = (SNPL_cpp_primitive *) registered_primitives->get(i);
    dictionary->add((void *) prim, prim->name);
  }
}
// Temporary
void SNPL_add_all_data_types(void *dict0)
{
  SNPL_lookup_table *dictionary
    = (SNPL_lookup_table *) dict0;
  for (int i=0; i<registered_types->size(); i++) {
    SNPL_data_type *dt
      = (SNPL_data_type *) registered_types->get(i);
    dictionary->add((void *) dt, dt->name);
  }
}



/*- end of file --------------------------------------------------------------*/
