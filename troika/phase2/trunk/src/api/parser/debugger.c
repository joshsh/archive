/**

\file  debugger.c

\brief  Example implementation of the Phase2 command line parser.

\note  This is not part of the Phase2 build.

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

/*******************************************************************************

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

*******************************************************************************/

#include "p2_ast.h"
#include "debug.h"

#include "../util/p2_term.h"

#include <stdio.h>  /* printf */
#include <string.h>  /* strcmp, strlen */
#include <stdlib.h>  /* exit (avoids 'implicit declaration' warning) */


/** Bison parser dependency. */
extern int yyparse();


/******************************************************************************/


/** Mock command evaluator. */
int p2_evaluate_command( char *name, p2_ast *args )
{
    int ret = 0;

    if ( args )
    {
        printf( "Evaluate command \"%s\" :  ", name );
        p2_ast__print( args );
    }

    else
        printf( "Evaluate command \"%s\"", name );


    /* Debugger recognizes just one command. */
    if ( !strcmp( name, "exit" ) )
        ret = 1;

    if ( args )
        p2_ast__delete( args );

    free( name );

    return ret;
}


/** Mock expression evaluator. */
int p2_evaluate_expression( p2_name *name, p2_ast *expr )
{
    int ret = 0;
    p2_ast *a;

    if ( name )
    {
        a = p2_ast__name( name );
        printf( "Evaluate expression \"" );
        p2_ast__print( a );
        p2_ast__delete( a );
        printf( "\" :  ");
    }

    else
        printf( "Evaluate anonymous expression :  " );

    p2_ast__print( expr );
    p2_ast__delete( expr );

    return ret;
}


/** Mock parse error handler. */
int p2_handle_parse_error( char *msg )
{
    int ret = 0;

    if ( msg )
    {
        printf( "Handle parse error :  %s", msg );
        free( msg );
    }

    else
        printf( "Handle parse error" );

    return ret;
}


/** \return  whether the lexer and parser are to avoid printing to stdout while
    matching input */
int p2_parser__suppress_output()
{
    return 0;
}


/** \return  whether a line number is printed before each new line of input */
int p2_parser__show_line_numbers()
{
    return 1;
}


/******************************************************************************/


/** yyparse is invoked here. */
int main()
{
    int yyparse__exit_value;
    enum parser_return_state return_state;

    printf( "Phase2 command-line parser debugger.  Type '\\exit;' to quit.\n\n" );

    if ( ( yyparse__exit_value = yyparse( &return_state ) ) )
        printf( "Parser exited abnormally (exit_value = %d).\n", yyparse__exit_value );

    else if ( return_state == RETURN_STATE__ABORTED )
        printf( "Parser was aborted by a user action.\n" );
    else
        printf( "Parser reached end-of-input.\n" );

    return 0;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
