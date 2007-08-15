#ifndef CPP_PRIMITIVE_ITF_H
#define CPP_PRIMITIVE_ITF_H

#include "../snpl.h"  // For debugging macro.
using namespace std;



void SNPL_cpp_primitive_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_cpp_primitive_deserializing_constructor(ifstream &);
void SNPL_cpp_primitive_destructor(void *);



void SNPL_cpp_primitive_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef CPP_PRIMITIVE_DEBUG
    cout << "<> void SNPL_cpp_primitive_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif

  SNPL_cpp_primitive *prim = (SNPL_cpp_primitive *) p;

  // For now, primitives will simply be identified by their name, regardless of
  // parameter types.  So, no overloading of names just yet.
  SNPL_write_cstring(out, prim->name);
}



void *SNPL_cpp_primitive_deserializing_constructor(ifstream &in)
{
  #ifdef CPP_PRIMITIVE_DEBUG
    cout << "<> void *SNPL_cpp_primitive_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s0 = new char[MAX_STRLEN];
  in >> s0;
  char *s1 = SNPL_read_cstring(s0);
  SNPL_cpp_primitive *prim = SNPL_reference_primitive(s1);
  delete s0;
  delete s1;
  return (void *) prim;
}



void SNPL_cpp_primitive_destructor(void *p)
{
  #ifdef CPP_PRIMITIVE_DEBUG
    cout << "<> void SNPL_cpp_primitive_destructor(void *)" << endl;
    cout.flush();
  #endif
  
  SNPL_cpp_primitive_destructor((SNPL_cpp_primitive *) p);
}

#endif
