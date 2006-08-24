/*+
 * deserializer.h
 *
 * The graph deserializer for SNPL data sets.
 *
 * author: Josh Shinavier
 * last edited: 12/23/04
 */

#ifndef DESERIALIZER_H
#define DESERIALIZER_H



#include <cstring>

struct reference_hole {
  char *s;
  int item_index;
  struct SNPL_data_type *dt;  // {dt} and {s} are mutually exclusive
  void **location;
};

SNPL_array *reference_holes;



/**
 * For a reference to an item by its index and the index of its type.
 * This is only to be called during graph deserialization.
 */
void *SNPL_decode_item_reference(char *s)
{
  void *p;
  char *cur = s, ch;
  while (*cur != '.')
    cur++;
  ch = *cur;
  *cur = '\0';
  cur++;
  SNPL_array *array = (SNPL_array *) items_by_type->get(atoi(s));
  if (array != NULL)
    p = array->get(atoi(cur));
  else
    p = NULL;
  cur--;
  *cur = ch;
  return p;
}



/**
 * For a reference to an item whose type can be assumed.
 * This is only to be called during graph deserialization.
 */
void *SNPL_decode_item_reference(SNPL_data_type *dt, int item_index)
{
  return ((SNPL_array *) items_by_type->get((int) type_indices->lookup((void *) dt)-1))->get(item_index);
}



/**
 * This is only to be called during graph deserialization.
 */
void SNPL_add_reference_hole(char *s, void **location)
{
  reference_hole *hole = (reference_hole *) malloc(sizeof(reference_hole));
  hole->dt = NULL;
  hole->s = strdup(s);
  hole->location = location;
  reference_holes->enqueue((void *) hole);
}



/**
 * This is only to be called during graph deserialization.
 */
void SNPL_add_reference_hole(SNPL_data_type *dt, int item_index, void **location)
{
  reference_hole *hole = (reference_hole *) malloc(sizeof(reference_hole));
  hole->s = NULL;
  hole->dt = dt;
  hole->item_index = item_index;
  hole->location = location;
  reference_holes->enqueue((void *) hole);
}



/**
 *
 */
//### You need to build error handling into this function
void *SNPL_graph_deserializer(ifstream &in)
{
  /* Will include a reference to each data type appearing in the graph */
  SNPL_array *data_types
    = new SNPL_array(INIT_MAX_DATA_TYPES,0);

  /* An array of arrays to keep track of the objects themselves */
  items_by_type = new SNPL_array(INIT_MAX_DATA_TYPES,0);

  SNPL_array *member_types
    = new SNPL_array(INIT_MAX_MEMBER_TYPES,0);

  /* Allows SNPL_decode_item_reference to find a type's index */
  type_indices = new SNPL_associative_array(INIT_MAX_DATA_TYPES*2,2,2);

  /* By the end of the main loop this will contain a description of all
     missing references in the data set, which can then be filled in after
     each identifier (e.g. "10.31" for item #31 of type #10) resolves to an item */
  reference_holes = new SNPL_array(100, 2);  
  
  void *cur;
  SNPL_data_type *cur_type;
  int cur_type_index=0, total_items_of_type;
  SNPL_array *item_array;

  char *s = new char[MAX_STRLEN];

  // Get the root item's type index from the header.
  int root_typeindex;
  in >> s >> s;
  char *ch = s;
  while (*ch != '.')
    ch++;
  *ch = '\0';
  root_typeindex = atoi(s);

  // Get past the first type index.
  in >> s;

  // Read the file and build or reference all objects and data types in the
  // graph.
  while (*s != '}')
  {
    /* Get type name */
    in >> s;
//XXX cout << "s = " << s << endl; cout.flush();
    /* Use the type's name to look it up in the registry */
    char *s0 = SNPL_read_cstring(s);
    cur_type = SNPL_reference_type(s0);
    delete s0;
//XXX cout << "cur_type = " << cur_type->name << endl; cout.flush();
    /* Read the number of objects of this type, and create an array to
       hold them. */
    in >> total_items_of_type >> s;
    item_array
      = new SNPL_array(total_items_of_type,0);
    items_by_type->enqueue((void *) item_array);
//XXX cout << "zwillinge" << endl; cout.flush();
    /* Deserialize the individual objects */
    for (int i=0; i<total_items_of_type; i++)
    {
      // Get past the item's local id.
      in >> s;
//XXX cout << "Hennessey" << endl; cout.flush();
      /*
        Here is the moment of truth for the type's deserializer.
        A bad deserializer could lead to bad data, segmentation faults, etc.
      */
      cur = cur_type->deserializing_constructor(in);
//XXX cout << "Heineken" << endl; cout.flush();
      // Add this item to the type-specific array, so it can be resolved by
      // its integer index.
      item_array->enqueue(cur);
//XXX cout << "Feldschloesschen" << endl; cout.flush();
      // Bind the item with the appropriate data type.
      SNPL_register_item((void *) cur, cur_type);
//XXX cout << "registering item of type " << cur_type->name << endl;
//XXX cout << "value = " << (int) cur << endl;
    }
    in >> s;  // (get past the closing bracket)
//XXX cout << "here i am" << endl; cout.flush();
    data_types->enqueue((void *) cur_type);
//XXX cout << "mendicants" << endl; cout.flush();
    type_indices->add(
      (void *) cur_type,
      (void *) data_types->size());
//XXX cout << "Zanzibar" << endl; cout.flush();
    in >> s;
    if (!strcmp(s,"A)"))
      break;
    cur_type_index++;
//XXX cout << "Marxismus" << endl; cout.flush();
  }
  delete s;
//XXX cout << "Mt. Rushmore" << endl; cout.flush();
  // Deserialize the global associative array for this data set.
  SNPL_ternary_associative_array *local_TAA
    = (SNPL_ternary_associative_array *)
      SNPL_ternary_associative_array_deserializing_constructor(in);
  
  // Fill reference holes
  while (reference_holes->size()) {
  
    // Get the next reference_hole.
    reference_hole *hole = (reference_hole *) reference_holes->pop();
    
    // Use the reference_hole to fill in a missing reference in a preserializing item.
    if (hole->dt == NULL)
      *(hole->location) = SNPL_decode_item_reference(hole->s);
    else
      *(hole->location) = SNPL_decode_item_reference(hole->dt, hole->item_index);
      
    // Destroy the reference_hole.
    if (hole->s != NULL)
      delete hole->s;  // delete or free() ?
    free(hole);
  }
  delete reference_holes;  
  
  delete member_types;
  delete type_indices;
  
  void **condensed = local_TAA->condense(), **runner = condensed;
  for (int i=0; i<local_TAA->size(); i++) {
    void *key1 = *(runner++);
    void *key2 = *(runner++);
    void *target = *(runner++);
    SNPL_associate(key1, key2, target);
    //SNPL_associate(*(runner++), *(runner++), *(runner++));
  }
  delete condensed;
  delete local_TAA;

  void *root
    = ((SNPL_array *) items_by_type->get(root_typeindex))->get(0);

  while (items_by_type->size())
    delete (SNPL_array *) items_by_type->pop();
  delete items_by_type;
  delete data_types;

  return root;
}



#endif

/*- end of file --------------------------------------------------------------*/
