// Reference-hole-UNFRIENDLY:
// Make sure key_maps are serialized before chars.

#ifndef KEY_MAP_ITF_H
#define KEY_MAP_ITF_H

#include "../snpl.h"  // ^ BREADTH_FIRST_SERIALIZATION
#include "../type/key_map.h"
using namespace std;



void SNPL_key_map_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_key_map_deserializing_constructor(ifstream &);
void SNPL_key_map_destructor(void *);
void SNPL_key_map_preserializer(void *, SNPL_array *);



void SNPL_key_map_preserializer(
  void *p,
  SNPL_array *data_stack)
{
  #ifdef KEY_MAP_DEBUG
    cout << "<> void SNPL_key_map_preserializer(void *, SNPL_array *)" << endl;
    cout.flush();
  #endif

  SNPL_key_map *keymap = (SNPL_key_map *) p;
  for (int i=0; i<keymap->size; i++) {
//cout << "i = " << i << " (" << (int) *(keymap->keys[i]) << ")" << endl;
    #ifdef BREADTH_FIRST_SERIALIZATION
      data_stack->enqueue((void *) keymap->keys[i]);
    #else
      data_stack->push((void *) keymap->keys[i]);
    #endif 
  }
}



void SNPL_key_map_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef KEY_MAP_DEBUG
    cout << "<> void SNPL_key_map_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif

  SNPL_key_map *keymap = (SNPL_key_map *) p;
  out << keymap->size << " {";
  for (int i=0; i<keymap->size; i++)
    out << " " << (int) item_indices->lookup((void *) keymap->keys[i]) - 1;
  out << " }";
}



void *SNPL_key_map_deserializing_constructor(ifstream &in)
{
  #ifdef KEY_MAP_DEBUG
    cout << "<> void *SNPL_key_map_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  int size, char_id;
  char ch;
  in >> size >> ch;
  SNPL_key_map *keymap = SNPL_construct_key_map(size);
  SNPL_data_type *char_type = SNPL_reference_type("char");

  for (int i=0; i<size; i++) {
    in >> char_id;
    SNPL_define_key(
      keymap,
      (char *) SNPL_decode_item_reference(char_type, char_id),
      i);
  }

  in >> ch;  // (ignore closing bracket)
  return (void *) keymap;
}



void SNPL_key_map_destructor(void *p)
{
  #ifdef KEY_MAP_DEBUG
    cout << "<> void SNPL_key_map_destructor(void *)" << endl;
    cout.flush();
  #endif

  SNPL_key_map_destructor((SNPL_key_map *) p);
}

#endif
