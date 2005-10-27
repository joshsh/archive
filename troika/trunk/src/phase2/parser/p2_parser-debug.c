/**
    \file  p2_parser-debug.c

    \brief  Debugging/testing application for the Phase2 command line parser.

    \note  This is not part of the Phase2 build proper.

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

#include <stdio.h>  // printf



/** The Bison parser function. */
extern void yyparse();



/** Mock command evaluator. */
extern void p2_evaluate_command(char *name, char *args)
{
    printf("Command \"%s\":\n%s", name, args);
}



/** Mock expression evaluator. */
extern void p2_evaluate_expression(char *name, char *expr)
{
    if (name)
        printf("Expression \"%s\":\n%s", name, expr);
    else
        printf("Anonymous expression:\n%s", expr);
}



main()
{
    yyparse();
}


