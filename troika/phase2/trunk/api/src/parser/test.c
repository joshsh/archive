/**

\file  parser-test.c

\brief  Example implementation of the Phase2 command line parser.

\note  The compiled program is not installed.

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

/*******************************************************************************

Phase2 language API, Copyright (C) 2006 Joshua Shinavier.

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

/*
#include <string.h>
#include <stdlib.h>
*/

#include <Ast.h>
#include <common.h>
#include "../settings.h"


typedef struct Interpreter Interpreter;

struct Interpreter
{
    int bogus;
};


/** Bison parser dependency. */
extern int
yyparse( Interpreter *c, p2_parser__exit_state *es );


/******************************************************************************/


/** Mock command evaluator. */
int
interpreter__evaluate_command( Interpreter *c, char *name, Ast *args )
{
    int ret = 0;
    c = 0;

    if ( args )
    {
        PRINT( "Evaluate command \"%s\" :  ", name );
        ast__print( args );
    }

    else
        PRINT( "Evaluate command \"%s\"", name );

    PRINT( "\n" );

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
int
interpreter__evaluate_expression( Interpreter *c, Name *name, Ast *expr )
{
    int ret = 0;
    Ast *a;
    c = 0;

    if ( name )
    {
        a = ast__name( name );
        PRINT( "Evaluate expression \"" );
        ast__print( a );
        ast__delete( a );
        PRINT( "\" :  ");
    }

    else
        PRINT( "Evaluate anonymous expression :  " );

    ast__print( expr );

    PRINT( "\n" );

    ast__delete( expr );

    return ret;
}


/** Mock parse error handler. */
int
interpreter__handle_parse_error( Interpreter *c, char *msg )
{
    int ret = 0;
    c = 0;

    if ( msg )
    {
        PRINT( "Handle parse error :  %s", msg );
        free( msg );
    }

    else
        PRINT( "Handle parse error" );

    PRINT( "\n" );

    return ret;
}


/** \return  whether the lexer and parser are to avoid printing to stdout while
    matching input */
boolean
interpreter__quiet( Interpreter *c )
{
    c = 0;
    return FALSE;
}


/** \return  whether a line number is printed before each new line of input */
boolean
interpreter__show_line_numbers( Interpreter *c )
{
    c = 0;
    return TRUE;
}


/******************************************************************************/


static int active = 0;

/** yyparse is invoked here. */
p2_parser__exit_state
parse( void )
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
            PRINT( "Parser exited abnormally (exit_value = %d).\n", yyparse__exit_value );
        active = 0;

        return return_state;
    }
}


int
main( int argc, char *argv[] )
{
    /* Avoid compiler warnings. */
    argc = 0;
    argv = 0;

    PRINT( "Phase2 v%s command-line parser debugger.  Type '_quit;' to exit.\n", VERSION );

    switch( parse() )
    {
        case exit_state__aborted:
            PRINT( "Parser was aborted by a user action.\n" );
            break;
        case exit_state__end_of_input:
            PRINT( "Parser reached end-of-input.\n" );
            break;
        case exit_state__locked_out:
            PRINT( "Second concurrent activation of parse is not allowed.\n" );
            break;
        case exit_state__parse_failure:
            PRINT( "Parse failure.\n" );
            break;
    }

    return 0;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
