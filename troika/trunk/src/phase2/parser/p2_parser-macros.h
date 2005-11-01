// Debugging ///////////////////////////////////////////////////////////////////

/** If defined, echo each token as it is matched by the lexer. */
// #define LEXER_DEBUG

/** If defined, echo each production as it is matched by the parser. */
// #define PARSER_DEBUG

/** If defined, print terms as raw integer sequences, rather than as
    parenthesized expressions. */
// #define PRINT_TERM_AS_ARRAY


// Formatting //////////////////////////////////////////////////////////////////

/** If defined, prevent all output from the parser module (apart from debugging
    messages). */
// define SUPPRESS_OUTPUT

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

/** Flag for output suppression. */
int suppress_output;


/** Current "column" number, where tabs only count for one column. */
int character_number, last_character_number;

/** Current line number.  Starts at 0. */
int line_number;

/** Current statement number.  Starts at 0 for each line of input. */
int statement_number;


/** Registers a new column offset. */
void advance_character_number(int nchars);

/** Registers a new line of input. */
void advance_line_number();

/** Registers a new statement. */
void advance_statement_number();

