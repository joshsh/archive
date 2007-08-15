#ifndef INT_ITF_H
#define INT_ITF_H

#include "../snpl.h"  // For debugging macro.
using namespace std;



int *int_add(int *i1, int *i2)
{
  int *i3 = (int *) malloc(sizeof(int));
  *i3 = *i1 + *i2;
  return i3;
}
int *int_subtract(int *i1, int *i2)
{
  int *i3 = (int *) malloc(sizeof(int));
  *i3 = *i1 - *i2;
  return i3;
}
int *int_multiply(int *i1, int *i2)
{
  int *i3 = (int *) malloc(sizeof(int));
  *i3 = *i1 * *i2;
  return i3;
}
int *int_divide(int *i1, int *i2)
{
  int *i3 = (int *) malloc(sizeof(int));
  *i3 = *i1 / *i2;
  return i3;
}
int *int_mod(int *i1, int *i2)
{
  int *i3 = (int *) malloc(sizeof(int));
  *i3 = *i1 % *i2;
  return i3;
}



void SNPL_int_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_int_deserializing_constructor(ifstream &);
void SNPL_int_destructor(void *);



void SNPL_int_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef INT_DEBUG
    cout << "<> void SNPL_int_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif
    
  // Will atoi() ever have issues with the default representation of int's?
  out << *((int *) p);
}



void *SNPL_int_deserializing_constructor(ifstream &in)
{
  #ifdef INT_DEBUG
    cout << "<> void *SNPL_int_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif
  
  int *p = new int[1];
  in >> *p;
  return (void *) p;
}



void SNPL_int_destructor(void *p)
{
  #ifdef INT_DEBUG
    cout << "<> void SNPL_int_destructor(void *)" << endl;
    cout.flush();
  #endif
  
  delete (int *) p;
}

#endif

