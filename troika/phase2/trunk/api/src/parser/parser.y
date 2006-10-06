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

#include "Parser-impl.h"

/* FIXME: global variable */
Parser *parser;

/*
#define YYmalloc    malloc
#define YYFREE      free
*/

/*
struct yy_buffer_state;

extern struct yy_buffer_state
yy_scan_string( const char *yy_str );
*/


/******************************************************************************/


int error_character_number, error_line_number;

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
yyerror( Parser *p, exit_state *ignored, const char *msg )
{
    /* Avoid "unused parameter" compiler warning. */
    p = 0;
    ignored = 0;

    /* Only the first error in a statement is reported. */
    if ( *yyerror_msg )
    {
#if DEBUG__PARSER
        PRINT( "yyerror: %s (not reported due to previous error)\n", msg );
#endif
    }

    else
    {
#if DEBUG__PARSER
        PRINT( "yyerror: %s (reported)\n", msg );
#endif

        if ( strlen( msg ) >= ERROR_BUFFER__SIZE )
            strcpy( yyerror_msg, "[parse error message overflows buffer]" );
        else
            strcpy( yyerror_msg, msg );

        error_character_number = get_char_number();
        error_line_number = get_line_number();
    }
}


#define ERROK  { yyerrok; parser->exit_early = FALSE; }


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


struct Command
{
    OBJ( STRING ) *name;
    OBJ( ARRAY ) *args;
};


struct Expression
{
    Object *value;
    OBJ( NAME ) *name;
};


/******************************************************************************/


%}


%union
{
    char                character;
    double              real;
    int                 integer;
    char                *string;

    /* Semantic values cannot use “real” objects, but only pointers to them. */
    struct Command      *command;
    struct Expression   *expression;

    struct Object       *object;
}


%token TOK__L_PAREN         TOK__R_PAREN
/*%token TOK__L_SQ_BRACKET    TOK__R_SQ_BRACKET*/
%token TOK__L_BRACE         TOK__R_BRACE
%token TOK__L_ASSIGN        TOK__R_ASSIGN
%token TOK__COLON           TOK__COMMA          TOK__SEMICOLON
%token TOK__EOF

%token <character>  TOK__CHAR
%token <integer>    TOK__INT
%token <real>       TOK__REAL
%token <string>     TOK__COMMAND_NAME   TOK__ID TOK__STRING

%type <command>     command
%type <expression>  expression
%type <object>      bag bag_head command_name name command_args subterm object term_item

%destructor { free( $$ ); } TOK__COMMAND_NAME TOK__ID TOK__STRING
%destructor { if ( $$ ) free( $$ ); } command expression

/*
%pure_parser
*/

%parse-param { Parser *parser }
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
        {
            parser__handle_error( parser, yyerror_msg );
            *yyerror_msg = '\0';
        }

        if ( parser->exit_early )
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

        new_parse( parser );

        ERROK;
    }

    | statements statement
    {
#if DEBUG__PARSER
        production( "statements ::=  statements statement" );
#endif

        if ( *yyerror_msg )
        {
            parser__handle_error( parser, yyerror_msg );
            *yyerror_msg = '\0';
        }

        if ( parser->exit_early )
        {
            *return_state = exit_state__aborted;
            YYACCEPT;
        }
    }

    /* Redundant semicolons are tolerated. */
    | statements TOK__SEMICOLON
    {
#if DEBUG__PARSER
        production( "statements ::=  statements TOK__SEMICOLON" );
#endif

        if ( *yyerror_msg )
        {
            parser__handle_error( parser, yyerror_msg );
            *yyerror_msg = '\0';
        }

        if ( parser->exit_early )
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
        {
            parser__handle_error( parser, yyerror_msg );
            *yyerror_msg = '\0';
        }

        if ( parser->exit_early )
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

        if ( $1 )
        {
            parser__handle_command( parser, $1->name, $1->args );
            free( $1 );
        }
    }

    /* Wait for the semicolon before evaluating an expression. */
    | expression TOK__SEMICOLON
    {
#if DEBUG__PARSER
        production( "statement ::=  expression TOK__SEMICOLON" );
#endif

        if ( $1 )
        {
            parser__handle_expression( parser, $1->name, $1->value );
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

        $$ = NEW( struct Command );

        if ( $$ )
        {
            $$->name = $1;
            $$->args = PARSER_REF2OBJ( ARRAY )( parser, array__new( 0, 0 ) );
        }
    }

    | command_name error
    {
#if DEBUG__PARSER
        production( "command ::=  command_name error" );
#endif

        $$ = 0;

        ERROK;
    }

    /* Command with command_args. */
    | command_name command_args
    {
#if DEBUG__PARSER
        production( "command ::=  command_name command_args" );
#endif

        if ( $1 && $2 )
        {
            $$ = NEW( struct Command );

            if ( $$ )
            {
                $$->name = $1;
                $$->args = $2;
            }
        }

        else
            $$ = 0;
    }

    | command_name command_args error
    {
#if DEBUG__PARSER
        production( "command ::=  command_name command_args error" );
#endif

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

        $$ = PARSER_REF2OBJ( STRING )( parser, $1 );
    };


command_args:

    name
    {
        Array *args;

#if DEBUG__PARSER
        production( "command_args ::=  name" );
#endif

        $$ = 0;

        args = array__new( 1, 0 );
        if ( args )
        {
            if ( array__enqueue( args, $1 ) )
                $$ = PARSER_REF2OBJ( ARRAY )( parser, args );
            else
                array__free( args );
        }
    }

    | command_args name
    {
#if DEBUG__PARSER
        production( "command_args ::=  command_args name" );
#endif

        $$ = $1;

        if ( $$ )
        {
            if ( !array__enqueue( object__value( $$ ), $2 ) )
                $$ = 0;
        }
    };


expression:

    /* Anonymous expression. */
    object
    {
#if DEBUG__PARSER
        production( "expression ::=  object" );
#endif

        $$ = NEW( struct Expression );

        if ( $$ )
        {
            $$->value = $1;
            $$->name = 0;
        }
    }

    | object error
    {
#if DEBUG__PARSER
        production( "expression ::=  object error" );
#endif

        $$ = 0;

        ERROK;
    }

    /* Left assignment from expression. */
    | name TOK__L_ASSIGN object
    {
#if DEBUG__PARSER
        production( "expression ::=  name TOK__L_ASSIGN object" );
#endif

        if ( $1 && ( $3 || FIRST_CLASS_NULL ) )
        {
            $$ = NEW( struct Expression );

            if ( $$ )
            {
                $$->name = $1;
                $$->value = $3;
            }
        }

        else
            $$ = 0;
    }

    | name TOK__L_ASSIGN error
    {
#if DEBUG__PARSER
        production( "expression ::=  name TOK__L_ASSIGN error" );
#endif

        $$ = 0;

        ERROK;
    }

    | name TOK__L_ASSIGN object error
    {
#if DEBUG__PARSER
        production( "expression ::=  name TOK__L_ASSIGN object error" );
#endif

        $$ = 0;

        ERROK;
    }

    /* Right assignment from expression. */
    | object TOK__R_ASSIGN name
    {
#if DEBUG__PARSER
        production( "expression ::=  object TOK__R_ASSIGN name" );
#endif

        if ( ( $1 || FIRST_CLASS_NULL ) && $3 )
        {
            $$ = NEW( struct Expression );

            if ( $$ )
            {
                $$->value = $1;
                $$->name = $3;
            }
        }

        else
            $$ = 0;
    }

    /* Named expression. */
    | object TOK__R_ASSIGN name error
    {
#if DEBUG__PARSER
        production( "expression ::=  object TOK__R_ASSIGN name error" );
#endif

        $$ = 0;

        ERROK;
    }

    | object TOK__R_ASSIGN error
    {
#if DEBUG__PARSER
        production( "expression ::=  term TOK__R_ASSIGN error" );
#endif

        $$ = 0;

        ERROK;
    };


object:

    subterm
    {
#if DEBUG__PARSER
        production( "object ::=  subterm" );
#endif

        $$ = $1;
    }

    | object subterm
    {
        Apply *a;

#if DEBUG__PARSER
        production( "object ::=  object subterm" );
#endif

        if ( ( $1 && $2 ) || FIRST_CLASS_NULL )
        {
            a = apply__new( $1, $2 );
            $$ = PARSER_REF2OBJ( APPLY )( parser, a );
        }

        else
            $$ = 0;
    };


subterm:

    term_item
    {
#if DEBUG__PARSER
        production( "subterm ::=  term_item" );
#endif

        $$ = $1;
    }

    | TOK__L_PAREN object TOK__R_PAREN
    {
#if DEBUG__PARSER
        production( "subterm ::=  TOK__L_PAREN object TOK__R_PAREN" );
#endif

        /* "Remove the parentheses". */
        $$ = $2;
    }

    | TOK__L_PAREN error
    {
#if DEBUG__PARSER
        production( "subterm ::=  TOK__L_PAREN error" );
#endif

        $$ = 0;

        ERROK;
    }

    | TOK__L_PAREN object error
    {
#if DEBUG__PARSER
        production( "subterm ::=  TOK__L_PAREN object error" );
#endif

        $$ = 0;

        ERROK;
    };


term_item:

    TOK__L_PAREN TOK__R_PAREN
    {
#if DEBUG__PARSER
        production( "term_item ::=  TOK__L_PAREN TOK__R_PAREN" );
#endif

        $$ = 0;
    }

    | TOK__CHAR
    {
#if DEBUG__PARSER
        production( "term_item ::=  TOK__CHAR" );
#endif

        $$ = PARSER_VAL2OBJ( CHARACTER )( parser, $1 );
    }

    | TOK__REAL
    {
#if DEBUG__PARSER
        production( "term_item ::=  TOK__REAL" );
#endif

        $$ = PARSER_VAL2OBJ( DOUBLE )( parser, $1 );
    }

    | TOK__INT
    {
#if DEBUG__PARSER
        production( "term_item ::=  TOK__INT" );
#endif

        $$ = PARSER_VAL2OBJ( INTEGER )( parser, $1 );
    }

    | TOK__STRING
    {
#if DEBUG__PARSER
        production( "term_item ::=  TOK__STRING" );
#endif

        $$ = PARSER_REF2OBJ( STRING )( parser, $1 );
    }

    | bag
    {
#if DEBUG__PARSER
        production( "term_item ::=  bag" );
#endif

        $$ = $1;
    }

    | name
    {
#if DEBUG__PARSER
        production( "term_item ::=  name" );
#endif

        $$ = $1;
    }


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

        ERROK;
    };


bag_head:

    TOK__L_BRACE object
    {
        Array *bag = 0;

#if DEBUG__PARSER
        production( "bag_head ::=  TOK__L_BRACE object" );
#endif

        if ( $2 || FIRST_CLASS_NULL )
        {
            bag = array__new( 1, 0 );
            if ( bag )
            {
                if ( array__enqueue( bag, $2 ) )
                    $$ = PARSER_REF2OBJ( ARRAY )( parser, bag );

                else
                {
                    array__free( bag );
                    $$ = 0;
                }
            }

            else
                $$ = 0;
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

    | bag_head TOK__COMMA object
    {
#if DEBUG__PARSER
        production( "bag_head ::=  bag_head TOK__COMMA object" );
#endif

        if ( $1 && ( $3 || FIRST_CLASS_NULL ) )
        {
            $$ = $1;
            if ( !array__enqueue( object__value( $$ ), $3 ) )
                $$ = 0;
        }

        else
            $$ = 0;
    }

    | bag_head TOK__COMMA error
    {
#if DEBUG__PARSER
        production( "bag_head ::=  bag_head TOK__COMMA error" );
#endif

        $$ = 0;

        ERROK;
    };


name:

    TOK__ID
    {
        Name *name;

#if DEBUG__PARSER
        production( "name ::=  TOK__ID" );
#endif

        if ( $1 )
        {
            name = array__new( 1, 0 );

            if ( name )
            {
                if ( array__enqueue( name, $1 ) )
                    $$ = PARSER_REF2OBJ( NAME )( parser, name );
                else
                    array__free( name );
            }
        }

        else
            $$ = 0;
    }

    | name TOK__COLON TOK__ID
    {
#if DEBUG__PARSER
        production( "name ::=  name TOK__COLON TOK__ID" );
#endif

        if ( $1 && $3 )
        {
            $$ = $1;
            if ( !array__enqueue( object__value( $$ ), $3 ) )
            {
                if ( $3 )
                    free( $3 );

                $$ = 0;
            }
        }

        else
        {
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

        ERROK;
    };


%%


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
