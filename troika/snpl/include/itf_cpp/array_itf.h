// Reference-hole-friendly

#ifndef ARRAY_ITF_H
#define ARRAY_ITF_H

#include "../util/array.h"
#include "../snpl.h"  // ^ BREADTH_FIRST_SERIALIZATION
using namespace std;

// Note: I don't yet have any primitives for SNPL_arrays.



void SNPL_array_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_array_deserializing_constructor(ifstream &);
void SNPL_array_destructor(void *);
void SNPL_array_preserializer(void *, SNPL_array *);



void SNPL_array_preserializer(
  void *p,
  SNPL_array *data_stack)
{
  #ifdef ASSOCIATIVE_DEBUG
    cout << "<> void SNPL_array_preserializer(void *, SNPL_array *)" << endl;
    cout.flush();
  #endif
  
  SNPL_array *a = (SNPL_array *) p;
  for (int i=0; i<a->size(); i++) {
    void *q = a->get(i);
    #ifdef BREADTH_FIRST_SERIALIZATION
      data_stack->enqueue(q);
    #else
      data_stack->push(q);
    #endif 
  }
}



void SNPL_array_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef ARRAY_DEBUG
    cout << "<> void SNPL_array_serializer(void *, SNPL_associative_array *, ofstream &)" << endl;
    cout.flush();
  #endif

  SNPL_array *a = (SNPL_array *) p;
  int size = a->size();
  out << a->expansion() << size << " {";

  for (int i=0; i<size; i++) {
    void *q = a->get(i);
    out << " "
        << (int) type_indices->lookup(
	   (void *) SNPL_find_type(q)) - 1
	<< "."
	<< (int) item_indices->lookup(q) - 1;
  }
  out << " }";
}



void *SNPL_array_deserializing_constructor(ifstream &in)
{
  #ifdef ARRAY_DEBUG
    cout << "<> void *SNPL_array_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[10];    
  int size, expansion;
  in >> expansion >> size >> s;  
  SNPL_array *array = new SNPL_array(size, expansion);
  void *p;
  for (int i=0; i<size; i++) {
    in >> s;
    p = SNPL_decode_item_reference(s);
    if (p == NULL)
      SNPL_add_reference_hole(s, array->get_array()+array->size());
    array->enqueue(p);
  }
  in >> s;  // (ignore closing bracket)
  delete s;
  
  return (void *) array;
}



void SNPL_array_destructor(void *p)
{
  /*
    Don't worry about the array's references; they are assumed to be
    first-class values in the data set, and consequently will be deleted
    individually.
  */
  delete (SNPL_array *) p;
}



#endif
