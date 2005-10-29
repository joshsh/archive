%{

/**

\file  p2_parser.y

\brief  Bison grammar file for the command line interface.

Phase2's command-line interface is a separable feature of the language. It
allows the user to pass arguments to either of a pair of functions defined in
the Phase2 client: one for program construction and one for querying or
manipulating the programming environment.

Expression Syntax

At the parser level, an expression is a parenthetically nested sequence of
dictionary items or special symbols, terminated by a semicolon. To give an
expression (that is, its "reduced" counterpart) a name, the semicolon may be
preceded by an equality symbol and then the name, e.g.

    (token1 token2) token3 = token4;

The unorthodox placement of the dictionary assignment command at the end of the
expression is aimed at command-line applications for which the programs you
write will not necessarily be read as you type them. After all, you might not
need to give an expression a name (particularly if you're only interested in the
side-effects); the trailing assignment command lets you put that decision off
till the last moment.

Command Syntax

Special commands are indicated with a slash plus the name of the command,
followed by a whitespace-delimited list of arguments (no parentheses) and
terminated by a semicolon, e.g.

    /command arg1 arg2;

Commands thus indicated do not belong to the program under construction, and are
to take immediate effect at parse time.

Interaction with the client

The role of the parser is simply to construct a p2_term to send to the client
for evaluation.  The client then handles the term and eventually deallocates it
after generating output.  The client cannot "talk back" to the parser.

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

#include <stdio.h>  // printf, sprintf
#include <string.h>  // strcpy


/** Command evaluator from the semantic module.
    \note  Both name and args (if not null) need to be freed externally. */
extern void p2_evaluate_command(char *name, p2_term *args);

/** Expression evaluator from the semantic module.
    \note  Both name (if not null) and expr need to be freed externally. */
extern void p2_evaluate_expression(char *name, p2_term *expr);

/** Parse error handler from the semantic module. */
extern void p2_handle_parse_error(char *msg);


////////////////////////////////////////////////////////////////////////////////


/** Current number of times yyparse() has been invoked. */
int input_number = 0;

/** Current line number.  Starts at 0. */
int line_number;

/** Current statement number.  Starts at 0 for each line of input. */
int statement_number;

extern int character_number;

int error_character_number, error_line_number, error_statement_number;
extern int last_character_number;

/** Error message to send to the semantic module. */
char error_msg[100];

/** Error message received by yyerror. */
char yyerror_msg[200];


void advance_input()
{
    input_number++;
    line_number = 0;
    statement_number = 0;
    character_number = 0;
    last_character_number = 0;
}


extern void advance_line()
{
    line_number++;
    statement_number = 0;
    character_number = 0;
    last_character_number = 0;
}


void advance_statement()
{
    statement_number++;
}


////////////////////////////////////////////////////////////////////////////////


int yywrap()
{
    // Parse only a single input stream.
    return 1;
}


/*  main() { yyparse(); }  */


void yyerror(const char *msg)
{
    if (*yyerror_msg)
        fprintf(stderr,
            "Parser warning: error reported by yyerror was never handled.\n");

    strcpy(yyerror_msg, msg);

    error_character_number = character_number;
    error_line_number = line_number;
    error_statement_number = statement_number;
}


////////////////////////////////////////////////////////////////////////////////


/** Evaluate a command. */
void handle_command(char *name, p2_term *args)
{
    #ifndef SUPPRESS_OUTPUT

        if (!statement_number)
            printf("\n");

        #ifdef COMMAND_OUTPUT_PREFIX
            printf(COMMAND_OUTPUT_PREFIX);
        #endif

    #ifndef SUPPRESS_OUTPUT

    p2_evaluate_command(name, args);

    #ifndef SUPPRESS_OUTPUT

        #ifdef COMMAND_OUTPUT_SUFFIX
            printf(COMMAND_OUTPUT_SUFFIX);
        #endif

        printf("\n");

    #ifndef SUPPRESS_OUTPUT
}


/** Evaluate an expression. */
void handle_expression(char *name, p2_term *expr)
{
    #ifndef SUPPRESS_OUTPUT

        if (!statement_number)
            printf("\n");

        #ifdef EXPRESSION_OUTPUT_PREFIX
            printf(EXPRESSION_OUTPUT_PREFIX);
        #endif

    #ifndef SUPPRESS_OUTPUT

    p2_evaluate_expression(name, expr);

    #ifndef SUPPRESS_OUTPUT

        #ifdef EXPRESSION_OUTPUT_SUFFIX
            printf(EXPRESSION_OUTPUT_SUFFIX);
        #endif

        printf("\n");

    #ifndef SUPPRESS_OUTPUT
}


/** Deal gracefully with a parse error.
    \note  the line and statement numbers in the error message reflect the
    position where the error was first detected. */
void handle_error(char *msg)
{
    #ifndef SUPPRESS_OUTPUT

        if (!statement_number)
            printf("\n");

        #ifdef ERROR_OUTPUT_PREFIX
            printf(ERROR_OUTPUT_PREFIX);
        #endif

    #endif

    // Note: "char" rather than "column" on account of the tab character.
    if (*yyerror_msg)
        sprintf(error_msg, "line %d, char %d: %s",
            error_line_number + 1, error_character_number + 1, yyerror_msg);
    else
        // If this happens, it means that the parser has not recovered from a
        // previous error, or that you've needlessly called handle_error.
        sprintf(error_msg, "unreported");
    *yyerror_msg = '\0';
    p2_handle_parse_error(error_msg);

    #ifndef SUPPRESS_OUTPUT

        #ifdef ERROR_OUTPUT_SUFFIX
            printf(ERROR_OUTPUT_SUFFIX);
        #endif

        printf("\n");

    #ifndef SUPPRESS_OUTPUT
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


/** To be passed as a function pointer to p2_term__for_all. */
void *deallocate(void *p)
{
    free(p);
    return (void *) 1;
}


/** Cleanup function for terms which own their atoms. */
void cleanup_term(p2_term *term)
{
    p2_term__for_all(term, deallocate);
    p2_term__delete(term);
}


////////////////////////////////////////////////////////////////////////////////


%}


%union
{
    char *string;

    /** (void *) instead of (p2_term *) because Bison won't take the alias. */
    void *parser_term;
}

%token YYERROR_VERBOSE

%token SEMICOLON EQUALS OPEN_PAREN CLOSE_PAREN NEWLINE E_O_F

%token <string> STRING
%token <string> COMMAND_NAME

%type <parser_term> term subterm arguments

%start parser_input


%%


parser_input:

    /* This production precedes input. */
    {
        ECHO("parser_input:  [null]");

        advance_input();
    }
    | parser_input command
    {
        ECHO("parser_input:  parser_input command");

        advance_statement();
    }
    | parser_input expression
    {
        ECHO("parser_input:  parser_input expression");

        advance_statement();
    }
    | parser_input SEMICOLON
    {
        ECHO("parser_input:  parser_input SEMICOLON");

        // Redundant semicolons are tolerated, but aren't recognized as
        // statements.
    }
    | parser_input error SEMICOLON
    {
        ECHO("parser_input:  error SEMICOLON");

        // Wait until the end of a statement to report an error (i.e. print
        // only one error message per invalid statement).
        handle_error("(invalid statement ignored)");

        // Make error reporting resume immediately.
        yyerrok;

        // Clear the lookahead token.
        yyclearin;

        advance_statement();
    };


command:

    COMMAND_NAME SEMICOLON
    {
        ECHO("command :  COMMAND SEMICOLON");

        // Handle command with no arguments.
        handle_command($1, 0);
    }
    | COMMAND_NAME arguments SEMICOLON
    {
        ECHO("command :  COMMAND arguments SEMICOLON");

        // Handle command with arguments.
        handle_command($1, (p2_term *) $2);
        free($1);
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
        free($3);
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

        if (!$2)
        {
            cleanup_term($1);
            ...
        }

        // Combine the terms using a left-associative merge.
        $$ = p2_term__merge_la((p2_term *) $1, (p2_term *) $2);
    }
    | term error
    {
        ECHO("term :  term error");

        cleanup_term((p2_term *) $1);
        $$ = 0;
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

