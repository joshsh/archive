/**
    \file  p2_parser-debugger.c

    \brief  Debugging/testing application for the Phase2 command line parser.

    \note  This is not part of the Phase2 build.

    \author  Joshua Shinavier   \n
             parcour@gmail.com  \n
             +1 509 570-6990    \n */

/*//////////////////////////////////////////////////////////////////////////////

Phase2 language API, Copyright (C) 2005 Joshua Shinavier.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

*///////////////////////////////////////////////////////////////////////////////

#include "p2_parser-macros.h"
#include "../util/p2_term.h"

#include <stdio.h>  // printf


/** Debugging print function for p2_terms. */
void p2_term__print(p2_term *term)
{
    #ifdef PRINT_TERM_AS_ARRAY
        void **cur = term->head, **sup = term->buffer + term->buffer_size;

        while (cur < sup)
        {
            printf("%d ", (int) *cur);
            cur++;
        }
    #else
        unsigned int i, length = p2_term__length(term);

        if (length == 1)
            printf((char *) *(term->head + 1));

        else
        {
            printf("(");
            for (i = 0; i < length - 1; i++)
            {
                p2_term__print(p2_term__subterm_at(term, i));
                printf(" ");
            }
            p2_term__print(p2_term__subterm_at(term, length - 1));
            printf(")");
        }
    #endif  // PRINT_TERM_AS_ARRAY
}


/** To be passed as a function pointer to p2_term__for_all. */
void *deallocate(void *p)
{
    free(p);
    return (void *) 1;
}


////////////////////////////////////////////////////////////////////////////////


/** The Bison parser function. */
extern int yyparse();


/** Mock command evaluator. */
extern void p2_evaluate_command(char *name, p2_term *args)
{
    if (args)
    {
        printf("Command \"%s\": ", name);
        p2_term__print(args);
        printf("\n");

        p2_term__for_all(args, deallocate);
        p2_term__delete(args);
    }

    else
        printf("Command \"%s\"\n", name);

    free(name);
}


/** Mock expression evaluator. */
extern void p2_evaluate_expression(char *name, p2_term *expr)
{
    if (name)
    {
        printf("Expression \"%s\": ", name);
        p2_term__print(expr);
        printf("\n");

        free(name);
    }

    else
    {
        printf("Anonymous expression: ");  fflush(stdout);
        p2_term__print(expr);
        printf("\n");
    }

    // Expression argument should never be null.
    p2_term__for_all(expr, deallocate);
    p2_term__delete(expr);
}


/** Mock parse error handler. */
extern void p2_handle_parse_error(char *msg)
{
    fprintf(stderr,"Parse error: %s\n", msg);
}


////////////////////////////////////////////////////////////////////////////////


main()
{
    while (1)  // Break out of the debugger by killing the process.
    {
        if (yyparse())
            printf("Parser (would have) exited abnormally.\n");
        else
            printf("Parser (would have) exited normally.\n");
    }
}

