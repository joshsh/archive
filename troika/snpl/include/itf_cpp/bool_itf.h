#ifndef BOOL_ITF_H
#define BOOL_ITF_H

#include "../snpl.h"  // For debugging macro.
using namespace std;



void SNPL_bool_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_bool_deserializing_constructor(ifstream &);
void SNPL_bool_destructor(void *);



void SNPL_bool_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef BOOL_DEBUG
    cout << "<> void SNPL_bool_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif

  out << (int) *((bool *) p);
}



void *SNPL_bool_deserializing_constructor(ifstream &in)
{
  #ifdef BOOL_DEBUG
    cout << "<> void *SNPL_bool_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  bool *b = new bool[1];
  int val;
  in >> val;  // Read as int (0 or 1), then convert to a bool.
  *b = (val == 1);
  return (void *) b;
}



void SNPL_bool_destructor(void *p)
{
  #ifdef BOOL_DEBUG
    cout << "<> void SNPL_bool_destructor(void *)" << endl;
    cout.flush();
  #endif

  delete (bool *) p;
}



#endif
