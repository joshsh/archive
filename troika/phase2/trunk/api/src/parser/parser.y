%{

/**

\file  parser.y

\brief  Bison grammar file for Phase2's command line interpreter.

The role of the parser is simply to construct an expression to send to the
interpreter for evaluation.  The interpreter then handles the expression and
eventually frees it after generating output.  The interpreter cannot "talk back"
to the parser except to tell it to quit.

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

#include <string.h>  /* strcpy */

#include <common.h>
#include <collection/Term.h>
#include <Ast.h>

#include "../settings.h"


/* Avoids C99 warning: implicit declaration of function ‘yylex’ */
int
yylex( void );


/* Language module dependencies ***********************************************/


typedef struct Interpreter Interpreter;

extern int
interpreter__evaluate_command( Interpreter *c, char * /*name*/, Ast * /*args*/, const char * /*text*/ );

extern int
interpreter__evaluate_expression( Interpreter *c, Name * /*name*/, Ast * /*expr*/, const char * /*text*/ );

extern int
interpreter__handle_parse_error( Interpreter *c, char * /*msg*/ );

extern int
interpreter__quiet( Interpreter *c );


/* Lexer dependencies *********************************************************/


extern boolean pad_newline;

extern void
new_parse( Interpreter *c );

extern int
get_char_number( void );

extern int
get_line_number( void );


/******************************************************************************/


/** Output decoration. */
/* #define TOK__COMMAND_OUTPUT_PREFIX "\t>> "    */
/* #define EXPRESSION_OUTPUT_PREFIX "\t>> " */
/* #define ERROR_OUTPUT_PREFIX "\t>> "      */
/* #define TOK__COMMAND_OUTPUT_SUFFIX " <<"      */
/* #define EXPRESSION_OUTPUT_SUFFIX " <<"   */
/* #define ERROR_OUTPUT_SUFFIX " <<"        */


/** Evaluate a command. */
static void
handle_command( Interpreter *c, char * /*name*/, Ast * /*args*/ );

/** Evaluate an expression. */
static void
handle_expression( Interpreter *c, Name * /*name*/, Ast * /*expr*/ );

/** Deal gracefully with a parse error. */
static void
handle_error( Interpreter *c );


/******************************************************************************/


/** Buffer size is arbitary... */
#define ERROR_BUFFER__SIZE  0xFF

static int exit_early;

static int error_character_number, error_line_number;

/** Buffer for "verbose" error message received by yyerror. */
static char yyerror_msg[ERROR_BUFFER__SIZE];

extern void
lexer__clear_buffer();

extern char *
lexer__get_buffer();


/******************************************************************************/


void
parser__push( const char *s )
{
    yy_scan_string( s );
}


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
yyerror( Interpreter *c, exit_state *ignored, const char *msg )
{
    /* Avoid "unused parameter" compiler warning. */
    c = 0;
    ignored = 0;

    /* Only the first error in a statement is reported. */
    if ( ! *yyerror_msg )
    {
#if DEBUG__PARSER
        PRINT( "yyerror: %s (reported)\n", msg );
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
        PRINT( "yyerror: %s (not reported due to previous error)\n", msg );
#endif
    }
}


#define ERROK  { yyerrok; exit_early = 0; }


#if DEBUG__PARSER
/** Echo each production as it is matched by the parser. */
static void
production( char *s )
{
    PRINT( "Matched %s\n", s );
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
    Statement *stmt = NEW( Statement );

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
        ast = term__head( t );
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


%token TOK__L_PAREN         TOK__R_PAREN
%token TOK__L_SQ_BRACKET    TOK__R_SQ_BRACKET
%token TOK__L_BRACE         TOK__R_BRACE
%token TOK__L_ASSIGN        TOK__R_ASSIGN
%token TOK__COMMA           TOK__COLON          TOK__SEMICOLON
%token TOK__EOF

%token <character>  TOK__CHAR
%token <integer>    TOK__INT
%token <real>       TOK__REAL
%token <string>     TOK__COMMAND_NAME   TOK__ID TOK__STRING

%type <bag>         bag bag_head
%type <name>        name
%type <parser_node> bracketed_term term_item
%type <statement>   command expression statement
%type <string>      command_name id
%type <term>        command_args subterm term


/*
%pure_parser
*/

%parse-param { Interpreter *compiler }
%parse-param { exit_state *return_state }

/** Report more detailed parse error messages. */
%error-verbose

%start input


%%


input:

    statements TOK__EOF
    {
#if DEBUG__PARSER
        production( "input ::=  statements TOK__EOF" );
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
    | statements TOK__SEMICOLON
    {
#if DEBUG__PARSER
        production( "statements ::=  statements TOK__SEMICOLON" );
#endif

        if ( *yyerror_msg )
            handle_error( compiler );

        if ( exit_early )
        {
            *return_state = exit_state__aborted;
            YYACCEPT;
        }
    }

    | statements error TOK__SEMICOLON
    {
#if DEBUG__PARSER
        production( "statements ::=  statements error TOK__SEMICOLON" );
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
    command TOK__SEMICOLON
    {
#if DEBUG__PARSER
        production( "statement ::=  command TOK__SEMICOLON" );
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
    | expression TOK__SEMICOLON
    {
#if DEBUG__PARSER
        production( "statement ::=  expression TOK__SEMICOLON" );
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


/* Trivial rule prevents memory leaks when non-grammatical TOK__COMMAND_NAME is
   matched by the lexer. */
command_name:

    TOK__COMMAND_NAME
    {
#if DEBUG__PARSER
        production( "command_name ::=  TOK__COMMAND_NAME" );
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
    | name TOK__L_ASSIGN term
    {
#if DEBUG__PARSER
        production( "expression ::=  name TOK__L_ASSIGN term" );
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

    | name TOK__L_ASSIGN error
    {
#if DEBUG__PARSER
        production( "expression ::=  name TOK__L_ASSIGN error" );
#endif

        $$ = 0;

        if ( $1 )
            ast__delete( ast__name( $1 ) );

        ERROK;
    }

    | name TOK__L_ASSIGN term error
    {
#if DEBUG__PARSER
        production( "expression ::=  name TOK__L_ASSIGN term error" );
#endif

        $$ = 0;

        if ( $1 )
            ast__delete( ast__name( $1 ) );

        if ( $3 )
            ast__delete( ast__term( $3 ) );

        ERROK;
    }

    /* Right assignment from expression. */
    | term TOK__R_ASSIGN name
    {
#if DEBUG__PARSER
        production( "expression ::=  term TOK__R_ASSIGN name" );
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
    | term TOK__R_ASSIGN name error
    {
#if DEBUG__PARSER
        production( "expression ::=  term TOK__R_ASSIGN name error" );
#endif

        $$ = 0;

        if ( $1 )
            ast__delete( ast__term( $1 ) );

        if ( $3 )
            ast__delete( ast__name( $3 ) );

        ERROK;
    }

    | term TOK__R_ASSIGN error
    {
#if DEBUG__PARSER
        production( "expression ::=  term TOK__R_ASSIGN error" );
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

    | TOK__L_PAREN term TOK__R_PAREN
    {
#if DEBUG__PARSER
        production( "subterm ::=  TOK__L_PAREN term TOK__R_PAREN" );
#endif

        if ( $2 )
            /* "Remove the parentheses" from the term. */
            $$ = $2;

        else
            $$ = 0;
    }

    | TOK__L_PAREN error
    {
#if DEBUG__PARSER
        production( "subterm ::=  TOK__L_PAREN error" );
#endif

        $$ = 0;

        ERROK;
    }

    | TOK__L_PAREN term error
    {
#if DEBUG__PARSER
        production( "subterm ::=  TOK__L_PAREN term error" );
#endif

        $$ = 0;

        if ( $2 )
            ast__delete( ast__term( $2 ) );

        ERROK;
    };


term_item:

    TOK__L_PAREN TOK__R_PAREN
    {
#if DEBUG__PARSER
        production( "term_item ::=  TOK__L_PAREN TOK__R_PAREN" );
#endif

        $$ = ast__null();
    }

    | TOK__CHAR
    {
#if DEBUG__PARSER
        production( "term_item ::=  TOK__CHAR" );
#endif

        $$ = ast__char( $1 );
    }

    | TOK__REAL
    {
#if DEBUG__PARSER
        production( "term_item ::=  TOK__REAL" );
#endif

        $$ = ast__float( $1 );
    }

    | TOK__INT
    {
#if DEBUG__PARSER
        production( "term_item ::=  TOK__INT" );
#endif

        $$ = ast__int( $1 );
    }

    | TOK__STRING
    {
#if DEBUG__PARSER
        production( "term_item ::=  TOK__STRING" );
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

    TOK__L_SQ_BRACKET term TOK__R_SQ_BRACKET
    {
#if DEBUG__PARSER
        production( "bracketed_term ::=  TOK__L_SQ_BRACKET term TOK__R_SQ_BRACKET" );
#endif

        if ( $2 )
            $$ = ( void* ) ast__term( $2 );

        else
            $$ = 0;
    }

    | TOK__L_SQ_BRACKET error
    {
#if DEBUG__PARSER
        production( "bracketed_term ::=  TOK__L_SQ_BRACKET error" );
#endif

        $$ = 0;

        ERROK;
    }

    | TOK__L_SQ_BRACKET term error
    {
#if DEBUG__PARSER
        production( "bracketed_term ::=  TOK__L_SQ_BRACKET term error" );
#endif

        $$ = 0;

        if ( $2 )
            ast__delete( ast__term( $2 ) );

        ERROK;
    };


bag:

    bag_head TOK__R_BRACE
    {
#if DEBUG__PARSER
        production( "bag ::=  bag_head TOK__R_BRACE" );
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

    TOK__L_BRACE term
    {
#if DEBUG__PARSER
        production( "bag_head ::=  TOK__L_BRACE term" );
#endif

        if ( $2 )
        {
            $$ = ( void* ) array__new( 1, 0 );
            array__enqueue( $$, term2ast( $2 ) );
        }

        else
            $$ = 0;
    }

    | TOK__L_BRACE error
    {
#if DEBUG__PARSER
        production( "bag_head ::=  TOK__L_BRACE error" );
#endif

        $$ = 0;

        ERROK;
    }

    | bag_head TOK__COMMA term
    {
#if DEBUG__PARSER
        production( "bag_head ::=  bag_head TOK__COMMA term" );
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

    | bag_head TOK__COMMA error
    {
#if DEBUG__PARSER
        production( "bag_head ::=  bag_head TOK__COMMA error" );
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
        production( "name ::=  TOK__ID" );
#endif

        if ( $1 )
        {
            $$ = array__new( 1, 0 );
            array__enqueue( $$, $1 );
        }

        else
            $$ = 0;
    }

    | name TOK__COLON id
    {
#if DEBUG__PARSER
        production( "name ::=  name TOK__COLON TOK__ID" );
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

    | name TOK__COLON error
    {
#if DEBUG__PARSER
        production( "name ::=  name TOK__COLON error" );
#endif

        $$ = 0;
        ast__delete( ast__name( $1 ) );

        ERROK;
    };


/* Trivial rule prevents memory leaks when non-grammatical TOK__ID is matched by
   the lexer. */
id:

    TOK__ID
    {
#if DEBUG__PARSER
        production( "id ::=  TOK__ID" );
#endif

        $$ = STRDUP( $1 );
    };


%%


static void
handle_command( Interpreter *c, char *name, Ast *args )
{
    if ( !interpreter__quiet( c ) )
    {
        PRINT( "\n" );

#ifdef TOK__COMMAND_OUTPUT_PREFIX
        PRINT( TOK__COMMAND_OUTPUT_PREFIX );
#endif
    }

    /* Note: ownership of name and arguments is conferred to
       interpreter__evaluate_command. */
    exit_early = exit_early || interpreter__evaluate_command( c, name, args, lexer__get_buffer() );

    if ( !interpreter__quiet( c ) )
    {
#ifdef TOK__COMMAND_OUTPUT_SUFFIX
        PRINT( TOK__COMMAND_OUTPUT_SUFFIX );
#endif
    }

    lexer__clear_buffer();
}


static void
handle_expression( Interpreter *c, Name *name, Ast *expr )
{
    if ( !interpreter__quiet( c ) )
    {
        PRINT( "\n" );

#ifdef EXPRESSION_OUTPUT_PREFIX
        PRINT( EXPRESSION_OUTPUT_PREFIX );
#endif
    }

    /* Note: ownership of name and expression is conferred to
       interpreter__evaluate_expression. */
    exit_early = exit_early || interpreter__evaluate_expression( c, name, expr, lexer__get_buffer() );

    if ( !interpreter__quiet( c ) )
    {
#ifdef EXPRESSION_OUTPUT_SUFFIX
        PRINT( EXPRESSION_OUTPUT_SUFFIX );
#endif
    }

    lexer__clear_buffer();
}


static void
handle_error( Interpreter *c )
{
    char error_msg[ ERROR_BUFFER__SIZE + 0x20 ];

    if ( !interpreter__quiet( c ) )
    {
        PRINT( "\n" );

#ifdef ERROR_OUTPUT_PREFIX
        PRINT( ERROR_OUTPUT_PREFIX );
#endif
    }

    sprintf( error_msg, "line %d, column %d: %s",
        error_line_number, error_character_number, yyerror_msg );

    *yyerror_msg = '\0';
    exit_early = exit_early || interpreter__handle_parse_error( c, STRDUP( error_msg ) );

    if ( !interpreter__quiet( c ) )
    {
#ifdef ERROR_OUTPUT_SUFFIX
        PRINT( ERROR_OUTPUT_SUFFIX );
#endif
    }

    lexer__clear_buffer();
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
