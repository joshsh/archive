/*+

  data_type.cpp

  The SNPL_data_type structure is a wrapper for SNPL's "internal" and "external"
  data types.  For internal types, it describes their constituents, while for
  external types it contains references to type-specific serializers,
  deserializers and destructors.
  Certain data types also have a preserializer, ###

  author: Josh Shinavier
  last edited: 12/9/04

 */

#include "snpl.h"
#include <cstdlib>
#include <fstream>
#include "util/array.h"
#include "util/associative_array.h"
#include "util/strutil.h"
using namespace std;



SNPL_data_type *SNPL_data_type_constructor(
  char *name,
  void (*serializer0)(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &),
  void *(*deserializing_constructor0)(ifstream &),
  void (*destructor0)(void *),
  void (*preserializer0)(void *, SNPL_array *))
{
  SNPL_data_type *dt =
    (SNPL_data_type *) malloc(sizeof(SNPL_data_type));
  dt->serializer = serializer0;
  dt->deserializing_constructor = deserializing_constructor0;
  dt->destructor = destructor0;
  dt->preserializer = preserializer0;
  dt->name = SNPL_copy_cstring(name);
  return dt;
}



//void SNPL_data_type_destructor(SNPL_data_type *dt)
//{
//  delete dt->name;
//  free(dt);
//}



/*- end of file --------------------------------------------------------------*/

