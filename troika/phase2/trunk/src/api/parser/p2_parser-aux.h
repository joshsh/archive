/** An arbitrary value indicating that the parser was terminated by a user
    action, rather than by reaching the end of input. */
#define FORCED_EXIT 1331


enum p2_parser_item__type { URI, TERM, BAG };

typedef struct _p2_parser_item
{
    enum p2_parser_item__type type;

    void *value;

} p2_parser_atom;


// Debugging flags /////////////////////////////////////////////////////////////

//#define DEBUG

#ifdef DEBUG

    /** If defined, echo each token as it is matched by the lexer. */
    #define LEXER_DEBUG

    /** If defined, echo each production as it is matched by the parser. */
    #define PARSER_DEBUG

#endif  // DEBUG

/** If defined, print terms as raw integer sequences, rather than as
    parenthesized expressions. */
//#define PRINT_TERM_AS_ARRAY


// Formatting flags ////////////////////////////////////////////////////////////

/** If defined, prevent all output from the parser module (apart from debugging
    messages). */
// #define SUPPRESS_OUTPUT

/** If defined, begin each new line of input with a line number. */
#define SHOW_INPUT_LINENO

/** Added to each row of input (if defined). Used by the lexer. */
#define INPUT_PREFIX  ">>  "

/** Added to a block of output (if defined). Used by the parser. */
// #define COMMAND_OUTPUT_PREFIX "\t>> "
// #define EXPRESSION_OUTPUT_PREFIX "\t>> "
// #define ERROR_OUTPUT_PREFIX "\t>> "
// #define COMMAND_OUTPUT_SUFFIX " <<"
// #define EXPRESSION_OUTPUT_SUFFIX " <<"
// #define ERROR_OUTPUT_SUFFIX " <<"


// Declarations ////////////////////////////////////////////////////////////////


/** Turn output suppression on or off. */
void p2_parser__set_suppress_output(int flag);

/** Turn line numbers on or off. */
void p2_parser__set_show_line_numbers(int flag);

