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

    _command arg1 arg2;

Commands thus indicated do not belong to the program under construction, and are
to take immediate effect at parse time.

Interaction with the client

The role of the parser is simply to construct a Term to send to the client
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

#include <string.h>  /* strcpy */

#include <parser/Ast.h>
#include "p2_parser.h"

#include <util/Term.h>


/* Avoids C99 warning: implicit declaration of function ‘yylex’ */
int
yylex( void );


/* Language module dependencies ***********************************************/


typedef struct Compiler Compiler;

extern int
compiler__evaluate_command( Compiler *c, char * /*name*/, Ast * /*args*/ );

extern int
compiler__evaluate_expression( Compiler *c, Name * /*name*/, Ast * /*expr*/ );

extern int
compiler__handle_parse_error( Compiler *c, char * /*msg*/ );

extern int
compiler__quiet( Compiler *c );


/* Lexer dependencies *********************************************************/


extern void
new_parse( Compiler *c );

extern int
get_char_number( void );

extern int
get_line_number( void );

/** Current statement number.  Starts at 0 for each line of input. */
extern int pad_newline;


/******************************************************************************/


/** Output decoration. */
/* #define COMMAND_OUTPUT_PREFIX "\t>> "    */
/* #define EXPRESSION_OUTPUT_PREFIX "\t>> " */
/* #define ERROR_OUTPUT_PREFIX "\t>> "      */
/* #define COMMAND_OUTPUT_SUFFIX " <<"      */
/* #define EXPRESSION_OUTPUT_SUFFIX " <<"   */
/* #define ERROR_OUTPUT_SUFFIX " <<"        */


/** Evaluate a command. */
static void
handle_command( Compiler *c, char * /*name*/, Ast * /*args*/ );

/** Evaluate an expression. */
static void
handle_expression( Compiler *c, Name * /*name*/, Ast * /*expr*/ );

/** Deal gracefully with a parse error. */
static void
handle_error( Compiler *c );


/******************************************************************************/


/** Buffer size is arbitary... */
#define ERROR_BUFFER__SIZE  0xFF

static int exit_early;

static int error_character_number, error_line_number;

/** Buffer for "verbose" error message received by yyerror. */
static char yyerror_msg[ERROR_BUFFER__SIZE];


/******************************************************************************/


/** This is the default value. */
#define YYMAXDEPTH    10000

/** This is the default value. */
#define YYINITDEPTH   200


/** Tells the parser to process only a single input stream. */
int
yywrap( void )
{
    return 1;
}


/** Copies reported error messages to a string, where they wait to be passed on to
    the semantic module. */
void
yyerror( Compiler *c, p2_parser__exit_state *ignored, const char *msg )
{
    /* Avoid "unused parameter" compiler warning. */
    c = 0;
    ignored = 0;

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
static void
production( char *s )
{
    printf( "Matched %s\n", s );
}
#endif


#if DEBUG__PARSER__USE_YYDEBUG
#define YYDEBUG  1
#endif


/******************************************************************************/


typedef struct Statement Statement;

struct Statement
{
    Name *name;
    char *simple_name;
    Ast *expr;
};


static Statement *
new_statement( Name *name, char *simple_name, Ast *expr )
{
    Statement *stmt = new( Statement );

    stmt->name = name;
    stmt->simple_name = simple_name;
    stmt->expr = expr;

    return stmt;
}


/******************************************************************************/


static Ast *
term2ast( Term *t )
{
    Ast *ast;

    if ( term__length( t ) > 1 )
        ast = ast__term( t );

    else
    {
        ast = *( t->head + 1 );
        term__delete( t );
    }

    return ast;
}


/******************************************************************************/


%}


%union
{
    char                character;
    double              real;
    int                 integer;
    char                *string;

    /** (void *) instead of (Ast *) because Bison won't take an alias here. */
    void                *parser_node;

    struct Array        *bag, *name;
    struct Statement    *statement;
    struct Term         *term;
}


%token L_PAREN R_PAREN
%token L_SQ_BRACKET R_SQ_BRACKET
%token COMMA L_BRACE R_BRACE
%token COLON E_O_F L_ASSIGN R_ASSIGN SEMICOLON

%token <character>  CHAR
%token <integer>    INT
%token <real>       FLOAT
%token <string>     COMMAND_NAME ID STRING

%type <bag>         bag bag_head
%type <name>        name
%type <parser_node> bracketed_term term_item
%type <statement>   command expression statement
%type <string>      command_name id
%type <term>        command_args subterm term


/*
%pure_parser
*/

%parse-param { Compiler *compiler }
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
            handle_error( compiler );

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

        new_parse( compiler );

        ERROK;
    }

    | statements statement
    {
        #if DEBUG__PARSER
        production( "statements ::=  statements statement" );
        #endif

        if ( *yyerror_msg )
            handle_error( compiler );

        if ( exit_early )
        {
            *return_state = exit_state__aborted;
            YYACCEPT;
        }

        pad_newline = TRUE;
    }

    /* Redundant semicolons are tolerated. */
    | statements SEMICOLON
    {
        #if DEBUG__PARSER
        production( "statements ::=  statements SEMICOLON" );
        #endif

        if ( *yyerror_msg )
            handle_error( compiler );

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

        if ( *yyerror_msg )
            handle_error( compiler );

        if ( exit_early )
        {
            *return_state = exit_state__aborted;
            YYACCEPT;
        }

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
            if ( $1->simple_name )
                handle_command( compiler, $1->simple_name, $1->expr );

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
                handle_expression( compiler, $1->name, $1->expr );

            free( $1 );
        }
    };


command:

    /* Command with no command_args. */
    command_name
    {
        #if DEBUG__PARSER
        production( "command ::=  command_name" );
        #endif

        $$ = new_statement( 0, $1, 0 );
    }

    | command_name error
    {
        #if DEBUG__PARSER
        production( "command ::=  command_name error" );
        #endif

        if ( $1 )
            free( $1 );

        $$ = 0;

        ERROK;
    }

    /* Command with command_args. */
    | command_name command_args
    {
        #if DEBUG__PARSER
        production( "command ::=  command_name command_args" );
        #endif

        if ( $2 )
            $$ = new_statement( 0, $1, ast__term( $2 ) );
    }

    | command_name command_args error
    {
        #if DEBUG__PARSER
        production( "command ::=  command_name command_args error" );
        #endif

        if ( $1 )
            free( $1 );

        if ( $2 )
            ast__delete( ast__term( $2 ) );

        $$ = 0;

        ERROK;
    };


/* Trivial rule prevents memory leaks when non-grammatical COMMAND_NAME is
   matched by the lexer. */
command_name:

    COMMAND_NAME
    {
        #if DEBUG__PARSER
        production( "command_name ::=  COMMAND_NAME" );
        #endif

        $$ = STRDUP( $1 );
    };


command_args:

    name
    {
        #if DEBUG__PARSER
        production( "command_args ::=  name" );
        #endif

        /* Create a singleton term containing one argument. */
        $$ = term__new( ast__name( $1 ), 1 );
    }

    | command_args name
    {
        #if DEBUG__PARSER
        production( "command_args ::=  command_args name" );
        #endif

        /* Concatenate the command command_args. */
        if ( $1 )
        {
            $$ = term__cat( $1, term__new( ast__name( $2 ), 1 ) );
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
            $$ = new_statement( 0, 0, term2ast( $1 ) );
        else
            $$ = 0;
    }

    | term error
    {
        #if DEBUG__PARSER
        production( "expression ::=  term error" );
        #endif

        if ( $1 )
            ast__delete( ast__term( $1 ) );

        $$ = 0;

        ERROK;
    }

    /* Left assignment from expression. */
    | name L_ASSIGN term
    {
        #if DEBUG__PARSER
        production( "expression ::=  name L_ASSIGN term" );
        #endif

        if ( $1 && $3 )
            $$ = new_statement( $1, 0, term2ast( $3 ) );

        else
        {
            $$ = 0;

            if ( $1 )
                ast__delete( ast__name( $1 ) );

            if ( $3 )
                ast__delete( ast__term( $3 ) );
        }
    }

    | name L_ASSIGN error
    {
        #if DEBUG__PARSER
        production( "expression ::=  name L_ASSIGN error" );
        #endif

        $$ = 0;

        if ( $1 )
            ast__delete( ast__name( $1 ) );

        ERROK;
    }

    | name L_ASSIGN term error
    {
        #if DEBUG__PARSER
        production( "expression ::=  name L_ASSIGN term error" );
        #endif

        $$ = 0;

        if ( $1 )
            ast__delete( ast__name( $1 ) );

        if ( $3 )
            ast__delete( ast__term( $3 ) );

        ERROK;
    }

    /* Right assignment from expression. */
    | term R_ASSIGN name
    {
        #if DEBUG__PARSER
        production( "expression ::=  term R_ASSIGN name" );
        #endif

        if ( $1 && $3 )
                                /* !      */
            $$ = new_statement( $3, 0, term2ast( $1 ) );

        else
        {
            $$ = 0;

            if ( $1 )
                ast__delete( ast__term( $1 ) );

            if ( $3 )
                ast__delete( ast__name( $3 ) );
        }
    }

    /* Named expression. */
    | term R_ASSIGN name error
    {
        #if DEBUG__PARSER
        production( "expression ::=  term R_ASSIGN name error" );
        #endif

        $$ = 0;

        if ( $1 )
            ast__delete( ast__term( $1 ) );

        if ( $3 )
            ast__delete( ast__name( $3 ) );

        ERROK;
    }

    | term R_ASSIGN error
    {
        #if DEBUG__PARSER
        production( "expression ::=  term R_ASSIGN error" );
        #endif

        $$ = 0;

        if ( $1 )
            ast__delete( ast__term( $1 ) );

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
            $$ = term__merge_la( $1, $2 );

        else
        {
            $$ = 0;

            if ( $1 )
                ast__delete( ast__term( $1 ) );
            if ( $2 )
                ast__delete( ast__term( $2 ) );
        }
    };


subterm:

    term_item
    {
        #if DEBUG__PARSER
        production( "subterm ::=  term_item" );
        #endif

        if ( $1 )
            $$ = term__new( $1, 0 );

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
            ast__delete( ast__term( $2 ) );

        ERROK;
    };


term_item:

    CHAR
    {
        #if DEBUG__PARSER
        production( "term_item ::=  CHAR" );
        #endif

        $$ = ast__char( $1 );
    }

    | FLOAT
    {
        #if DEBUG__PARSER
        production( "term_item ::=  FLOAT" );
        #endif

        $$ = ast__float( $1 );
    }

    | INT
    {
        #if DEBUG__PARSER
        production( "term_item ::=  INT" );
        #endif

        $$ = ast__int( $1 );
    }

    | STRING
    {
        #if DEBUG__PARSER
        production( "term_item ::=  STRING" );
        #endif

        $$ = ast__string( STRDUP( $1 ) );
    }

    | bag
    {
        #if DEBUG__PARSER
        production( "term_item ::=  bag" );
        #endif

        if ( $1 )
            $$ = ast__bag( $1 );

        else
            $$ = 0;
    }

    | name
    {
        #if DEBUG__PARSER
        production( "term_item ::=  name" );
        #endif

        if ( $1 )
            $$ = ast__name( $1 );

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
            $$ = ( void* ) ast__term( $2 );

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
            ast__delete( ast__term( $2 ) );

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
            ast__delete( ast__bag( $1 ) );

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
            $$ = ( void* ) array__new( 1, 0 );
            array__enqueue( $$, term2ast( $2 ) );
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
            array__enqueue( $$, term2ast( $3 ) );
        }

        else
        {
            $$ = 0;

            if ( $1 )
                ast__delete( ast__bag( $1 ) );
            if ( $3 )
                ast__delete( ast__term( $3 ) );
        }
    }

    | bag_head COMMA error
    {
        #if DEBUG__PARSER
        production( "bag_head ::=  bag_head COMMA error" );
        #endif

        $$ = 0;

        if ( $1 )
            ast__delete( ast__bag( $1 ) );

        ERROK;
    };


name:

    id
    {
        #if DEBUG__PARSER
        production( "name ::=  ID" );
        #endif

        if ( $1 )
        {
            $$ = array__new( 1, 0 );
            array__enqueue( $$, $1 );
        }

        else
            $$ = 0;
    }

    | name COLON id
    {
        #if DEBUG__PARSER
        production( "name ::=  name COLON ID" );
        #endif

        if ( $1 && $3 )
        {
            $$ = $1;
            array__enqueue( $$, $3 );
        }

        else
        {
            if ( $1 )
                ast__delete( ast__name( $1 ) );

            if ( $3 )
                free( $3 );

            $$ = 0;
        }
    }

    | name COLON error
    {
        #if DEBUG__PARSER
        production( "name ::=  name COLON error" );
        #endif

        $$ = 0;
        ast__delete( ast__name( $1 ) );

        ERROK;
    };


/* Trivial rule prevents memory leaks when non-grammatical ID is matched by
   the lexer. */
id:

    ID
    {
        #if DEBUG__PARSER
        production( "id ::=  ID" );
        #endif

        $$ = STRDUP( $1 );
    };


%%


static void
handle_command( Compiler *c, char *name, Ast *args )
{
    #if DEBUG__PARSER
    printf( "[] handle_command(%#x, %#x, %#x)\n",
        ( int ) c, ( int ) name, ( int ) args );
    #endif

    if ( !compiler__quiet( c ) )
    {
        printf( "\n" );

        #ifdef COMMAND_OUTPUT_PREFIX
        printf( COMMAND_OUTPUT_PREFIX );
        #endif
    }

    /* Note: ownership of name and arguments is conferred to
       compiler__evaluate_command. */
    exit_early = exit_early || compiler__evaluate_command( c, name, args );

    if ( !compiler__quiet( c ) )
    {
        #ifdef COMMAND_OUTPUT_SUFFIX
        printf( COMMAND_OUTPUT_SUFFIX );
        #endif
    }
}


static void
handle_expression( Compiler *c, Name *name, Ast *expr )
{
    #if DEBUG__PARSER
    printf( "[] handle_expression(%#x, %#x, %#x)\n",
        ( int ) c, ( int ) name, ( int ) expr );
    #endif

    if ( !compiler__quiet( c ) )
    {
        printf( "\n" );

        #ifdef EXPRESSION_OUTPUT_PREFIX
        printf( EXPRESSION_OUTPUT_PREFIX );
        #endif
    }

    /* Note: ownership of name and expression is conferred to
       compiler__evaluate_expression. */
    exit_early = exit_early || compiler__evaluate_expression( c, name, expr );

    if ( !compiler__quiet( c ) )
    {
        #ifdef EXPRESSION_OUTPUT_SUFFIX
        printf( EXPRESSION_OUTPUT_SUFFIX );
        #endif
    }
}


static void
handle_error( Compiler *c )
{
    char error_msg[ ERROR_BUFFER__SIZE + 0x20 ];

    #if DEBUG__PARSER
    printf( "[] handle_error(%#x)\n", ( int ) c );
    #endif

    if ( !compiler__quiet( c ) )
    {
        printf( "\n" );

        #ifdef ERROR_OUTPUT_PREFIX
        printf( ERROR_OUTPUT_PREFIX );
        #endif
    }

    sprintf( error_msg, "line %d, column %d: %s",
        error_line_number, error_character_number, yyerror_msg );

    *yyerror_msg = '\0';
    exit_early = exit_early || compiler__handle_parse_error( c, STRDUP( error_msg ) );

    if ( !compiler__quiet( c ) )
    {
        #ifdef ERROR_OUTPUT_SUFFIX
        printf( ERROR_OUTPUT_SUFFIX );
        #endif
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
