#ifndef DATA_TYPE_ITF_H
#define DATA_TYPE_ITF_H

#include "../snpl.h"  // For debugging macro.
using namespace std;



void SNPL_data_type_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_data_type_deserializing_constructor(ifstream &);
void SNPL_data_type_destructor(void *);



void SNPL_data_type_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef DATA_TYPE_DEBUG
    cout << "<> void SNPL_data_type_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif
  
  // Will atoi() ever have issues with the default representation of int's?
  out << ((SNPL_data_type *) p)->name;
}



void *SNPL_data_type_deserializing_constructor(ifstream &in)
{
  #ifdef DATA_TYPE_DEBUG
    cout << "<> void *SNPL_data_type_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif
  
  char *s = new char[MAX_STRLEN];
  in >> s;
  char *s1 = SNPL_read_cstring(s);
  SNPL_data_type *dt = SNPL_reference_type(s1);
  delete s1;
  delete s;
  return (void *) dt;
}



void SNPL_data_type_destructor(void *p)
{
  #ifdef DATA_TYPE_DEBUG
    cout << "<> void SNPL_data_type_destructor(void *)" << endl;
    cout.flush();
  #endif
  
  SNPL_data_type *dt = (SNPL_data_type *) p;
  delete dt->name;
  free(dt);
  //SNPL_data_type_destructor((SNPL_data_type *) p);
}

#endif

