// Reference-hole-UNFRIENDLY.  Serialize with caution.

#ifndef TERNARY_ASSOCIATIVE_ARRAY_ITF_H
#define TERNARY_ASSOCIATIVE_ARRAY_ITF_H

#include "../snpl.h"  // ^ BREADTH_FIRST_SERIALIZATION
#include "../util/ternary_associative_array.h"

void SNPL_ternary_associative_array_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_ternary_associative_array_deserializing_constructor(ifstream &);
void SNPL_ternary_associative_array_destructor(void *);
void SNPL_ternary_associative_array_preserializer(void *, SNPL_array *);



/*
  Warning: this is going to result in some huge stacks.
  You'll want to find a better way, e.g. by only adding those items which are
  not already in the serialization hash table (write a new global function
  for this).
  In this particular case, you'll want to design a graph traversal routine
  *specifically* for the elements of ternary...arrays.
*/
void SNPL_ternary_associative_array_preserializer(
  void *p,
  SNPL_array *data_stack)
{
  #ifdef TERNARY_ASSOCIATIVE_ARRAY_DEBUG
    cout << "<> void SNPL_ternary_associative_array_preserializer(void *, SNPL_array *)" << endl;
    cout.flush();
  #endif
  
  SNPL_ternary_associative_array *baa
    = (SNPL_ternary_associative_array *) p;
  void **items = baa->condense();
  void **cur = items, **max = cur + (baa->size()*3);
  
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



void SNPL_ternary_associative_array_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef ternary_ASSOCIATIVE_ARRAY_DEBUG
    cout << "<> void SNPL_ternary_associative_array_serializer(void *, SNPL_associative_array *, ofstream &)" << endl;
    cout.flush();
  #endif

  SNPL_ternary_associative_array *baa
    = (SNPL_ternary_associative_array *) p;
  void **cur = baa->get_entries();
  int size = baa->size();
  out << baa->sparsity() << " "
      << baa->expansion() << " "
      << size << " {\n";
  for (int i=0; i<size; i++) {
    while (*cur == NULL)
      cur += 3;
    out << "\t";
    for (int j=0; j<3; j++) {
      out << "\t"
          << (int) type_indices->lookup((void *) SNPL_find_type(*cur)) - 1
	  << "."
	  << (int) item_indices->lookup(*cur) - 1;
      cur++;
    }
    out << '\n';
  }
  out << "\t}";
} 



void *SNPL_ternary_associative_array_deserializing_constructor(ifstream &in)
{
  #ifdef ternary_ASSOCIATIVE_ARRAY_DEBUG
    cout << "<> void *SNPL_ternary_associative_array_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[MAX_STRLEN];
  double sparsity, expansion;
  int size;
  in >> sparsity >> expansion >> size >> s;
  
  // We start the array off as compactly as possible, in case
  // it remains static (no additions before it is re-serialized).
  SNPL_ternary_associative_array *baa
    = new SNPL_ternary_associative_array((int) (size*sparsity), sparsity, expansion);

  void *key_a, *key_b, *key_c;
  for (int i=0; i<size; i++) {
    in >> s;
    key_a = SNPL_decode_item_reference(s);
//XXX cout << "Deserializing triplet:\n\t" << s << ": " << (char *) key_a << endl;
    in >> s;
    key_b = SNPL_decode_item_reference(s);
//XXX cout << "\t" << s << ": " << (char *) key_b << endl;    
    in >> s;
    key_c = SNPL_decode_item_reference(s);
//XXX cout << "\t" << s << ": " << (char *) key_c << endl;    
    baa->add(key_a, key_b, key_c); 
  }
  in >> s;  // (ignore closing bracket)
  delete s;

  //! Remember to register any new item...
  return (void *) baa;
}



void SNPL_ternary_associative_array_destructor(void *p)
{
  #ifdef TERNARY_ASSOCIATIVE_ARRAY_DEBUG
    cout << "<> void SNPL_ternary_associative_array_destructor(void *)" << endl;
    cout.flush();
  #endif
  
  delete (SNPL_ternary_associative_array *) p;
}



#endif
