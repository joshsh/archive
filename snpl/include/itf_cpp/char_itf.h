#ifndef CHAR_ITF_H
#define CHAR_ITF_H

#include "../snpl.h"  // For debugging macro.
using namespace std;



//!
#include <iostream>
void SNPL_char_print(char *s)
{
  cout << *s;
  cout.flush(); //~ wasty...
}

void SNPL_char_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_char_deserializing_constructor(ifstream &);
void SNPL_char_destructor(void *);



void SNPL_char_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef CHAR_DEBUG
    cout << "<> void SNPL_char_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif

  out << (int) *((char *) p);
}



void *SNPL_char_deserializing_constructor(ifstream &in)
{
  #ifdef CHAR_DEBUG
    cout << "<> void *SNPL_char_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[1];
  int val;
  in >> val;  // Read as int (ascii value in decimal), then convert to a char.
  *s = (char) val;
  return (void *) s;
}



void SNPL_char_destructor(void *p)
{
  #ifdef CHAR_DEBUG
    cout << "<> void SNPL_char_destructor(void *)" << endl;
    cout.flush();
  #endif

  delete (char *) p;
}

#endif
