#ifndef DEBUG_H
#define DEBUG_H


enum parser_return_state
{
    RETURN_STATE__END_OF_INPUT,
    RETURN_STATE__ABORTED,
    RETURN_STATE__PARSE_FAILURE,
    RETURN_STATE__LOCKED_OUT
};


#endif  /* DEBUG_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
