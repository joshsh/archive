/**

\file  p2_parser-debugger.c

\brief  Example implementation of the Phase2 command line parser.

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

#include "p2_parser-aux.h"
#include "../util/p2_term.h"

#include <stdio.h>  // printf
#include <string.h>  // strcmp, strlen
#include <stdlib.h>  // exit (avoids 'implicit declaration' warning)


void p2_term__print(p2_term *term, int top_level);
void *deallocate(void *p);


////////////////////////////////////////////////////////////////////////////////


/** The Bison parser function. */
extern int yyparse();

/** Term cleanup function from Bison output. */
extern void cleanup_term(p2_term *term);

extern void p2_parser__set_suppress_output(int flag);
extern void p2_parser__set_show_line_numbers(int flag);


/** Mock command evaluator.
    \note  the p2_term argument 'args' should be freed after use (if not null),
    whereas the character string 'name' belongs to the parser. */
extern void p2_evaluate_command(char *name, p2_term *args)
{
    int quit = 0;

    if (!name || !strlen(name))
    {
        printf("Error: command must have a name!");
        return;
    }

    if (args)
    {
        printf("Evaluate command \"%s\": ", name);
        p2_term__print(args, 1);
        //printf("\n");
    }

    else
        printf("Evaluate command \"%s\"", name);


    // Process the command.
    if (!strcmp(name, "exit"))
        quit = 1;
    // else
    //    printf("Unknown command.\n");

    if (args)
        cleanup_term(args);

    if (quit)
    {
        printf("\n\n");

        /** \warning  This way, yyparse never terminates. */
        exit(0);
    }
}


/** Mock expression evaluator.
    \note  the p2_term argument 'args' should be freed after use,
    whereas the character string 'name' belongs to the parser. */
extern void p2_evaluate_expression(char *name, p2_term *expr)
{
    if (!expr)
    {
        printf("Error: expression must not be null!");
        return;
    }

    if (name)
    {
        printf("Evaluate expression \"%s\": ", name);
        p2_term__print(expr, 1);
    }

    else
    {
        printf("Evaluate anonymous expression: ");
        p2_term__print(expr, 1);
    }

    cleanup_term(expr);
}


/** Mock parse error handler.
    \note  the character string 'msg' belongs to the parser. */
extern void p2_handle_parse_error(char *msg)
{
    if (msg && strlen(msg))
        printf("Handle parse error: %s.", msg);
    else
        printf("Handle parse error.");
}


////////////////////////////////////////////////////////////////////////////////


/** Debugger controlling function.  Invocation of yyparse() is here. */
main()
{
    p2_parser__set_suppress_output(0);
    p2_parser__set_show_line_numbers(1);

    printf("Phase2 command-line parser debugger.  Type '/exit' to quit.\n\n");

    if (yyparse())
        printf("Parser exited abnormally.\n");
    else
        printf("Parser exited normally.\n");
}


/** Print function for p2_terms. */
void p2_term__print(p2_term *term, int top_level)
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
            if (!top_level)
                printf("(");

            for (i = 0; i < length - 1; i++)
            {
                p2_term__print(p2_term__subterm_at(term, i), 0);
                printf(" ");
            }
            p2_term__print(p2_term__subterm_at(term, length - 1), 0);

            if (!top_level)
                printf(")");
        }
    #endif  // PRINT_TERM_AS_ARRAY
}

