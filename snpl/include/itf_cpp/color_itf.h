#ifndef COLOR_ITF_H
#define COLOR_ITF_H

#include "../type/color.h"
#include "../snpl.h"  // For debugging macro.
using namespace std;



void SNPL_color_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_color_deserializing_constructor(ifstream &);
void SNPL_color_destructor(void *);



void SNPL_color_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef COLOR_DEBUG
    cout << "<> void SNPL_color_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif

  char *s = ((SNPL_color *) p)->encode();
  out << s;
  delete s;
}



void *SNPL_color_deserializing_constructor(ifstream &in)
{
  #ifdef COLOR_DEBUG
    cout << "<> void *SNPL_color_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[7];
  in >> s;
  SNPL_color *new_color = new SNPL_color(s);
  delete s;
  return (void *) new_color;
}



void SNPL_color_destructor(void *p)
{
  #ifdef COLOR_DEBUG
    cout << "<> void SNPL_color_destructor(void *)" << endl;
    cout.flush();
  #endif

  delete (SNPL_color *) p;
}

#endif
