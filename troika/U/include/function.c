/*+
 * function.c
 *
 * A wrapper for specialized member functions which define basic operations for
 * a user-accessible class.
 *
 * author: Josh Shinavier
 * last edited: 7/3/04
 */


 
function::function(void (*function_ptr0)(item**),
                   char *function_name0,
                   char *arg_description0)
          : function_ptr(function_ptr0),
            function_name(ascii::copy(function_name0))
{
  #ifdef FUNCTION_DEBUG
    cout << "<> function::function(void (*)(item**), char *, char *): \"" << function_name << "\"" << endl;
    cout.flush();
  #endif

  char *arg_description = ascii::copy(arg_description0);
  
  char *s = arg_description;
  int args = 0;
  while (*s != '\0') {
    if (*s == ':')
      args++;
    s++;
  }
  function_arguments = new void_array(args);
  s = arg_description;
  while (*s != '\0') {
    bool readwrite = (*s == 'w');
    s += 2;
    char *s2 = s;
    while ((int) *s > 32)
      s++;
    char ch = *s;
    *s = '\0';
    function_arguments->append(
      (void *) new argument(identify_classe(s2), readwrite));
    *s = ch;    
    while (*s && *s <= 32)
      s++;
  }
  //function_arguments->minimize();
  delete arg_description;
}



char *function::name()
{
  return function_name;
}



void (*function::ptr())(item**)
{
  return function_ptr;
}



/**
 * void_array *function::arguments()
 *
 *
 */
void_array *function::arguments()
{
  return function_arguments;
}



/*- end of file --------------------------------------------------------------*/
