// Debugging ///////////////////////////////////////////////////////////////////

// #define LEXER_DEBUG
// #define PARSER_DEBUG

#ifdef PARSER_DEBUG
    #define DEBUG_PRINTF printf
#else
    #define DEBUG_PRINTF
#endif


// Formatting //////////////////////////////////////////////////////////////////

/** Added to each row of input (where defined). Used by the lexer. */
#define INPUT_PREFIX  ">>  "

/** Added to a block of output (where defined). Used by the parser. */
// #define COMMAND_OUTPUT_PREFIX "\t>> "
// #define EXPRESSION_OUTPUT_PREFIX "\t>> "
// #define COMMAND_OUTPUT_SUFFIX " <<"
// #define EXPRESSION_OUTPUT_SUFFIX " <<"


// 

/** Omit namespace prefixes ..........................
#define OMIT_XMLNS
