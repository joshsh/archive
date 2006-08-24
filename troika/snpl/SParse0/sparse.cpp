/*+
 * sparse.cpp
 *
 * This is the C++ portion of the "SParse" programming environment. 
 *
 * author: Josh Shinavier
 * last edited: 12/28/04
 */

#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// For dictionary and reverse dictionary
#include "../include/util/lookup_table.h"
#include "../include/util/associative_array.h"

#include "../include/snpl.h"



/*
  A global lookup table to hold dictionary entries created by the programmer
  over the course of a SParse session.
  In general, the dictionary is not preserved between sessions.
*/
SNPL_lookup_table *dictionary;
SNPL_lookup_table *primitives_dictionary;
SNPL_lookup_table *data_types_dictionary;
/*
  For a reverse look-up of an item's name by its address (for SNPL_print()).
*/
SNPL_associative_array *reverse_dictionary;

SNPL_data_type *cstring_type_SPARSE_CPP;



extern "C"
{
  void yyparse();

  // Allows the bison module to construct and register character strings.
  void *SParse_cstring_constructor(char *s)
  {
    return SNPL_cstring_constructor(s);
  }

  // Allows the bison module to construct and register chars.
  void *SParse_char_constructor(char ch)
  {
    return SNPL_char_constructor(ch);
  }
  
  // Allows the bison module to construct and register boolean values.
  void *SParse_bool_constructor(int b)
  {
    return SNPL_bool_constructor(b == 1);
  }
    
  // Allows the bison module to construct and register ints.
  void *SParse_int_constructor(int i)
  {
    return SNPL_int_constructor(i);
  }

  // Allows the bison module to construct and register doubles.
  void *SParse_double_constructor(double d)
  {
    return SNPL_double_constructor(d);
  }

  // Allows the bison module to retrieve existing dictionary words.
  void *SParse_dictionary_lookup(char *id)
  {
    return dictionary->lookup(id);
  }

  /*
    Allows the bison module to add new words to the dictionary.
    Note: the last name given for an item is the one the reverse
    dictionary will bind it to.
  */
  void SParse_dictionary_add(void *p, char *id)
  {
    //void *p2 = dictionary->lookup(id);
    dictionary->add(p, id);
    //if (p2 != NULL)
    //  reverse_dictionary->remove(p2);
    //cout << "########] " << strdup(id) << " [#########" << endl;
    char *s = strdup(id);
    SNPL_register_item((void *) s, cstring_type_SPARSE_CPP);
    reverse_dictionary->add(p, (void *) s);
  }

  // Allows the bison module to interpret and reduce lambda terms via SNPL.
  void *SParse_interpret_expression(char *body, int length, int bound_variables)
  {
    return SNPL_interpret_expression(body, length, bound_variables);
  }
  
  // Allows the bison module to display lambda expressions, primitives and other data items.
  void SParse_print(void *p)
  {
    SNPL_print_iteminfo(p, reverse_dictionary);
  }

} // end extern "C"



int main(int argv, char *args[])
{
  /*
    Always call SNPL_constructor first, if only to avoid accidentally putting something
    before the call which needs to access a registry object.
  */
  SNPL_constructor();
  cstring_type_SPARSE_CPP = SNPL_reference_type("cstring");
  if (argv < 2) {
    //XXX cout << "Creating new dictionary." << endl;
    dictionary = new SNPL_lookup_table(0, 2);
    
    //XXX cout << "Filling the dictionary." << endl; cout.flush();
    
    primitives_dictionary = new SNPL_lookup_table(0, 2);
    data_types_dictionary = new SNPL_lookup_table(0, 2);
    SNPL_register_item((void *) dictionary, "lookup_table");
    SNPL_register_item((void *) primitives_dictionary, "lookup_table");
    SNPL_register_item((void *) data_types_dictionary, "lookup_table");

    // Give the user access to the run-time dictionaries.
    dictionary->add((void *) dictionary, "dictionary");
    dictionary->add((void *) primitives_dictionary, "primitives");
    dictionary->add((void *) data_types_dictionary, "data_types");

    // Give the user access to whatever C++ primitives were imported at compile time.
    SNPL_add_all_primitives(dictionary);
    SNPL_add_all_primitives(primitives_dictionary);
    SNPL_add_all_data_types(dictionary);
    SNPL_add_all_data_types(data_types_dictionary);
  
  } else {
    //XXX cout << "Loading dictionary." << endl;
    void *p = SNPL_import_graph(args[1]);
    if ((p == NULL)||(SNPL_find_type(p) != SNPL_reference_type("lookup_table"))) {
      cout << "Error in sparse.cpp: not a valid dictionary." << endl;
      SNPL_failandbail();
    } else
      dictionary = (SNPL_lookup_table *) p;
  }
   
  //XXX cout << "Building reverse dictionary." << endl; cout.flush();
  
  // Build the reverse dictionary (not directly accessible to the user).
  reverse_dictionary = new SNPL_associative_array(0, 2, 2);  
  // Just so it's garbage collected if there is a fatal error.
  SNPL_register_item((void *) reverse_dictionary, "associative_array");  
  char **all_entries = dictionary->list_all(), **cur = all_entries;

  for (int i=0; i<dictionary->size(); i++) {
    SNPL_register_item((void *) *cur, cstring_type_SPARSE_CPP);
    reverse_dictionary->add(dictionary->lookup(*cur), *cur);
    cur++;
  }
  delete all_entries;  // Individual entries are not being deleted as they've been given to reverse_dictionary.

  //XXX cout << "Parsing input." << endl; cout.flush();
  
  // Call the flex/bison interpreter.
  yyparse();

  /*
    Note: you don't need to delete the dictionaries, as they're first-class
    values (ergo, SNPL_destructor will delete them for you).
  */
  SNPL_destructor();
}



/*- end of file --------------------------------------------------------------*/
