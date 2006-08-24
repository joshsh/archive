/*+
 * serializer.h
 *
 * The graph serializer for data sets.
 *
 * author: Josh Shinavier
 * last edited: 12/22/04
 *
 * Note: removed the "internal type" mechanism on 11/21/04 as I don't foresee
 * re-introducing aggregate types except insofar as they're given an interface
 * and imported as external types.
 */

#ifndef SERIALIZER_H
#define SERIALIZER_H



//#include "snpl.h"

#define EDGE_ARRAY_STARTSIZE 0
#define EDGE_ARRAY_EXPANSION 2



SNPL_associative_array *get_edges_by_item(
  SNPL_ternary_associative_array *array)
{
  // A hash table in which to store collections of outbound edges from each item
  // appearing (in a key0 or key1 position) in {array}.
  SNPL_associative_array *edges_by_item
    = new SNPL_associative_array(100, 2, 2);

/*XXX*/ cout << "one" << endl; cout.flush();
  // Get a "condensed" version of the ternary associative array which we can
  // easily cycle through.
  void **compact_array = array->condense(),
    **cur = compact_array;
/*XXX*/ cout << "array->size() = " << array->size() << endl; cout.flush();
  // Build a hash table containing a pair of collections of outbound edges
  // from each key item in the original array.
  SNPL_array **edge_arrays;
  void *i0, *i1, *i2;
  for (int i=0; i<array->size(); i++)
  {
    i0 = *(cur++);
    i1 = *(cur++);
    i2 = *(cur++);
/*XXX*/ cout << "i0 = " << (int) i0 << endl;
/*XXX*/ cout << "i1 = " << (int) i1 << endl;
/*XXX*/ cout << "i2 = " << (int) i2 << endl;

    // For key0 and key1 positions
    for (int k=0; k<2; k++) {

      // Find the pair of collections of outbound edges associated with the
      // appropriate key.
      if (!k)
        edge_arrays = (SNPL_array **)
          edges_by_item->lookup(i0);
      else
	edge_arrays = (SNPL_array **)
          edges_by_item->lookup(i1);

      // If the collections do not yet exist, create a pair of empty
      // collections.
      if (edge_arrays == NULL) {
        edge_arrays = new SNPL_array *[2];
        edge_arrays[0] = new SNPL_array(
          EDGE_ARRAY_STARTSIZE, EDGE_ARRAY_EXPANSION);
        edge_arrays[1] = new SNPL_array(
          EDGE_ARRAY_STARTSIZE, EDGE_ARRAY_EXPANSION);
	if (!k)
	  edges_by_item->add(i0, (void *) edge_arrays);
	else
	  edges_by_item->add(i1, (void *) edge_arrays);
      }

      // The edge array is a collection of edges in which the item in question is
      // the corresponding key value.
/*XXX*/ cout << "Adding an edge." << endl;
      if (!k) {
        edge_arrays[0]->enqueue(i1);
        edge_arrays[0]->enqueue(i2);
      } else {
        edge_arrays[1]->enqueue(i0);
        edge_arrays[1]->enqueue(i2);
      }
    }
  }
  delete compact_array;
/*XXX*/ cout << "edges_by_item->size() = " << edges_by_item->size() << endl;
  return edges_by_item;
}

/*
make
./sparse
a (next (b associate)).
b (next (c associate)).
c (next (a associate)).
dictionary export.
*/



// Destroy edge array structures.
void delete_edges_by_item(SNPL_associative_array *edges_by_item)
{
  SNPL_array **edge_arrays;
  void **edges_by_item_compact
    = edges_by_item->condense();
  delete edges_by_item;
  void **cur = 1+edges_by_item_compact;
  for (int i=0; i<edges_by_item->size(); i++) {
    edge_arrays = (SNPL_array **) *cur;
    delete edge_arrays[0];
    delete edge_arrays[1];
    delete edge_arrays;
    cur += 2;
  }
  delete edges_by_item_compact;
}



/**
 * Note: with breadth-first serialization the root object always has the address 0.0
 */
void SNPL_graph_serializer(void *root, ofstream &out)
{
  SNPL_ternary_associative_array *reduced_TAA
    = new SNPL_ternary_associative_array(101,0,0);
    
  // Note: {edges_by_item} is not unaffected by the graph traversal.
  SNPL_associative_array *edges_by_item
    = get_edges_by_item(SNPL_get_global_associative_array());
    
  // To associate each object and each data type with an integer index
  SNPL_associative_array
    *item_indices = new SNPL_associative_array(101,0,0),
    *type_indices = new SNPL_associative_array(51,0,0);
  
  // For the graph traversal.  Note: if you're using the breadth-first
  // traversal these are actually queues rather than stacks.
  SNPL_array *data_stack
    = new SNPL_array(INIT_MAX_DEPTH,0);
  data_stack->push(root);

  // Will include a reference to each data type appearing in the graph
  SNPL_array *data_types
    = new SNPL_array(INIT_MAX_DATA_TYPES,0);
  int total_types=0;
  
  // An array of arrays to keep track of the objects themselves.
  // The index of each array corresponds to the index of the appropriate
  // data type in data_types.
  items_by_type  = new SNPL_array(INIT_MAX_DATA_TYPES,0); 
  
  // Misc. variables for the graph traversal.
  void *cur;
  SNPL_data_type *cur_type;
  int cur_type_index;
  SNPL_array *item_array;
  int j, k, maxj;
  void *key, *target;
  SNPL_array *er;
  SNPL_array **edge_arrays;

  // The graph traversal
  while ((cur = data_stack->pop()) != NULL)  // exit when the stack is empty
  {
    // Search for cur in the hash table, proceed if it has not yet been indexed
    if (!(int) item_indices->lookup(cur))
    {
/*XXX*/ cout << "cur = " << (int) cur << endl; cout.flush();
      // Get the object's data type
      cur_type = SNPL_find_type(cur);
/*XXX*/ cout << "cur_type = " << cur_type->name << endl; cout.flush();
    
      // Search for cur's data type in the hash table.
      cur_type_index = (int) type_indices->lookup((void *) cur_type) - 1;

      // If this data type is not in the hash table...  
      if (cur_type_index < 0)
      {
        // Add the type to the list of data types.
        data_types->enqueue((void *) cur_type);
        
	// Create a new object array.
        item_array = new SNPL_array(16,0);

	// Now add this to the collection of all object arrays.
        items_by_type->enqueue((void *) item_array);

	// Finally, add a reference to the index table.
	type_indices->add((void *) cur_type, (void *) (++total_types));
      }
      else
      {
        // If this type is in the table, just retrieve the appropriate object array.    
        item_array = (SNPL_array *) items_by_type->get(cur_type_index);
      }
      
      // Add the object to the array just found or created.
      item_array->enqueue((void *) cur);

      // Now add a reference to the object itself in the hash table.
      item_indices->add(cur, (void *) item_array->size());
      
      // If this data type has a preserializer, allow it to add the item's
      // own references to the stack.
      if (cur_type->preserializer != NULL)
	cur_type->preserializer(cur, data_stack);

      /*
        Add the item's immediate associative "neighborhood".
      */
      edge_arrays = (SNPL_array **) edges_by_item->lookup(cur);
      // If {cur} has outbound edges, inspect them.
      if (edge_arrays != NULL) {
        // For key0 and key1 positions
        for (k=0; k<2; k++) {
          er = edge_arrays[k];
          maxj = er->size();
/*XXX*/ cout << "\tmaxj = " << maxj << endl;
          j = 0;
          while (j < maxj) {
            key = er->get(j);
	    target = er->get(j+1);

	    // We've found a new "active" edge.
	    if (item_indices->lookup(key) != NULL) {
	      // If the target is not yet in the graph, add it.
	      if (item_indices->lookup(target) == NULL) {
                #ifdef BREADTH_FIRST_SERIALIZATION
                  data_stack->enqueue(target);
                #else
                  data_stack->push(target);
                #endif 
	        //maxi++;
	        //is_in_graph->add(target, (void *) 1);
	      }
              // Record this edge in reduced_TAA
	      if (!k)
	        reduced_TAA->add(cur, key, target);
	      else
	        reduced_TAA->add(key, cur, target); 
	      // Remove this edge from the array
	      er->simple_remove(j+1);
	      er->simple_remove(j);
	      // Edge array is now two cells smaller.
	      maxj -= 2;
	      j -= 2;
	    }  // end if
	    j += 2;
          }  // end while
        }  // end for
      }  // end if
    
    }
  }
  
  delete data_stack;
  delete_edges_by_item(edges_by_item);

  // Re-number the active data types so that the internal data types come
  // first in the file, then the external types (for better readability).
  // Among the external types, the preserializing types must come last, in
  // ascending order of dependency.
  int type_index=0, new_index, precedence, temp;
  int *permuted_indices = new int[data_types->size()];
  
  for (int i=0; i<data_types->size(); i++)
  {
    cur_type = (SNPL_data_type *) data_types->get(i);

    new_index = type_index++;
    permuted_indices[new_index] = i;
    precedence = SNPL_find_type_precedence(cur_type);

    // Filter down according to precedence
    while ((new_index > 0) //~ unnecessary
           &&(precedence < SNPL_find_type_precedence((SNPL_data_type *) data_types->get(permuted_indices[new_index-1]))))
    {
      permuted_indices[new_index]
        = permuted_indices[new_index-1];
      permuted_indices[new_index-1] = i;
      new_index--;
    }
  }
  
  // Re-hash the active data types using these new indices.
  for (int i=0; i<data_types->size(); i++)
    type_indices->add(data_types->get(permuted_indices[i]), (void *) (i+1));
      
  // Now write to the output file.  Begin with a header indicating where the
  // deserializer-parser should look for the root object.
  out << "root: "
      << (int) type_indices->lookup((void *) SNPL_find_type(root)) - 1
      << '.'
      << (int) item_indices->lookup((void *) root) - 1
      << "\n\n";
  
  // Record all data items to the output file, grouped by type in the order
  // described by permuted_indices[].
  for (int i=0; i<data_types->size(); i++)
  {
    cur_type = (SNPL_data_type *) data_types->get(permuted_indices[i]);
    item_array = (SNPL_array *) items_by_type->get(permuted_indices[i]);
       
    out << i << ")\t" << cur_type->name;
    out << ' ' << item_array->size() << " {\n";
      
    // Serialization of individual data items.
    for (int j=0; j<item_array->size(); j++)
    {
      cur = item_array->get(j);
      out << "\t" << j << ")\t";
	
      cur_type->serializer(cur, item_indices, type_indices, out);
	
      out << "\n";
    }   
    out << "}\n\n";
  }

  delete permuted_indices;
  delete data_types;

  // Create an array of all items in the graph, so as to pick out only those
  // associations which apply.
  //int total_items = 0;
  //for (int i=0; i<items_by_type->size(); i++)
  //  total_items += ((SNPL_array *) items_by_type->get(i))->size();
  //void **all_items = new (void *)[total_items], **pos = all_items;
/*  
  SNPL_array *all_items = new SNPL_array(100,2);
  while (items_by_type->size()) {
    SNPL_array *a = (SNPL_array *) items_by_type->pop();
    while (a->size())
      all_items->enqueue(a->pop());
      //*(pos++) = a->pop();
    delete a;
  }*/
  delete items_by_type;

/*XXX*/ cout << "Serializing reduced TAA." << endl; cout.flush();
  SNPL_data_type *taa_type = SNPL_reference_type("ternary_associative_array");
  out << "A) ";
  taa_type->serializer((void *) reduced_TAA, item_indices, type_indices, out);
  out << "\n";
  delete reduced_TAA;
  
  //delete all_items;
  delete item_indices, type_indices;
}



#endif

/*- end of file --------------------------------------------------------------*/

