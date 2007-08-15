/*+
 * universe.h
 *
 * A class for creating, loading, manipulating and saving data sets either
 * inputted by a human user, or generated according to commands inputted by a
 * human user.
 *
 * author: Josh Shinavier
 * last edited: 6/27/04
 */

//class lookup_table;  //8{in-charge, not-in-charge} 
 
class universe : item
{
  static class classe *item_classe;

  //class classe **classes;
  //class void_array *classes, *functions;
  class linked_list **classe_instances;
  
  char *universe_name;

  class lookup_table *special_references;
  
  void construct(char *);

  // This is a helper array to reduce the time complexity of the (linked-list
  // intensive) deserialize routine from O(n^2) to O(n).
  //
  class item ***deserialization_array;

public:

  universe(int);

  // Create a new (empty) universe
  //
  universe(char *);

  // Load a universe from a file
  //
  universe(ifstream &);

  ~universe();  //Remember to make this virtual if deriving from "item".

  // This should eventually be made private.
  //
  int add(class item *, class classe *);

  // Look up an item by its address (used in deserialization)
  //
  class item *reference(char *);

  class item *reference_local(int, char *);

  class item *reference_special(char *);
  
  void add_special_reference(class item *, char *);
  
  void display_special_references();
  
  //Note: at present, "universe" is not derived from "item", however at some
  //      point it probably will be -- then these two functions will
  //      need to be made virtual.
  //
  virtual void serialize(ofstream &);
  virtual void deserialize(ifstream &);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static universe* constructor_wrapper(int id0) { return new universe(id0); }

};

classe *universe::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
