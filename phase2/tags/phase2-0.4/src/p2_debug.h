#ifndef P2_DEBUG_H
#define P2_DEBUG_H 1



// While the gSOAP DOM parser is still struggling with namespaces...
#define OMIT_XMLNS



//#define DEBUG
//#define PARSER_DEBUG


// If this is commented out, the parser will pass arrays of strings, rather than
// XML, for parser arguments.
//#define PASS_COMMAND_ARGS_AS_XML

#ifdef DEBUG
    #include <stdio.h>
#endif

#ifdef PARSER_DEBUG
  #define DEBUG_PRINTF printf
#else
  #define DEBUG_PRINTF
#endif



#endif
