%{

/**
    \file  p2_parser.y

    \brief  Bison grammar file for the command line interface.

    Phase2's command-line interface is a separable feature of the language. It
    allows the user to pass arguments to either of a pair of functions defined
    in the Phase2 client: one for program construction and one for querying or
    manipulating the programming environment.

    Expression Syntax

    At the parser level, an expression is a parenthetically nested sequence of
    dictionary items or special symbols, terminated by a semicolon. To give an
    expression (that is, its "reduced" counterpart) a name, the semicolon may be
    preceded by an equality symbol and then the name, e.g.

        (token1 token2) token3 = token4;

    The unorthodox placement of the dictionary assignment command at the end of
    the expression is aimed at command-line applications for which the programs
    you write will not necessarily be read as you type them. After all, you
    might not need to give an expression a name (particularly if you're only
    interested in the side-effects); the trailing assignment command lets you
    put that decision off till the last moment.

    Command Syntax

    Special commands are indicated with a slash plus the name of the command,
    followed by a whitespace-delimited list of arguments (no parentheses) and
    terminated by a semicolon, e.g.

        /command arg1 arg2;

    Commands thus indicated do not belong to the program under construction, and
    are to take immediate effect at parse time.

    Interaction with the client

    The role of the parser is simply to construct a p2_term to send to the
    client for evaluation.  The client then handles the term and eventually
    deallocates it after generating output.  The client cannot "talk back" to
    the parser.

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

#include <stdio.h>  // fprintf, printf


/** Command evaluator from the semantic module.
    \note  Both name and args (if not null) need to be freed externally. */
extern void p2_evaluate_command(char *name, p2_term *args);

/** Expression evaluator from the semantic module.
    \note  Both name (if not null) and expr need to be freed externally. */
extern void p2_evaluate_expression(char *name, p2_term *expr);

/** Parse error handler from the semantic module. */
extern void p2_handle_parse_error(char *msg);


////////////////////////////////////////////////////////////////////////////////


int yywrap()
{
    return 1;
}


/*  main() { yyparse(); }  */


void yyerror(const char *msg) { }


/** Evaluate a command, decorating output. */
void handle_command(char *name, p2_term *args)
{
    printf("\n");

    #ifdef COMMAND_OUTPUT_PREFIX
        printf(COMMAND_OUTPUT_PREFIX);
    #endif

    p2_evaluate_command(name, args);

    #ifdef COMMAND_OUTPUT_SUFFIX
        printf(COMMAND_OUTPUT_SUFFIX);
    #endif

    printf("\n");
}


/** Evaluate a expression, decorating output. */
void handle_expression(char *name, p2_term *expr)
{
    printf("\n");

    #ifdef EXPRESSION_OUTPUT_PREFIX
        printf(EXPRESSION_OUTPUT_PREFIX);
    #endif

    p2_evaluate_expression(name, expr);

    #ifdef EXPRESSION_OUTPUT_SUFFIX
        printf(EXPRESSION_OUTPUT_SUFFIX);
    #endif

    printf("\n");
}


void handle_error(char *msg)
{
    printf("\n");

    #ifdef ERROR_OUTPUT_PREFIX
        printf(ERROR_OUTPUT_PREFIX);
    #endif

    p2_handle_parse_error(msg);

    #ifdef ERROR_OUTPUT_SUFFIX
        printf(ERROR_OUTPUT_SUFFIX);
    #endif

    printf("\n");
}


////////////////////////////////////////////////////////////////////////////////


#ifdef PARSER_DEBUG
    #define ECHO echo_production
#else
    #define ECHO
#endif


/** Debugging output. */
void echo_production(char *s)
{
    printf("Found %s\n", s);
}


////////////////////////////////////////////////////////////////////////////////

%}


%union
{
    char *string;

    /** (void *) instead of (p2_term *) because Bison won't take the alias. */
    void *parser_term;
}

%token SEMICOLON EQUALS OPEN_PAREN CLOSE_PAREN

%token <string> STRING
%token <string> COMMAND

%type <parser_term> term subterm arguments


%%


parser_input:

    /* This production precedes input. */
    {
        ECHO("parser_input:  [null]");

        // Global variables could be initialized here.

        #ifdef INPUT_PREFIX
            printf(INPUT_PREFIX);  // Can the lexer do this?
        #else
            printf("\n");
        #endif
    }
    | parser_input command
    {
        ECHO("parser_input:  parser_input command");
    }
    | parser_input expression
    {
        ECHO("parser_input:  parser_input expression");
    }
    | parser_input SEMICOLON
    {
        ECHO("parser_input:  parser_input SEMICOLON");

        // Redundant semicolons are permitted.
    }
    | error SEMICOLON
    {
        ECHO("parser_input:  error SEMICOLON");

        // Wait until the end of a statement to report an error (i.e. print
        // only one error message per invalid statement).
        handle_error("invalid statement");
    };


command:

    COMMAND SEMICOLON
    {
        ECHO("command :  COMMAND SEMICOLON");

        // Handle command with no arguments.
        handle_command($1, 0);
    }
    | COMMAND arguments SEMICOLON
    {
        ECHO("command :  COMMAND arguments SEMICOLON");

        // Handle command with arguments.
        handle_command($1, (p2_term *) $2);
    };


expression:

    term SEMICOLON
    {
        ECHO("expression :  term SEMICOLON");

        // Handle anonymous expression.
        handle_expression(0, (p2_term *) $1);
    }
    | term EQUALS STRING SEMICOLON
    {
        ECHO("expression :  term EQUALS token SEMICOLON");

        // Handle named expression.
        handle_expression($3, (p2_term *) $1);
    };


arguments:

    STRING
    {
        ECHO("arguments :  token");

        // Create a singleton term containing one argument.
        $$ = p2_term__new((void *) $1, 1);
    }
    | arguments STRING
    {
        ECHO("arguments :  arguments token");

        // Concatenate the command arguments.
        $$ = p2_term__cat((p2_term *) $1, p2_term__new((void *) $2, 1));
    };


term:

    subterm
    {
        ECHO("term :  subterm");
        $$ = $1;
    }
    | term subterm
    {
        ECHO("term :  term subterm");

        // Combine the terms using a left-associative merge.
        $$ = p2_term__merge_la((p2_term *) $1, (p2_term *) $2);
    };


subterm:

    STRING
    {
        ECHO("subterm :  token");

        // Create a singleton term containing one literal or identifier.
        $$ = p2_term__new((void *) $1, 1);
    }
    | OPEN_PAREN term CLOSE_PAREN
    {
        ECHO("subterm :  OPEN_PAREN term CLOSE_PAREN");

        // "Remove the parentheses" from the term.
        $$ = $2;
    };

