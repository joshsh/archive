/*+
 * lookup_table.cpp
 *
 * These use a binary search to pair a string identifier with a named "item".
 *
 * author: Josh Shinavier
 * last edited: 12/4/04
 */

//#include <cstdlib>
#include <cstring>
#include "lookup_table.h"
using namespace std;



/**
 * SNPL_lookup_table::SNPL_lookup_table(universe *u, int size0)
 *
 *
 */
SNPL_lookup_table::SNPL_lookup_table(int size0, double expansion0)
       : SNPL_array(size0, expansion0)
{
  #ifdef lookup_table_DEBUG
    cout << "<> SNPL_lookup_table::SNPL_lookup_table(int, double)" << endl;
    cout.flush();
  #endif
}



/**
 * SNPL_lookup_table::~SNPL_lookup_table()
 *
 *
 */
SNPL_lookup_table::~SNPL_lookup_table()
{
  #ifdef lookup_table_DEBUG
    cout << "<> SNPL_lookup_table::~SNPL_lookup_table()" << endl;
    cout.flush();
  #endif

  for (int i=0; i<filled; i++)
    delete ((reference *) expanding_array[(head_index+i)%array_size]);
}



//Note: multiple points of exit.
void *SNPL_lookup_table::lookup(char *s)
{
  if (!filled)
    return NULL;
  int min=0, max=filled, mid = (min+max)/2;
  while (true) { //Break out when item is found or binary search is exhausted.
    reference *ref = (reference *) expanding_array[(head_index+mid)%array_size];
    int cmp = strcmp(s, ref->id);
    if (!cmp)
      return ref->ref;
    else {
      int mid0 = mid;
      if (cmp > 0) {
        min = mid;
        mid = (mid+max)/2;
      } else {
        max = mid;
        mid = (min+mid)/2;
      }
      if (mid == mid0)
        return NULL;
    }
  }
}



//Note: multiple points of exit.
void *SNPL_lookup_table::lookup_location(char *s)
{
  if (!filled)
    return NULL;
  int min=0, max=filled, mid = (min+max)/2;
  while (true) { //Break out when item is found or binary search is exhausted.
    reference *ref = (reference *) expanding_array[(head_index+mid)%array_size];
    int cmp = strcmp(s, ref->id);
    if (!cmp)
      return &(ref->ref);
    else {
      int mid0 = mid;
      if (cmp > 0) {
        min = mid;
        mid = (mid+max)/2;
      } else {
        max = mid;
        mid = (min+mid)/2;
      }
      if (mid == mid0)
        return NULL;
    }
  }
}



/*
  Note: a new reference will always overwrite an older reference with the same
  index (name).
*/
void SNPL_lookup_table::add(void *ref0, char *id0)
{
//cout << "inserting \"" << id0 << "\"" << endl; cout.flush();
  if (ref0 == NULL)
    return;
    
  void *newref = (void *) new reference(ref0, id0);
  if (!filled) {
    enqueue(newref);
    return;
  }

  int min=0, max=filled, mid = (min+max)/2;
  while (true) { //Break out when item is found or binary search is exhausted.
    reference *ref = (reference *) expanding_array[(head_index+mid)%array_size];
    int cmp = strcmp(id0, ref->id);
    if (!cmp) {  // (this reference is previously occupied)
      delete ref;
      expanding_array[(head_index+mid)%array_size] = (void *) newref;
      return;  
    } else {
      int mid0 = mid;
      if (cmp > 0) {// cout << ">: " << mid << " (" << ref->id << ")" << endl;
        min = mid;
        mid = (mid+max)/2;
      } else {// cout << "<: " << mid << " (" << ref->id << ")" << endl;
        max = mid;
        mid = (min+mid)/2;
      }
      if (mid == mid0) {
	if (cmp < 0)
	  insert_before(mid, newref);
	else
	  insert_after(mid, newref);
        return;
      }
    }
  }
}



//Note: multiple points of exit.
void SNPL_lookup_table::remove(char *id0)
{
  if (!filled)
    return;

  int min=0, max=filled, mid = (min+max)/2;
  while (true)  //Break out when item is found or binary search is exhausted.
  {
    reference *ref = (reference *) expanding_array[(head_index+mid)%array_size];
    int cmp = strcmp(id0, ref->id);
    if (!cmp)  // Entry found.  Remove it and left-shift the tail of the array.
    {
      filled--;
      for (int i=mid; i<filled; i++)
        expanding_array[(head_index+i)%array_size]
          = expanding_array[(head_index+i+1)%array_size];
      break;
    }
    else
    {
      int mid0 = mid;
      if (cmp > 0) {
        min = mid;
        mid = (mid+max)/2;
      } else {
        max = mid;
        mid = (min+mid)/2;
      }
      if (mid == mid0)
        break;
    }
  }
}



/**
 * void SNPL_lookup_table::display()
 *
 * For debugging purposes.
 */
#ifdef lookup_table_DEBUG
void SNPL_lookup_table::display()
{
  cout << '\t' << 'x' << ")\t" << filled << " {";
  for (int i=0; i<filled; i++) {
    cout << " " << ((reference *) expanding_array[(head_index+i)%array_size])->id;
    if (i < (filled-1))
      cout << " <";
  }
  cout << " }\n";
}
#endif



char **SNPL_lookup_table::list_all()
{
  char **l = new char *[filled];
  for (int i=0; i<filled; i++) {
    reference *ref
      = (reference *) expanding_array[(i+head_index)%array_size];
    l[i] = new char[strlen(ref->id)+1];
    strcpy(l[i],ref->id);
  }
  return l;
}



/*- end of file --------------------------------------------------------------*/

