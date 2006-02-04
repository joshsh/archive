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
expression a name, the semicolon may be preceded by an equality symbol and
hen the name, e.g.

    (token1 token2) token3 = token4;

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
after generating output.  The client cannot "talk back" to the parser except to
tell it to terminate.

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

#include <stdio.h>  // fprintf, printf, sprintf
#include <string.h>  // strcpy

#ifdef PARSER_DEBUG
    #define YYDEBUG 1
#endif


////////////////////////////////////////////////////////////////////////////////

/** Command evaluator from the semantic module.
    \note  args (if not null) needs to be freed externally. */
extern int p2_evaluate_command(char *name, p2_term *args);

/** Expression evaluator from the semantic module.
    \note  expr needs to be freed externally. */
extern int p2_evaluate_expression(char *name, p2_term *expr);

/** Parse error handler from the semantic module. */
extern int p2_handle_parse_error(char *msg);


////////////////////////////////////////////////////////////////////////////////

/** Evaluate a command. */
int handle_command(char *name, p2_term *args);

/** Evaluate an expression. */
int handle_expression(char *name, p2_term *expr);

/** Deal gracefully with a parse error. */
int handle_error(char *msg);


////////////////////////////////////////////////////////////////////////////////

extern int last_character_number, line_number, statement_number;
extern int suppress_output, show_line_numbers;
int error_character_number, error_line_number, error_statement_number;


/** "verbose" error message received by yyerror. */
char yyerror_msg[200];

/** Adorned (with location info.) error message to send to the semantic module. */
char error_msg[100];


extern void new_parse();
extern void advance_statement_number();


////////////////////////////////////////////////////////////////////////////////

/** This is the default value. */
#define YYMAXDEPTH    10000

/** This is the default value. */
#define YYINITDEPTH   200

/** Tells the parser to process only a single input stream. */
int yywrap();

/** Copies reported error messages to a string, where they wait to passed on to
    the semantic module. */
void yyerror(const char *msg);

/** Causes the last error reported to yyerror to be ignored. */
void skip_error();

//extern void yyterminate();
//extern void yy_fatal_error(char *);
/** Exit the parser (before reaching the end of input). */
#define END_IT_ALL  { /*yylex(0,0);*/ YYABORT; }


////////////////////////////////////////////////////////////////////////////////


#ifdef PARSER_DEBUG
    #define ECHO echo_production
#else
    #define ECHO
#endif

/** Debugging output. */
void echo_production(char *s);

/** Cleanup function for terms which own their atoms. */
void cleanup_term(p2_term *term);


////////////////////////////////////////////////////////////////////////////////

%}


%union
{
    char *string;

    /** (void *) instead of (p2_term *) because Bison won't take the alias. */
    void *parser_term;
}

/** Report more detailed parse error messages. */
%token YYERROR_VERBOSE

%token OPEN_PAREN CLOSE_PAREN EQUALS SEMICOLON E_O_F

%token <string> STRING COMMAND_NAME

%type <parser_term> term subterm arguments

%start parser_input

%%


parser_input:

    statements E_O_F
    {
        ECHO("parser_input:  statements E_O_F");

        /** Always exit normally from here. */
        YYACCEPT;
    };


statements:

    /* This production precedes input. */
    {
        ECHO("statements:  [null]");

        new_parse();
    }
    | statements command
    {
        ECHO("statements:  statements command");

        yyerrok;
        advance_statement_number();
    }
    | statements expression
    {
        ECHO("statements:  statements expression");

        yyerrok;
        advance_statement_number();
    }
    | statements SEMICOLON
    {
        ECHO("statements:  statements SEMICOLON");

        // Redundant semicolons are tolerated, but aren't recognized as
        // statements.
    }
    | statements error SEMICOLON
    {
        ECHO("statements:  error SEMICOLON");

        // Wait until the end of a statement to report an error (i.e. print
        // only one error message per invalid statement).
        if (handle_error("[this argument is ignored]"))
            END_IT_ALL;

        // Make error reporting resume immediately.
        yyerrok;

        // Clear the lookahead token.
        yyclearin;

        advance_statement_number();
    };


command:

    COMMAND_NAME SEMICOLON
    {
        int exit = 0;

        ECHO("command :  COMMAND SEMICOLON");

        // Handle command with no arguments.
        if (handle_command($1, 0))
            exit = 1;
        free($1);

        if (exit)
            END_IT_ALL
    }
    | COMMAND_NAME arguments SEMICOLON
    {
        int exit = 0;

        ECHO("command :  COMMAND arguments SEMICOLON");

        // Handle command with arguments.
        if ($2)
            if (handle_command($1, (p2_term *) $2))
                exit = 1;
        free($1);

        if (exit)
            END_IT_ALL;
    };


expression:

    term SEMICOLON
    {
        ECHO("expression :  term SEMICOLON");

        // Handle anonymous expression.
        if ($1)
            if (handle_expression(0, (p2_term *) $1))
                END_IT_ALL;
    }
    | term EQUALS STRING SEMICOLON
    {
        int exit = 0;

        ECHO("expression :  term EQUALS token SEMICOLON");

        // Handle named expression.
        if ($1)
            if (handle_expression($3, (p2_term *) $1))
                exit = 1;
        free($3);

        if (exit)
            END_IT_ALL;
    }
    | term EQUALS error
    {
        ECHO("expression :  term EQUALS error");

        if ($1)
        {
            cleanup_term((p2_term *) $1);

            // Note: this error propagates upward to a statement production, and
            // doesn't need to be handled here.
            skip_error();
        }
    }
    | term EQUALS STRING error
    {
        ECHO("expression :  term EQUALS STRING error");

        if ($1)
        {
            cleanup_term((p2_term *) $1);

            // Note: this error propagates upward to a statement production, and
            // doesn't need to be handled here.
            skip_error();
        }

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
        if ($1)
            $$ = p2_term__cat((p2_term *) $1, p2_term__new((void *) $2, 1));
        else
        {
            free($2);
            $$ = 0;
        }
    }
    | arguments error
    {
        // Error is encountered after an argument term has been constructed.
        // Clean up the existing term, and produce a null term as the semantic
        // value.

        ECHO("arguments :  arguments error");

        // Handle the error only if the existing term is good (i.e. if an error
        // is found in an argument list, ignore errors in the rest of the
        // statement).VALGRIND = valgrind -v --tool=memcheck --leak-check=full --show-reachable=yes
        if ($1)
        {
            cleanup_term((p2_term *) $1);
            if (handle_error(0))
                END_IT_ALL;
        }

        $$ = 0;
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

        // If either subterm is null, clean up the existing term and produce a
        // null term as the semantic value.  In this case, the error has already
        // been reported.
        if (!$1 || !$2)
        {
            if ($1)
                cleanup_term($1);
            if ($2)
                cleanup_term($2);
            $$ = 0;
        }

        else
            // Combine the terms using a left-associative merge.
            $$ = p2_term__merge_la((p2_term *) $1, (p2_term *) $2);
    }
    | term error
    {
        // Error is encountered after a term has been constructed.  Clean up the
        // existing term, and produce a null term as the semantic value.

        ECHO("term :  term error");

        // Handle the error only if the existing term is good (i.e. if an error
        // is found in an expression, ignore errors in the rest of the
        // expression).
        if ($1)
        {
            cleanup_term((p2_term *) $1);
            if (handle_error(0))
                END_IT_ALL;
        }

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
    }
    | OPEN_PAREN term error
    {
        ECHO("subterm :  OPEN_PAREN term error");

        cleanup_term((p2_term *) $2);
        if (handle_error(0))
            END_IT_ALL;

        $$ = 0;
    };


%%

// Basic parser functions //////////////////////////////////////////VALGRIND = valgrind -v --tool=memcheck --leak-check=full --show-reachable=yes
////////////


int yywrap()
{
    return 1;
}


/*  main() { yyparse(); }  */


void yyerror(const char *msg)
{
    if (*yyerror_msg)
        fprintf(stderr,
            "Parser warning: last error reported to yyerror was never handled: %s.\n",
            yyerror_msg);

    strcpy(yyerror_msg, msg);

    error_character_number = last_character_number;
    error_line_number = line_number;
    error_statement_number = statement_number;
}


void skip_error()
{
    *yyerror_msg = 0;
}


// Expression handling /////////////////////////////////////////////////////////


int handle_command(char *name, p2_term *args)
{
    int retval;

    if (!suppress_output)
    {
        if (!statement_number)
            printf("\n");

        #ifdef COMMAND_OUTPUT_PREFIX
            printf(COMMAND_OUTPUT_PREFIX);
        #endif
    }

    retval = p2_evaluate_command(name, args);

    if (!suppress_output)
    {
        #ifdef COMMAND_OUTPUT_SUFFIX
            printf(COMMAND_OUTPUT_SUFFIX);
        #endif

        printf("\n\n");
    }

    return retval;
}


int handle_expression(char *name, p2_term *expr)
{
    int retval;

    if (!suppress_output)
    {
        if (!statement_number)
            printf("\n");

        #ifdef EXPRESSION_OUTPUT_PREFIX
            printf(EXPRESSION_OUTPUT_PREFIX);
        #endif
    }

    retval = p2_evaluate_expression(name, expr);

    if (!suppress_output)
    {
        #ifdef EXPRESSION_OUTPUT_SUFFIX
            printf(EXPRESSION_OUTPUT_SUFFIX);
        #endif

        printf("\n\n");
    }

    return retval;
}


int handle_error(char *msg)
{
    int retval;

    if (!suppress_output)
    {
        if (!statement_number)
            printf("\n");

        #ifdef ERROR_OUTPUT_PREFIX
            printf(ERROR_OUTPUT_PREFIX);
        #endif
    }

    // Note: "char" rather than "column" on account of the tab character.
    if (*yyerror_msg)
        sprintf(error_msg, "line %d, char %d: %s",
            error_line_number + 1, error_character_number + 1, yyerror_msg);
    else
        // If this happens, it means that the parser has not recovered from a
        // previous error, or that you've needlessly called handle_error.
        sprintf(error_msg, "unreported");
    *yyerror_msg = '\0';
    retval = p2_handle_parse_error(error_msg);

    if (!suppress_output)
    {
        #ifdef ERROR_OUTPUT_SUFFIX
            printf(ERROR_OUTPUT_SUFFIX);
        #endif

        printf("\n\n");
    }

    return retval;
}


// Debugging and cleanup ///////////////////////////////////////////////////////


void echo_production(char *s)
{
    printf("Found %s\n", s);
}


/** To be passed as a function pointer to p2_term__for_all. */
void *deallocate(void *p)
{
//printf("::::: deallocate\n");
    free(p);
    return (void *) 1;
}


void cleanup_term(p2_term *term)
{
//printf("::::: cleanup_term\n");
    p2_term__for_all(term, deallocate);
    p2_term__delete(term);
}

