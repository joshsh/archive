/*+
 * serial.cpp
 *
 * Functions for reading and writing SNPL data graphs to and from formatted text
 * files.  Note that the graph serializer does not necessarily ###
 *
 * Note: all iterative (no recursive) implementations of graph algorithms here
 *
 * author: Josh Shinavier
 * last edited: 12/14/04
 */

#include "snpl.h"
using namespace std;



/* (arbitrary) initial dimensions for the expanding arrays */
#define INIT_MAX_DEPTH 16
#define INIT_MAX_DATA_TYPES 64
#define INIT_MAX_MEMBER_TYPES 8





// Unfortunately these have to be global (for the sake of
// SNPL_decode_item_reference()).  That, or you could pass them with each
// call to a type serializer
SNPL_array *items_by_type;
SNPL_associative_array *type_indices;

/*
  Prototypes go here.
*/
//void *SNPL_graph_deserializer(ifstream &);
//void SNPL_graph_destructor(void *, SNPL_data_type *);
//void SNPL_graph_serializer(void *, ofstream &);

#include "deserializer.h"
#include "serializer.h"



/**
 *
 *//*
void SNPL_graph_destructor(void *root,
                           SNPL_data_type *root_type)
{
  // here, the hash table serves as a "visited" flag for each object in the graph
  SNPL_associative_array *ht
    = new SNPL_associative_array(101,0,0);

  // For the graph traversal
  SNPL_array *data_stack
    = new SNPL_array(INIT_MAX_DEPTH,0);
  SNPL_array *type_stack
    = new SNPL_array(INIT_MAX_DEPTH,0);

  data_stack->push(root);
  type_stack->push((void *) root_type);

  void *cur;
  SNPL_data_type *cur_type;
  SNPL_internal_type_interface *internal_type_interface;
  SNPL_external_type_interface *external_type_interface;
  void **cur_child;
  SNPL_data_type **cur_child_type;

  // the graph traversal
  while ((cur = data_stack->pop()) != NULL)  // exit when the stack is empty
  {
    // get the object's data type
    cur_type = (SNPL_data_type *) type_stack->pop();

    // add a "visited" reference to the object in the hash table
    ht->add(cur, (void *) 1);

    if (cur_type->external)
    {
      // just call the appropriate destructor
      ((SNPL_external_type_interface *) cur_type->interface)->destructor(cur);
    }
    else
    {
      internal_type_interface
        = (SNPL_internal_type_interface *) cur_type->interface;

      cur_child = (void **) cur;
      cur_child_type = internal_type_interface->children;

      // push all unvisited children to the stacks
      for (int i=0; i<internal_type_interface->size; i++)
      {
        // we use a depth-first traversal here
        if ((*cur_child != NULL)&&(ht->lookup(*cur_child) != NULL))
	{
          data_stack->push(*cur_child);
          type_stack->push(*cur_child_type);
        }

        cur_child++;
        cur_child_type++;
      }
    }
  }

  delete data_stack; delete type_stack;
  delete ht;
}*/





/*- end of file --------------------------------------------------------------*/

