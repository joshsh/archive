// Reference-hole-UNFRIENDLY.  Serialize with caution.

#ifndef ASSOCIATIVE_ARRAY_ITF_H
#define ASSOCIATIVE_ARRAY_ITF_H

#include "../util/associative_array.h"
#include "../snpl.h"  // ^ BREADTH_FIRST_SERIALIZATION
using namespace std;

/* Note: there's no elegant way to fill scope holes for associative arrays, so
beware of giving them key or target values of any other preserializing type */


void SNPL_associative_array_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_associative_array_deserializing_constructor(ifstream &);
void SNPL_associative_array_destructor(void *);
void SNPL_associative_array_preserializer(void *, SNPL_array *);



void SNPL_associative_array_preserializer(
  void *p,
  SNPL_array *data_stack)
{
  #ifdef ASSOCIATIVE_ARRAY_DEBUG
    cout << "<> void SNPL_associative_array_preserializer(void *, SNPL_array *)" << endl;
    cout.flush();
  #endif
  
  SNPL_associative_array *aa = (SNPL_associative_array *) p;
  void **items = aa->condense();
  void **cur = items, **max = cur + (aa->size()*2);
  
  while (cur < max) {
    #ifdef BREADTH_FIRST_SERIALIZATION
      data_stack->enqueue(*cur);
    #else
      data_stack->push(*cur);
    #endif      
    cur++;
  }
  
  delete items;
}



void SNPL_associative_array_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef ASSOCIATIVE_ARRAY_DEBUG
    cout << "<> void SNPL_associative_array_serializer(void *, SNPL_associative_array *, ofstream &)" << endl;
    cout.flush();
  #endif

  SNPL_associative_array *aa = (SNPL_associative_array *) p;
  void **items = aa->condense();
  void **cur = items;
  
  int size = aa->size();
  out << aa->sparsity() << " "
      << aa->expansion() << " "
      << size << " {\n";
  for (int i=0; i<size; i++) {
    out << "\t";
    for (int j=0; j<2; j++) {
      out << "\t"
          << (int) type_indices->lookup((void *) SNPL_find_type(*cur)) - 1
	  << "."
	  << (int) item_indices->lookup(*cur) - 1;
      cur++;
    }
    out << '\n';
  }
  out << "\t}";
  
  delete items;
} 



void *SNPL_associative_array_deserializing_constructor(ifstream &in)
{
  #ifdef ASSOCIATIVE_ARRAY_DEBUG
    cout << "<> void *SNPL_associative_array_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[10];
  double sparsity, expansion;
  int size;
  in >> sparsity >> expansion >> size >> s;
  
  // We start the array off as compactly as possible, in case
  // it remains static (no additions before it is re-serialized).
  SNPL_associative_array *array
    = new SNPL_associative_array((int) (size*sparsity), sparsity, expansion);

  void *key_a, *key_b;
  for (int i=0; i<size; i++) {
    in >> s;
    key_a = SNPL_decode_item_reference(s);
    in >> s;
    key_b = SNPL_decode_item_reference(s);
    array->add(key_a, key_b); 
  }
  in >> s;  // (ignore closing bracket)
  delete s;

  //! Remember to register any new item...
  return (void *) array;
}



void SNPL_associative_array_destructor(void *p)
{
  delete (SNPL_associative_array *) p;
}



#endif
