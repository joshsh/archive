%{

/**

\file  p2_parser.y

\brief  Bison grammar file for Phase2's command line front-end.

Phase2's command-line interface is a separable feature of the language. It
allows the user to pass command_args to either of a pair of functions defined in
the Phase2 client: one for program construction and one for querying or
manipulating the programming environment.

Expression Syntax

At the parser level, an expression is a parenthetically nested sequence of
dictionary items or special symbols, terminated by a semicolon. To give an
expression a name, the semicolon may be preceded by an equality symbol and
then the name, e.g.

    (token1 token2) token3 = token4;

Command Syntax

Special commands are indicated with a backslash plus the name of the command,
followed by a whitespace-delimited list of command_args (no parentheses) and
terminated by a semicolon, e.g.

    \command arg1 arg2;

Commands thus indicated do not belong to the program under construction, and are
to take immediate effect at parse time.

Interaction with the client

The role of the parser is simply to construct a p2_term to send to the client
for evaluation.  The client then handles the term and eventually frees it
after generating output.  The client cannot "talk back" to the parser except to
tell it to terminate.

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
#include "p2_parser.h"
#include "../p2_flags.h"

#include "../util/p2_term.h"

#include <stdio.h>  /* printf, sprintf */
#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */

/* Avoid C99 warning: implicit declaration of function ‘yylex’ */
int yylex( void );


/* Language module dependencies ***********************************************/


extern int p2_compiler__evaluate_command( char *name, p2_ast *args );
extern int p2_compiler__evaluate_expression( p2_name *name, p2_ast *expr );
extern int p2_compiler__handle_parse_error( char *msg );

extern int p2_compiler__suppress_output();


/* Lexer dependencies *********************************************************/


extern void new_parse();
extern int get_char_number();
extern int get_line_number();

/** Current statement number.  Starts at 0 for each line of input. */
extern int statement_number;


/******************************************************************************/


/** Output decoration. */
/* #define COMMAND_OUTPUT_PREFIX "\t>> "    */
/* #define EXPRESSION_OUTPUT_PREFIX "\t>> " */
/* #define ERROR_OUTPUT_PREFIX "\t>> "      */
/* #define COMMAND_OUTPUT_SUFFIX " <<"      */
/* #define EXPRESSION_OUTPUT_SUFFIX " <<"   */
/* #define ERROR_OUTPUT_SUFFIX " <<"        */


/** Evaluate a command. */
void handle_command( char *name, p2_ast *args );

/** Evaluate an expression. */
void handle_expression( p2_name *name, p2_ast *expr );

/** Deal gracefully with a parse error. */
void handle_error();


/******************************************************************************/


/** Buffer size is arbitary... */
#define ERROR_BUFFER__SIZE  0xFF

int exit_early;

int error_character_number, error_line_number, invalid_statement_number;

/** Buffer for "verbose" error message received by yyerror. */
char yyerror_msg[ ERROR_BUFFER__SIZE ];


/******************************************************************************/


/** This is the default value. */
#define YYMAXDEPTH    10000

/** This is the default value. */
#define YYINITDEPTH   200


/** Tells the parser to process only a single input stream. */
int yywrap()
{
    return 1;
}


/** Copies reported error messages to a string, where they wait to be passed on to
    the semantic module. */
void yyerror( p2_parser__exit_state *ignored, const char *msg )
{
    /* Only the first error in a statement is reported. */
    if ( ! *yyerror_msg )
    {
        #if DEBUG__PARSER
        printf( "yyerror: %s (reported)\n", msg );
        #endif

        if ( strlen( msg ) >= ERROR_BUFFER__SIZE )
            strcpy( yyerror_msg, "[parser error message overflows buffer]" );
        else
            strcpy( yyerror_msg, msg );

        error_character_number = get_char_number();
        error_line_number = get_line_number();
        invalid_statement_number = statement_number;
    }

    else
    {
        #if DEBUG__PARSER
        printf( "yyerror: %s (not reported due to previous error)\n", msg );
        #endif
    }
}


#define ERROK  { yyerrok; exit_early = 0; }


#if DEBUG__PARSER
/** Echo each production as it is matched by the parser. */
void production( char *s )
{
    printf( "Matched %s\n", s );
}
/*#define PRODUCTION( x )  printf( "Matched %s\n", x );
#else
#define PRODUCTION*/
#endif


#if DEBUG__PARSER__USE_YYDEBUG
#define YYDEBUG  1
#endif


/******************************************************************************/


struct statement
{
    char *name;
    p2_ast *expr;
};


struct statement *new_statement( char *name, p2_ast *expr )
{
    struct statement *stmt = ( struct statement* ) malloc( sizeof( struct statement ) );

    stmt->name = name;
    stmt->expr = expr;

    return stmt;
}


/******************************************************************************/


static p2_ast *term2ast( p2_term *t )
{
    p2_ast *ast;

    if ( p2_term__length( t ) > 1 )
        ast = p2_ast__term( t );

    else
    {
        ast = ( p2_ast* ) *( t->head + 1 );
        p2_term__delete( t );
    }

    return ast;
}


/******************************************************************************/


%}


%union
{
    char *string_t;

    int int_t;
    double float_t;
    char char_t;

    /** (void *) instead of (p2_term *), (p2_ast *) (p2_array*) because
        Bison won't take an alias here. */
    void *term, *name, *bag, *parser_node;

    struct statement *stmt;
}


%token L_PAREN R_PAREN
%token L_SQ_BRACKET R_SQ_BRACKET;
%token L_BRACE COMMA R_BRACE
%token COLON EQUALS SEMICOLON E_O_F

%token <string_t> ID STRING COMMAND_NAME
%token <char_t> CHAR
%token <float_t> FLOAT;
%token <int_t> INT;

%type <term> term subterm command_args
%type <parser_node> term_item bracketed_term
%type <bag> bag bag_head
%type <name> name
%type <stmt> statement command expression


/*
%pure_parser
*/

%parse-param { p2_parser__exit_state *return_state }

/** Report more detailed parse error messages. */
%error-verbose

%start input


%%


input:

    statements E_O_F
    {
        #if DEBUG__PARSER
        production( "input ::=  statements E_O_F" );
        #endif

        if ( *yyerror_msg )
            handle_error();

        if ( exit_early )
            *return_state = exit_state__aborted;

        else
            *return_state = exit_state__end_of_input;

        /* Exit normally. */
        YYACCEPT;
    };


statements:

    /* This production precedes input. */
    {
        #if DEBUG__PARSER
        production( "statements ::=  [null]" );
        #endif

        *return_state = exit_state__parse_failure;

        #if YYDEBUG
        yydebug = 1;
        #endif

        new_parse();

        ERROK;
    }

    | statements statement
    {
        #if DEBUG__PARSER
        production( "statements ::=  statements statement" );
        #endif

        if ( *yyerror_msg )
            handle_error();

        if ( exit_early )
        {
            *return_state = exit_state__aborted;
            YYACCEPT;
        }

        statement_number++;
    }

    /* Redundant semicolons are tolerated. */
    | statements SEMICOLON
    {
        #if DEBUG__PARSER
        production( "statements ::=  statements SEMICOLON" );
        #endif

        if ( *yyerror_msg )
            handle_error();

        if ( exit_early )
        {
            *return_state = exit_state__aborted;
            YYACCEPT;
        }
    }

    | statements error SEMICOLON
    {
        #if DEBUG__PARSER
        production( "statements ::=  statements error SEMICOLON" );
        #endif

        ERROK;
    };


statement:

    /* Wait for the semicolon before executing a command. */
    command SEMICOLON
    {
        #if DEBUG__PARSER
        production( "statement ::=  command SEMICOLON" );
        #endif

        $$ = 0;

        if ( $1 )
        {
            if ( $1->name )
                handle_command( $1->name, $1->expr );

            free( $1 );
        }
    }

    /* Wait for the semicolon before evaluating an expression. */
    | expression SEMICOLON
    {
        #if DEBUG__PARSER
        production( "statement ::=  expression SEMICOLON" );
        #endif

        $$ = 0;

        if ( $1 )
        {
            if ( $1->expr )
                                   /* !      */
                handle_expression( ( p2_name* ) $1->name, $1->expr );

            free( $1 );
        }
    };


command:

    /* Command with no command_args. */
    COMMAND_NAME
    {
        #if DEBUG__PARSER
        production( "command ::=  COMMAND_NAME" );
        #endif

        if ( $1 )
            $$ = new_statement( $1, 0 );
        else
            $$ = 0;
    }

    /* Command with command_args. */
    | COMMAND_NAME command_args
    {
        #if DEBUG__PARSER
        production( "command ::=  COMMAND_NAME command_args" );
        #endif

        if ( $1 )
            $$ = new_statement( $1, p2_ast__term( ( p2_term* ) $2 ) );
        else
            $$ = 0;
    };


command_args:

    name
    {
        #if DEBUG__PARSER
        production( "command_args ::=  name" );
        #endif

        /* Create a singleton term containing one argument. */
        $$ = p2_term__new( ( void* ) p2_ast__name( ( p2_array* ) $1 ), 1 );
    }

    | command_args name
    {
        #if DEBUG__PARSER
        production( "command_args ::=  command_args name" );
        #endif

        /* Concatenate the command command_args. */
        if ( $1 )
        {
            $$ = p2_term__cat(
                ( p2_term* ) $1,
                p2_term__new( ( void* ) p2_ast__name( ( p2_array* ) $2 ), 1 ) );
        }

        else
        {
            $$ = 0;

            free( $2 );
        }
    };


expression:

    /* Anonymous expression. */
    term
    {
        #if DEBUG__PARSER
        production( "expression ::=  term" );
        #endif

        if ( $1 )
            $$ = new_statement( 0, term2ast( ( p2_term* ) $1 ) );
        else
            $$ = 0;
    }

    /* Named expression. */
    | term EQUALS name
    {
        #if DEBUG__PARSER
        production( "expression ::=  term EQUALS name" );
        #endif

        if ( $1 )
                                /* !      */
            $$ = new_statement( ( char* ) $3, term2ast( ( p2_term* ) $1 ) );
        else
            $$ = 0;
    }

    | term EQUALS error
    {
        #if DEBUG__PARSER
        production( "expression ::=  term EQUALS error" );
        #endif

        $$ = 0;

        if ( $1 )
            p2_ast__delete( p2_ast__term( ( p2_term* ) $1 ) );

        ERROK;
    };


term:

    subterm
    {
        #if DEBUG__PARSER
        production( "term ::=  subterm" );
        #endif

        $$ = $1;
    }

    | term subterm
    {
        #if DEBUG__PARSER
        production( "term ::=  term subterm" );
        #endif

        if ( $1 && $2 )
            /* Combine the terms using a left-associative merge. */
            $$ = p2_term__merge_la( ( p2_term* ) $1,  ( p2_term* ) $2 );

        else
        {
            $$ = 0;

            if ( $1 )
                p2_ast__delete( p2_ast__term( ( p2_term* ) $1 ) );
            if ( $2 )
                p2_ast__delete( p2_ast__term( ( p2_term* ) $2 ) );
        }
    };


subterm:

    term_item
    {
        #if DEBUG__PARSER
        production( "subterm ::=  term_item" );
        #endif

        if ( $1 )
            $$ = ( void* ) p2_term__new( $1, 0 );

        else
            $$ = 0;
    }

    | L_PAREN term R_PAREN
    {
        #if DEBUG__PARSER
        production( "subterm ::=  L_PAREN term R_PAREN" );
        #endif

        if ( $2 )
            /* "Remove the parentheses" from the term. */
            $$ = $2;

        else
            $$ = 0;
    }

    | L_PAREN error
    {
        #if DEBUG__PARSER
        production( "subterm ::=  L_PAREN error" );
        #endif

        $$ = 0;

        ERROK;
    }

    | L_PAREN term error
    {
        #if DEBUG__PARSER
        production( "subterm ::=  L_PAREN term error" );
        #endif

        $$ = 0;

        if ( $2 )
            p2_ast__delete( p2_ast__term( ( p2_term* ) $2 ) );

        ERROK;
    };


term_item:

    CHAR
    {
        #if DEBUG__PARSER
        production( "term_item ::=  CHAR" );
        #endif

        $$ = p2_ast__char( $1 );
    }

    | FLOAT
    {
        #if DEBUG__PARSER
        production( "term_item ::=  FLOAT" );
        #endif

        $$ = p2_ast__float( $1 );
    }

    | INT
    {
        #if DEBUG__PARSER
        production( "term_item ::=  INT" );
        #endif

        $$ = p2_ast__int( $1 );
    }

    | STRING
    {
        #if DEBUG__PARSER
        production( "term_item ::=  STRING" );
        #endif

        $$ = p2_ast__string( $1 );
    }

    | bag
    {
        #if DEBUG__PARSER
        production( "term_item ::=  bag" );
        #endif

        if ( $1 )
            $$ = ( void* ) p2_ast__bag( ( p2_array* ) $1 );

        else
            $$ = 0;
    }

    | name
    {
        #if DEBUG__PARSER
        production( "term_item ::=  name" );
        #endif

        if ( $1 )
            $$ = ( void* ) p2_ast__name( ( p2_array* ) $1 );

        else
            $$ = 0;
    }

    | bracketed_term
    {
        #if DEBUG__PARSER
        production( "term_item ::=  bracketed_term" );
        #endif

        $$ = $1;
    };


bracketed_term:

    L_SQ_BRACKET term R_SQ_BRACKET
    {
        #if DEBUG__PARSER
        production( "bracketed_term ::=  L_SQ_BRACKET term R_SQ_BRACKET" );
        #endif

        if ( $2 )
            $$ = ( void* ) p2_ast__term( ( p2_term* ) $2 );

        else
            $$ = 0;
    }

    | L_SQ_BRACKET error
    {
        #if DEBUG__PARSER
        production( "bracketed_term ::=  L_SQ_BRACKET error" );
        #endif

        $$ = 0;

        ERROK;
    }

    | L_SQ_BRACKET term error
    {
        #if DEBUG__PARSER
        production( "bracketed_term ::=  L_SQ_BRACKET term error" );
        #endif

        $$ = 0;

        if ( $2 )
            p2_ast__delete( p2_ast__term( ( p2_term* ) $2 ) );

        ERROK;
    };


bag:

    bag_head R_BRACE
    {
        #if DEBUG__PARSER
        production( "bag ::=  bag_head R_BRACE" );
        #endif

        if ( $1 )
            $$ = $1;

        else
            $$ = 0;
    }

    | bag_head error
    {
        #if DEBUG__PARSER
        production( "bag ::=  bag_head error" );
        #endif

        $$ = 0;

        if ( $1 )
            p2_ast__delete( p2_ast__bag( ( p2_array* ) $1 ) );

        ERROK;
    };


bag_head:

    L_BRACE term
    {
        #if DEBUG__PARSER
        production( "bag_head ::=  L_BRACE term" );
        #endif

        if ( $2 )
        {
            $$ = ( void* ) p2_array__new( 1, 0 );
            p2_array__enqueue( ( p2_array* ) $$, term2ast( ( p2_term* ) $2 ) );
        }

        else
            $$ = 0;
    }

    | L_BRACE error
    {
        #if DEBUG__PARSER
        production( "bag_head ::=  L_BRACE error" );
        #endif

        $$ = 0;

        ERROK;
    }

    | bag_head COMMA term
    {
        #if DEBUG__PARSER
        production( "bag_head ::=  bag_head COMMA term" );
        #endif

        if ( $1 && $3 )
        {
            $$ = $1;
            p2_array__enqueue( ( p2_array* ) $$, term2ast( ( p2_term* ) $3 ) );
        }

        else
        {
            $$ = 0;

            if ( $1 )
                p2_ast__delete( p2_ast__bag( ( p2_array* ) $1 ) );
            if ( $3 )
                p2_ast__delete( ( p2_ast* ) $3 );
        }
    }

    | bag_head COMMA error
    {
        #if DEBUG__PARSER
        production( "bag_head ::=  bag_head COMMA error" );
        #endif

        $$ = 0;

        if ( $1 )
            p2_ast__delete( p2_ast__bag( ( p2_array* ) $1 ) );

        ERROK;
    };


name:

    ID
    {
        #if DEBUG__PARSER
        production( "name ::=  ID" );
        #endif

        $$ = ( void* ) p2_array__new( 1, 0 );
        p2_array__enqueue( ( p2_array* ) $$, $1 );
    }

    | name COLON ID
    {
        #if DEBUG__PARSER
        production( "name ::=  name COLON ID" );
        #endif

        if ( $1 )
        {
            $$ = $1;
            p2_array__enqueue( ( p2_array* ) $$, ( void* ) $3 );
        }

        else
        {
            $$ = 0;
            free( $3 );
        }
    }

    | name COLON error
    {
        #if DEBUG__PARSER
        production( "name ::=  name COLON error" );
        #endif

        $$ = 0;
        p2_ast__delete( p2_ast__name( ( p2_array* ) $1 ) );

        ERROK;
    };


%%


/*  main() { yyparse(); }  */


void handle_command( char *name, p2_ast *args )
{
    if ( !p2_compiler__suppress_output() )
    {
        if ( !statement_number )
            printf( "\n" );

        #ifdef COMMAND_OUTPUT_PREFIX
        printf( COMMAND_OUTPUT_PREFIX );
        #endif
    }

    /* Note: ownership of name and arguments are conferred to
       p2_compiler__evaluate_command. */
    exit_early = exit_early || p2_compiler__evaluate_command( name, args );

    if ( !p2_compiler__suppress_output() )
    {
        #ifdef COMMAND_OUTPUT_SUFFIX
        printf( COMMAND_OUTPUT_SUFFIX );
        #endif

        printf( "\n\n" );
    }
}


void handle_expression( p2_name *name, p2_ast *expr )
{
    if ( !p2_compiler__suppress_output() )
    {
        if ( !statement_number )
            printf( "\n" );

        #ifdef EXPRESSION_OUTPUT_PREFIX
        printf( EXPRESSION_OUTPUT_PREFIX );
        #endif
    }

    /* Note: ownership of name and expression are conferred to
       p2_compiler__evaluate_expression. */
    exit_early = exit_early || p2_compiler__evaluate_expression( name, expr );

    if (!p2_compiler__suppress_output())
    {
        #ifdef EXPRESSION_OUTPUT_SUFFIX
        printf( EXPRESSION_OUTPUT_SUFFIX );
        #endif

        printf( "\n\n" );
    }
}


void handle_error()
{
    char error_msg[ ERROR_BUFFER__SIZE + 0x20 ];

    if ( !p2_compiler__suppress_output() )
    {
        if ( !statement_number )
            printf( "\n" );

        #ifdef ERROR_OUTPUT_PREFIX
        printf( ERROR_OUTPUT_PREFIX );
        #endif
    }

    /* Note: "char" rather than "column" on account of the tab character. */
    sprintf( error_msg, "line %d, char %d: %s",
        error_line_number, error_character_number, yyerror_msg );

    *yyerror_msg = '\0';
    exit_early = exit_early || p2_compiler__handle_parse_error( STRDUP( error_msg ) );

    if ( !p2_compiler__suppress_output() )
    {
        #ifdef ERROR_OUTPUT_SUFFIX
        printf( ERROR_OUTPUT_SUFFIX );
        #endif

        printf( "\n\n" );
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
