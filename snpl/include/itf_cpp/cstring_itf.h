#ifndef CSTRING_ITF_H
#define CSTRING_ITF_H

#include "../snpl.h"  // For debugging macro.
#include <iostream>
using namespace std;



void SNPL_cstring_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_cstring_deserializing_constructor(ifstream &);
void SNPL_cstring_destructor(void *);



void SNPL_cstring_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef CSTRING_DEBUG
    cout << "<> void SNPL_cstring_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif
  
  SNPL_write_cstring(out, (char *) p);
}



void *SNPL_cstring_deserializing_constructor(ifstream &in)
{
  #ifdef CSTRING_DEBUG
    cout << "<> void *SNPL_cstring_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif
  
  char *s0 = new char[MAX_STRLEN];
  in >> s0;
  char *s1 = SNPL_read_cstring(s0);
  char *s2 = SNPL_copy_cstring(s1);
  delete s0;
  delete s1;
  return (void *) s2;
}



void SNPL_cstring_destructor(void *p)
{
  #ifdef CSTRING_DEBUG
    cout << "<> void SNPL_cstring_destructor(void *)" << endl;
    cout.flush();
  #endif
  
  delete (char *) p;
}

#endif
