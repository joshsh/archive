#ifndef LOOKUP_TABLE_H
#define LOOKUP_TABLE_H

/*+
 * lookup_table.h
 *
 * These use a binary search to pair a string identifier with a named "item".
 *
 * author: Josh Shinavier
 * last edited: 12/4/04
 */

#include "array.h"
#include <cstdlib>
using namespace std;

/*
  Bit of a modularity issue...
  ... but it's simplest to put this here, as lookup_table_itf.h uses the
  "reference" type explicitly, and I don't want to create two more files for such
  a tiny class.
*/
class reference
{
public:
  char *id;
  void *ref;
  reference(void *ref0, char *id0)
           : ref(ref0),
      id(strcpy((char *) malloc(1+strlen(id0)), id0))
  {}
  ~reference()
  {
    delete id;
  }
};

class SNPL_lookup_table : public SNPL_array
{

public:

  SNPL_lookup_table(int, double);

  ~SNPL_lookup_table();

  void *lookup(char *);
  
  //! Violates modularity, but necessary for lookup_table_itf.h.
  void *lookup_location(char *);

  void add(void *, char *);

  void remove(char *);

  char **list_all();

#ifdef lookup_table_DEBUG
  void display();
#endif

};



/*- end of file --------------------------------------------------------------*/

#endif
