/*+
 * array.cpp
 *
 * author: Josh Shinavier
 * last edited: 11/18/04
 */

//#include <cstdio>
#include <cstdlib>
#include "array.h" 
using namespace std;

#ifdef DEFAULT_EXPANSION_FACTOR
  #undef DEFAULT_EXPANSION_FACTOR
#endif

// By default, expand() doubles the size of the array.
// Note: the expansion factor does not need to be an integer.
#define DEFAULT_EXPANSION_FACTOR 2



SNPL_array::SNPL_array(int size0, double expansion0)
            : head_index(0),
	      expansion_factor(expansion0),
              array_size(size0),
	      filled(0)
{
  if (array_size <= 0)
    array_size = 1;
  expanding_array = new void *[array_size];
}



SNPL_array::~SNPL_array()
{
  delete expanding_array;
}



void **SNPL_array::get_array()
{
  return expanding_array;
}



int SNPL_array::size()
{
  return filled;
}



double SNPL_array::expansion()
{
  return expansion_factor;
}
  


void SNPL_array::clear()
{
  filled = 0;
  head_index = 0;
}



/** Array resizing *//*========================================================*/



/*
 * (private) void SNPL_array::expand()
 *
 */
void SNPL_array::expand()
{
  // If expansion_factor is too small, the table might not actually gain in
  // integer size when it expand()'s, leading to a segmentation fault.
  // This is here (rather than in the constructor) in case a call to minimize()
  // precedes a call to expand(), which for certain parameters could cause
  // failure.
  if ((expansion_factor <= 1)
    ||(array_size == (int) (array_size*expansion_factor)))
    expansion_factor = DEFAULT_EXPANSION_FACTOR;
    
  int size0 = (int) (expansion_factor*array_size);
  void **array0 = new void *[size0];
  for (int i=0; i<filled; i++)
    array0[i] = expanding_array[(head_index+i)%array_size]; 
  delete expanding_array;
  expanding_array = array0;
  head_index = 0;
  array_size = size0;
}



void SNPL_array::minimize()
{
  if (filled != array_size) {
    void **array0;
    if (!filled) {
      array0 = new void *[1];
      array_size = 1;
    } else {
      array0 = new void *[filled];
      for (int i=0; i<filled; i++)
        array0[i] = expanding_array[(head_index+i)%array_size];
    }
    delete expanding_array;
    expanding_array = array0;
    array_size = filled;
    head_index = 0;
  }
}



/** Random access *//*=========================================================*/



void *SNPL_array::get(int index)
{
  if ((index >=0)&&(index < filled))
    return expanding_array[(index+head_index)%array_size];
  else
    return NULL;
}



void SNPL_array::set(int index, void *p)
{
  if ((index >=0)&&(index < filled))
    expanding_array[(index+head_index)%array_size] = p;
}



/** Stack and queue operations *//*============================================*/
/*
   push    : pre-decrement head_index (pre-resize if is_full)
   enqueue : post-increment tail_index (pre-resize if is_full)
   pop     : post-increment head_index (if array is non-empty)
   dequeue : pre-decrement tail_index (if array is non-empty)
*/


#include <iostream>
void SNPL_array::push(void *p)
{
cout << "Pushing " << (int) p << endl;
  if (filled == array_size)
    expand();
  head_index = ((head_index-1)+array_size)%array_size;
  expanding_array[head_index] = p;
  filled++;
}



void *SNPL_array::pop()
{
  if (filled) {
    void *p = expanding_array[head_index];
    head_index = (head_index+1)%array_size;
    filled--;
    return p;
  } else
    return NULL;
}



void SNPL_array::enqueue(void *p)
{
//cout << "Enqueuing " << (int) p << endl;
  if (filled == array_size)
    expand();
  expanding_array[(head_index+filled)%array_size] = p;
  filled++;
}



void *SNPL_array::dequeue()
{
  if (filled) {
    filled--;
    return expanding_array[(head_index+filled)%array_size];
  } else
    return NULL;
}



/** Insertion and removal *//*=================================================*/



//Note: memory copying (and occasional array resizing) overhead
void SNPL_array::insert_before(int index, void *p)
{
  if ((index >= 0)&&(index < filled)) {
    if (filled == array_size)
      expand();
    for (int i=filled; i>index; i--)
      expanding_array[(head_index+i)%array_size]
        = expanding_array[(head_index+i-1)%array_size];
    expanding_array[(head_index+index)%array_size] = p;
    filled++;     
  }
}



//Note: memory copying (and occasional array resizing) overhead
void SNPL_array::insert_after(int index, void *p)
{
  if ((index >= 0)&&(index < filled)) {
    if (filled == array_size)
      expand();
    for (int i=filled; i>(index+1); i--)
      expanding_array[(head_index+i)%array_size]
        = expanding_array[(head_index+i-1)%array_size];
    expanding_array[(head_index+index+1)%array_size] = p;
    filled++;     
  }
}



//Note: O(n) memory copying overhead
void SNPL_array::remove(int index)
{
  if ((index >= 0)&&(index < filled)) {
    for (int i=index; i<(filled-1); i++)
      expanding_array[(head_index+i)%array_size]
        = expanding_array[(head_index+i+1)%array_size];
    filled--;
  }
}



/*
  Removes an array item and replaces it with the last item in the array.
  DON'T USE if the order of your array elements is important. 
  O(c)
*/
void SNPL_array::simple_remove(int index)
{
  if ((index >= 0)&&(index < filled)) {
    expanding_array[(head_index+index)%array_size]
      = expanding_array[(head_index+filled-1)%array_size];
    filled--;
  }
}



/*- end of file --------------------------------------------------------------*/

