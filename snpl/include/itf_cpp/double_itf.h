#ifndef DOUBLE_ITF_H
#define DOUBLE_ITF_H

#include "../snpl.h"  // For debugging macro.
using namespace std;



// Note: for right now, the names of functions wrapped as primitives need to be
// unique, i.e. two functions with the same name but different argument types
// cannot be distinguished, and importing them both will give you a C++ compile
// error.

double *double_add(double *d1, double *d2)
{
  double *d3 = (double *) malloc(sizeof(double));
  *d3 = *d1 + *d2;
  return d3;
}
double *double_subtract(double *d1, double *d2)
{
  double *d3 = (double *) malloc(sizeof(double));
  *d3 = *d1 - *d2;
  return d3;
}
double *double_multiply(double *d1, double *d2)
{
  double *d3 = (double *) malloc(sizeof(double));
  *d3 = *d1 * *d2;
  return d3;
}
double *double_divide(double *d1, double *d2)
{
  double *d3 = (double *) malloc(sizeof(double));
  *d3 = *d1 / *d2;
  return d3;
}



void SNPL_double_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_double_deserializing_constructor(ifstream &);
void SNPL_double_destructor(void *);



void SNPL_double_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef DOUBLE_DEBUG
    cout << "<> void SNPL_double_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif
  
  //? formatting issues?
  out << *((double *) p);
}



void *SNPL_double_deserializing_constructor(ifstream &in)
{
  #ifdef DOUBLE_DEBUG
    cout << "<> void *SNPL_double_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif
  
  double *p = new double[1];
  in >> *p;
  return (void *) p;
}



void SNPL_double_destructor(void *p)
{
  #ifdef DOUBLE_DEBUG
    cout << "<> void SNPL_double_destructor(void *)" << endl;
    cout.flush();
  #endif
  
  delete (double *) p;
}

#endif
