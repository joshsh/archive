#include "p2_parser-aux.h"

#include <stdio.h>  // printf


// Formatting flags, with accessors ////////////////////////////////////////////


/** Flags for output formatting. */
#ifdef SUPPRESS_OUTPUT
    int suppress_output = 1;
#else
    int suppress_output = 0;
#endif
#ifdef SHOW_LINE_NUMBERS
    int show_line_numbers = 1;
#else
    int show_line_numbers = 0;
#endif


void p2_parser__set_suppress_output(int flag)
{
    suppress_output = flag;
}


void p2_parser__set_show_line_numbers(int flag)
{
    show_line_numbers = flag;
}

