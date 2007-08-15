/*+
 * function.h
 *
 * A wrapper for specialized member functions which define basic operations for
 * a user-accessible class.
 *
 * author: Josh Shinavier
 * last edited: 7/3/04
 */

//  We can afford to include all this type info as there should be only a
// limited number of "function" objects (in contrast to "procedure" objects).

class function
{
  void (*function_ptr)(class item**);
  char *function_name;
  class void_array *function_arguments;

public:

  function(void (*)(class item **), char *, char *);
  
  char *name();
  
  void (*ptr())(class item**);
  
  class void_array *arguments();

};

/*- end of file --------------------------------------------------------------*/
