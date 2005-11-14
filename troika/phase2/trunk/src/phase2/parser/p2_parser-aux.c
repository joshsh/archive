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


// Formatting and error reporting functions ////////////////////////////////////


void new_parse()
{
    line_number = character_number = last_character_number = 0;

    print_input_prefix();
}


void print_input_prefix()
{
    if (!suppress_output)
    {
        // Display a line number at the beginning of each new line of input.
        if (show_line_numbers)
            printf("%d ", line_number + 1);  // 1-index the line number.

        // Add a prefix to each new line of input, to distinguish it from
        // output, and possibly to separate it from the line number.
        #ifdef INPUT_PREFIX
            printf(INPUT_PREFIX);
        #endif
    }
}


void advance_character_number(int n_chars)
{
    last_character_number = character_number;
    character_number += n_chars;
}


void advance_line_number()
{
    line_number++;
    statement_number = character_number = last_character_number = 0;
}


void advance_statement_number()
{
    statement_number++;
}

