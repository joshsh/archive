#ifndef DEBUG_H
#define DEBUG_H


#define DEBUG  0

#if DEBUG

    /** Echo each token as it is matched by the lexer. */
    #define DEBUG__LEXER  1

    /** Echo each production as it is matched by the parser. */
    #define DEBUG__PARSER  1

    #if DEBUG__PARSER

        /** Use Bison's built-in trace facility. */
        #define DEBUG__PARSER__USE_YYDEBUG  0

    #endif

#endif


enum parser_return_state
{
    RETURN_STATE__END_OF_INPUT,
    RETURN_STATE__ABORTED,
    RETURN_STATE__PARSE_FAILURE
};


#endif  /* DEBUG_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
