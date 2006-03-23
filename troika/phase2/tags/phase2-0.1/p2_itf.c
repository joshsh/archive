/*+

  p2_itf.c

  Semantic module for the command line interface.

  author: Joshua Shinavier
  last edited: 4/19/05

  [unrelated] Kernighan ^ 1974 C tutorial: http://www.lysator.liu.se/c/bwk-tutor.html

*///////////////////////////////////////////////////////////////////////////////



//#include <regex.h>
#include <string.h>
#include "include/util/hash_table.h"
//#include "phase2.h"
#include "p2_debug.h"



#define LOOKUP_TABLE struct P2_lookup_table

//Note: the parameter will eventually be replaced with an XML object type.
#define COMMAND_FORMAT char *(*)(char *)
#define COMMAND(function_name) char * function_name (char *args)

HASH_TABLE *commands;



#define OUT_ "\t>> "
#define _OUT " <<\n"

char *decorate_output(char *str)
{
  const int len = 1 + strlen(OUT_) + strlen(_OUT);
  char *dec = (char *) malloc(sizeof(char)*(len+strlen(str)));
  strcpy(dec, OUT_);
  strcat(dec, str);
  strcat(dec, _OUT);
  return dec;
}



/** PARSER INTERFACE *//////////////////////////////////////////////////////////



// Just in case...
#ifdef __cplusplus
extern "C" {
#endif



void yyparse();



/**
  Expressions are the domain of the "language" itself.
*/
char *P2_evaluate_expression(char *name, char *expr)
{
  // Process the expression and send it through the language module

  if (name != NULL) {
    if (!is_valid_identifier(name))
      return decorate_output("Warning: dictionary assignment failed (bad identifier).");
    else {
      //- dictionary assignment
    }
  }

  //! temporary
  return strdup(expr);
}



/**
  Commands are defined here, in the interface module.
*/
char *P2_execute_command(char *name, char *args)
{
  char *s1, *s2;
  void *p = hash_table__lookup(commands, (void *) name);

  if (p == NULL)
    return decorate_output("Error: invalid command.");
  else {
    s1 = ((COMMAND_FORMAT) p)(args);
    s2 = decorate_output(s1);
    free(s1);
    return s2;
  }
}



#ifdef __cplusplus
}  // end extern "C"
#endif



/** COMMANDS *//////////////////////////////////////////////////////////////////



COMMAND(P2_exit)
{
  //! P2_destructor();

  hash_table__delete(commands);

  exit(0);

  return "";  // Unreachable, but syntactically necessary.
}



/** MISC *///////////////////////////////////////////////////////////////////////

int is_valid_identifier(char *s)
{
  return 1;
}



/** MAIN *//////////////////////////////////////////////////////////////////////



int main(int argv, char *args[])
{
  //! P2_constructor();

  /* Name all available commands and add them to the "commands" dictionary. */
  commands = hash_table__new(100, 2, 2, STRING_DEFAULTS);
  // Note: don't forget to garbage collect the "key" strings.
  hash_table__add(commands, (void *) "exit", (void *) P2_exit);

  // Hand over control to the (flex/bison) parser.
  yyparse();

}



/*- end of file */
