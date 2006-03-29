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

#include <stdio.h>  /* printf */
#include <string.h>  /* strcmp, strlen */
#include <stdlib.h>  /* exit (avoids 'implicit declaration' warning) */

#include <parser/Ast.h>
#include "p2_parser.h"


/** Bison parser dependency. */
extern int yyparse();


typedef struct Compiler
{
    int bogus;

} Compiler;


/******************************************************************************/


/** Mock command evaluator. */
int compiler__evaluate_command( Compiler *c, char *name, Ast *args )
{
    int ret = 0;
    c = 0;

    if ( args )
    {
        printf( "Evaluate command \"%s\" :  ", name );
        ast__print( args );
    }

    else
        printf( "Evaluate command \"%s\"", name );


    /* Debugger recognizes just one command. */
    if ( !strcmp( name, "quit" )
      || !strcmp( name, "q" ) )
        ret = 1;

    if ( args )
        ast__delete( args );

    free( name );

    return ret;
}


/** Mock expression evaluator. */
int compiler__evaluate_expression( Compiler *c, Name *name, Ast *expr )
{
    int ret = 0;
    Ast *a;
    c = 0;

    if ( name )
    {
        a = ast__name( name );
        printf( "Evaluate expression \"" );
        ast__print( a );
        ast__delete( a );
        printf( "\" :  ");
    }

    else
        printf( "Evaluate anonymous expression :  " );

    ast__print( expr );
    ast__delete( expr );

    return ret;
}


/** Mock parse error handler. */
int compiler__handle_parse_error( Compiler *c, char *msg )
{
    int ret = 0;
    c = 0;

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
boolean compiler__suppress_output( Compiler *c )
{
    c = 0;
    return FALSE;
}


/** \return  whether a line number is printed before each new line of input */
boolean compiler__show_line_numbers( Compiler *c )
{
    c = 0;
    return TRUE;
}


/******************************************************************************/


static int active = 0;

/** yyparse is invoked here. */
p2_parser__exit_state parse()
{
    p2_parser__exit_state return_state;
    int yyparse__exit_value;

    if ( active )
    {
        return exit_state__locked_out;
    }

    else
    {
        active = 1;
        if ( ( yyparse__exit_value = yyparse( 0, &return_state ) ) )
            printf( "Parser exited abnormally (exit_value = %d).\n", yyparse__exit_value );
        active = 0;

        return return_state;
    }
}


int main()
{
    printf( "Phase2 v%s command-line parser debugger.  Type '!quit;' to exit.\n\n", VERSION );

    switch( parse() )
    {
        case exit_state__aborted:
            printf( "Parser was aborted by a user action.\n" );
            break;
        case exit_state__end_of_input:
            printf( "Parser reached end-of-input.\n" );
            break;
        case exit_state__locked_out:
            printf( "Second concurrent activation of parse is not allowed.\n" );
            break;
        case exit_state__parse_failure:
            printf( "Parse failure.\n" );
            break;
    }

    return 0;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
