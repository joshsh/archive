// Reference-hole-UNFRIENDLY:
//  Make sure SDL_Surfaces and bitmaps are serialized before GUIs.

#ifndef GUI_ITF_H
#define GUI_ITF_H

#include "../type/gui.h"
#include "../snpl.h"  // ^ BREADTH_FIRST_SERIALIZATION
using namespace std;



void SNPL_GUI_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_GUI_deserializing_constructor(ifstream &);
void SNPL_GUI_destructor(void *);
void SNPL_GUI_preserializer(void *, SNPL_array *);



void SNPL_GUI_preserializer(
  void *p,
  SNPL_array *data_stack)
{
  #ifdef GUI_DEBUG
    cout << "<> void SNPL_GUI_preserializer(void *, SNPL_array *)" << endl;
    cout.flush();
  #endif
  
  SNPL_GUI *gui = (SNPL_GUI *) p;
  
  // Note: we need to get at the actual bitmap and screen members here; if we
  // just make a copy, the serializer will not find that copy.
  void *screen = (void *) gui->get_screen();
  void *bitmap = (void *) gui->get_bitmap();
  #ifdef BREADTH_FIRST_SERIALIZATION
    data_stack->enqueue(screen);
    data_stack->enqueue(bitmap);
  #else
    data_stack->push(screen);
    data_stack->push(bitmap);
  #endif 
}



void SNPL_GUI_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef GUI_DEBUG
    cout << "<> void SNPL_GUI_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif

  SNPL_GUI *gui = (SNPL_GUI *) p;
  
  void *screen = (void *) gui->get_screen();
  void *bitmap = (void *) gui->get_bitmap();
  int scale = gui->get_scale();
    
  out << (int) type_indices->lookup((void *) SNPL_find_type(screen)) - 1
      << "."
      << ((int) item_indices->lookup(screen) - 1)
      << "\t"
      << (int) type_indices->lookup((void *) SNPL_find_type(bitmap)) - 1
      << "."
      << ((int) item_indices->lookup(bitmap) - 1) 
      << "\t" << gui->get_scale();
}



void *SNPL_GUI_deserializing_constructor(ifstream &in)
{
  #ifdef GUI_DEBUG
    cout << "<> void *SNPL_GUI_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  int scale;
  char *s = new char[10];
  in >> s;
  SDL_Surface *screen = (SDL_Surface *) SNPL_decode_item_reference(s);
  in >> s >> scale;
  SNPL_bitmap *bitmap = (SNPL_bitmap *) SNPL_decode_item_reference(s);
  delete s;
  return (void *) new SNPL_GUI(screen, bitmap, scale);
}



void SNPL_GUI_destructor(void *p)
{
  #ifdef GUI_DEBUG
    cout << "<> void SNPL_GUI_destructor(void *)" << endl;
    cout.flush();
  #endif

  delete (SNPL_GUI *) p;
}



#endif
