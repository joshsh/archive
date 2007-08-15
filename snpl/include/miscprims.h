/*+

  miscprims.h

  Miscellaneous primitives for importing into SNPL.  Most of these are only
  necessary because the importing utility is not set up to handle functions which
  pass by value.

  author: Josh Shinavier
  last edited: 1/13/04

*/

#ifndef MISCPRIMS_H
#define MISCPRIMS_H



#include <iostream>



/** pointers and addresses ****************************************************/

int *C_address_operator(void *p) {
  int *i = new int[1];
  *i = (int) p;
  return i;
}

char *C_indirection_operator(int *i) {
  char *ch = new char[1];
  *ch = *((char *) *i);
  return ch;
}



/** Arithmetic types **********************************************************/

/*
int *int_negate(int *arg)
{
  int *i = new int[1];
  *i = -(*arg);
  return i;
}

double *double_negate(double *arg)
{
  double *i = new double[1];
  *i = -(*arg);
  return i;
}*/

char *int_to_char(int *ip)
{
  char *p = (char *) malloc(sizeof(char));
  *p = (char) *ip;
  return p;
}

bool *int_greaterthan(int *i0, int *i1)
{
  bool *b = new bool[1];
  *b = (*i0 > *i1);
  return b;
}
bool *int_lessthan(int *i0, int *i1)
{
  bool *b = new bool[1];
  *b = (*i0 < *i1);
  return b;
}
bool *int_equals(int *i0, int *i1)
{
  bool *b = new bool[1];
  *b = (*i0 == *i1);
  return b;
}



/** math functions ************************************************************/

#include <math.h>

double *math_cos(double *arg) {
  double *d = new double[1];
  *d = cos(*arg);
  return d;
}

double *math_sin(double *arg) {
  double *d = new double[1];
  *d = sin(*arg);
  return d;
}

double *math_tan(double *arg) {
  double *d = new double[1];
  *d = tan(*arg);
  return d;
}

double *math_acos(double *arg) {
  double *d = new double[1];
  *d = acos(*arg);
  return d;
}

double *math_asin(double *arg) {
  double *d = new double[1];
  *d = asin(*arg);
  return d;
}

double *math_atan(double *arg) {
  double *d = new double[1];
  *d = atan(*arg);
  return d;
}

double *math_log(double *arg) {
  double *d = new double[1];
  *d = log(*arg);
  return d;
}

double *math_exp(double *arg) {
  double *d = new double[1];
  *d = exp(*arg);
  return d;
}

double *math_sqrt(double *arg) {
  double *d = new double[1];
  *d = sqrt(*arg);
  return d;
}


/** C strings *****************************************************************/

int *itf_strlen(char *s)
{
  int *i = new int[1];
  *i = strlen(s);
  return i;
}
void charcat(char *s, char *ch)
{
  int len = strlen(s);
  s[len++] = *ch;
  s[len] = '\0';
}
void printstr(char *s)
{
  cout << s;
}
void printchar(char *ch)
{
  cout << *ch;
}
void println(char *s)
{
  cout << s << endl;
}
char *new_cstring(int *i)
{
  char *s = new char[*i];
  *s = '\0';
  return s;
}
void capitalize(char *s)
{
  if ((*s >= 97)&&(*s <= 122))
    *s -= 32;
}



#endif
