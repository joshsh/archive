/*+
 * expanding_string.cpp
 *
 * author: Josh Shinavier
 * last edited: 11/18/04
 */

#include "expanding_string.h"
#include <cstring>
using namespace std; 

//!
//#include <iostream>

#ifdef DEFAULT_EXPANSION_FACTOR
  #undef DEFAULT_EXPANSION_FACTOR
#endif

// By default, expand() doubles the size of the string.
// Note: the expansion factor does not need to be an integer.
#define DEFAULT_EXPANSION_FACTOR 2


/*
#include "strutil.h"
#include <iostream>
void show_bytes(void *p, int n)
{
  view_bytes(p, n);
}
//! temporary debugging method
void SNPL_expanding_string::view_bytes()
{
  cout << "expanding_string: ";
  show_bytes((void *) expanding_string, filled);
}
//*/



int SNPL_expanding_string::actual_size()
{
  return string_size;
}
char *SNPL_expanding_string::actual_string()
{
  return expanding_string;
}


SNPL_expanding_string::SNPL_expanding_string(int size0, double expansion0)
            : expansion_factor(expansion0),
              string_size(size0),
	      filled(0)
{
  if (expansion_factor <= 1)
    expansion_factor = 2;
  if (string_size <= 0)
    string_size = 1;
  while (string_size*expansion_factor == string_size)
    string_size++;
  expanding_string = new char[string_size];
}



SNPL_expanding_string::~SNPL_expanding_string()
{
  delete expanding_string;
}



int SNPL_expanding_string::size()
{
  return filled;
}



double SNPL_expanding_string::expansion()
{
  return expansion_factor;
}
  


void SNPL_expanding_string::clear()
{
  filled = 0;
}



char *SNPL_expanding_string::copy()
{
  char *s = new char[filled+1];
  s[filled] = '\0';
  for (int i=0; i<filled; i++)
    s[i] = expanding_string[i];
  return s;
}



/** Array resizing *//*========================================================*/



/*
 * (private) void SNPL_expanding_string::expand()
 *
 */
void SNPL_expanding_string::expand()
{
  // If expansion_factor is too small, the table might not actually gain in
  // integer size when it expand()'s, leading to a segmentation fault.
  // This is here (rather than in the constructor) in case a call to minimize()
  // precedes a call to expand(), which for certain parameters could cause
  // failure.
  if ((expansion_factor <= 1)
    ||(string_size == (int) (string_size*expansion_factor)))
    expansion_factor = DEFAULT_EXPANSION_FACTOR;
    
  int size0 = (int) (expansion_factor*string_size);
  char *string0 = new char[size0];
  for (int i=0; i<filled; i++)
    string0[i] = expanding_string[i]; 
  delete expanding_string;
  expanding_string = string0;
  string_size = size0;
}



void SNPL_expanding_string::minimize()
{
  if (filled != string_size) {
    char *string0;
    if (!filled) {
      string0 = new char[1];
      string_size = 1;
    } else {
      string0 = new char[filled];
      for (int i=0; i<filled; i++)
        string0[i] = expanding_string[i];
    }
    delete expanding_string;
    expanding_string = string0;
    string_size = filled;
  }
}



/** Random access *//*=========================================================*/



/*
 * Note: for better speed, assumes the argument to be in the approprate range.
 */
char SNPL_expanding_string::get(int index)
{
  return expanding_string[index];
}



/*
 * Note: for better speed, assumes the argument to be in the approprate range.
 */
void SNPL_expanding_string::set(int index, char ch)
{
  expanding_string[index] = ch;
}



/** Adding to the string *//*==================================================*/



void SNPL_expanding_string::append(char ch)
{
//cout << "\t\t\tappend(char)" << endl;
//cout << "\t\t\tfilled = " << filled << endl;
//cout << "\t\t\tactual_size() = " << actual_size() << endl;  
  if (filled == string_size)
    expand();
//cout << "\t\t\tactual_size() = " << actual_size() << endl;
  char *es = actual_string();
  es[filled++] = ch;
}



void SNPL_expanding_string::append(char *s)
{
  int len = strlen(s);
  if (len) {
    len += filled;
    while (actual_size() < len)
      expand();
    char *s2 = actual_string() + filled;
    while ((*s2 = *s) != '\0') {
      s++;
      s2++;
    }
    filled = len;
  }
}



/*
  Append the value of a void pointer as if it were a 4-character string
  (The SNPL executor uses this)
*/
void SNPL_expanding_string::append(void *p)
{
  // It's a little inefficient to have this here...
  int len = sizeof(void *);
//cout << "len = " << len << endl; cout.flush();
  // I used size() here because the loop will not update the value of
  // string_size as the array expands.
//cout << "\t\t\tappend(void *p)" << endl;
//cout << "\t\t\tfilled+len = " << filled+len << endl;
//cout << "\t\t\tactual_size() = " << actual_size() << endl;
  while (filled+len > actual_size())
    expand();
//cout << "\t\t\tactual_size() = " << actual_size() << endl;
//cout << "len2 = " << len << endl; cout.flush();
  char *s = (char *) &p;
  char *es = actual_string();
  for (int i=0; i<len; i++)
  {
    es[filled++] = *s;
    s++;
  }
  //filled += len;
}



/*
  Note: don't be tempted to use strlen() here, as SNPL_expanding_strings may
  contain '\0's before the actual end of the string.
*/
void SNPL_expanding_string::append(SNPL_expanding_string *e)
{
//cout << "appending: ";
//show_bytes(e->expanding_string, e->filled);
//cout << "from: ";
//show_bytes(expanding_string, filled);

  char *s = e->expanding_string;
  int len = e->filled; //strlen(s);
  if (len) {
    len += filled;
//cout << "\t\t\tappend(SNPL_expanding_string *)" << endl;
//cout << "\t\t\tlen = " << len << endl;
//cout << "\t\t\tactual_size() = " << actual_size() << endl;
    while (actual_size() < len)
      expand();
//cout << "\t\t\tactual_size() = " << actual_size() << endl;
    char *s2 = actual_string() + filled;
    for (int i=0; i<e->filled; i++) {
      *s2 = *s;
    //while ((*s2 = *s) != '\0') {
      s++;
      s2++;
    }
    filled = len;
  }
//cout << "result: ";
//show_bytes(actual_string(), filled);
}



/*
  Take the last 'len' characters off of the tail end of the string, and return
  them as a new string (used by executor.h).
*/
char *SNPL_expanding_string::remove(int len)
{
//cout << filled << ", removing a string of length = " << len << endl; cout.flush();
  if (len > filled)
    len = filled;
  char *s = new char[len+1], *s2 = s;
  int offset = filled-len;
  for (int i=0; i<len; i++)
  {
    *s2 = expanding_string[offset+i];
    s2++;
  }
  *s2 = '\0';
  filled -= len;
//cout << "strlen(s) = " << strlen(s) << endl; cout.flush();
  return s;
}



void SNPL_expanding_string::rewind(int len)
{
  if (len > filled)
    len = filled;
  filled -= len;
}



/*- end of file ---------------------------------------------------------------*/

