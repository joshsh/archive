%{

/**

\file  lexer.l

\brief  Flex input file for Phase2's command line interpreter.

Identifiers beginning with an underscore (e.g. _quit) are reserved for special
commands.

Comments are delimited by "(:" and ":)", and may be nested to any level.

String literals are delimited by quote characters, and use the escape
sequences {\"} for the quote character and {\\} for the backslash, e.g.

    system "echo \"Hello World!\"";

\note  White space is relevant in Phase2 expressions.  Identifiers must be
separated either by white space or by appropriate syntax tokens, e.g.

    + a b  or  (+ a)b  or  ((+)a)b  but not  +ab

(the lexer will interpret this last expression as a single token, "+ab").
To include space characters within an identifier itself, use {\ }, e.g.

    print README.txt;

but

    print Untitled\ document\ #3;

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
#undef YY_DECL
#define YY_DECL int yylex( YYSTYPE *yylval, YYLTYPE *yylloc)
*/




/*
#ifndef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( YY_CURRENT_BUFFER_LVALUE->yy_is_interactive ) \
		{ \
		int c = '*'; \
		size_t n; \
		for ( n = 0; n < max_size && \
			     (c = getc( yyin )) != EOF && c != '\n'; ++n ) \
{\
putchar('\'');\
			buf[n] = (char) c; \
}\
putchar('.');\
		if ( c == '\n' ) \
			buf[n++] = (char) c; \
		if ( c == EOF ){ \
            putchar('!');\
            if ( ferror( yyin ) ) \
			     YY_FATAL_ERROR( "input in flex scanner failed" ); \
        } \
		result = n; \
		} \
	else \
		{ \
		errno=0; \
		while ( (result = fread(buf, 1, max_size, yyin))==0 && ferror(yyin)) \
			{ \
			if( errno != EINTR) \
				{ \
				YY_FATAL_ERROR( "input in flex scanner failed" ); \
				break; \
				} \
			errno=0; \
			clearerr(yyin); \
			} \
		}\
\

#endif
*/




#include "parser.h"
#include <common.h>
#include "../settings.h"

/*#include "header.h"*/

/** Added to each row of input (if defined). */
#define INPUT_PREFIX    ">>  "


/* FIXME: not thread-safe */
static char lexer_buffer[ENCODING__BUFFER_SIZE];
static char *bufptr = lexer_buffer;
void
lexer__clear_buffer()
{
    bufptr = lexer_buffer;
    *bufptr = '\0';
}
char *
lexer__get_buffer()
{
    return lexer_buffer;
}


/* Language module dependencies ***********************************************/

typedef struct Interpreter Interpreter;

extern boolean
interpreter__quiet( Interpreter *c );

extern boolean
interpreter__show_line_numbers( Interpreter *c );


/* ! */
static Interpreter *
compiler_global;


/* Line and character numbers *************************************************/


static int line_number, column_number, last_column_number;

boolean pad_newline;

/** Initialize the lexer for a new invocation of yyparse(). */
void
new_parse( Interpreter * /* itp */ );

/** \return  the current line number.  Starts at 1. */
int
get_line_number( void );

/** \return the current "column" number, where tabs only count for one column. */
int
get_char_number( void );


static int
new_token( int /* tok */, char * /* text */, int /* length */, const char * /* name */ );

static void
new_line( Interpreter *c );

/** TOKEN macro handles debugging messages and column count as well as return
    values from rules.  It must be the last statement in a rule action. */
#define NEWTOK( x ) new_token( x, yytext, yyleng, #x )
#define TOKEN( x )  if ( x ) return NEWTOK( x ); else NEWTOK( x );


/* Comments *******************************************************************/


/** Allows nested comments. */
static int comment_level = 0;


/* Quoted name fragments ******************************************************/


char string_buffer[ ENCODING__BUFFER_SIZE ];
char *buffer_limit = string_buffer + ENCODING__BUFFER_SIZE - 1;
char *buffer_pos = string_buffer;


static void
append_to_buffer( char *s )
{
    int len = strlen( s );

    if ( buffer_pos + len < buffer_limit )
    {
        strcpy( buffer_pos, s );
        buffer_limit += len;
        buffer_pos += len;
    }
}


/** \return  the interior of an id, with escape sequences undone */
static char *
extract_id( char *s )
{
    char *s_low = s, *s_high = s_low;

    while ( *s_high != '\0' )
    {
         if ( *s_high == '\\' )
         {
             s_high++;
             switch ( *s_high )
             {
                 case ' ':
                     *s_low = ' ';
                     break;

                 default:
                     ERROR( "extract_id: bad char: %i ('%c')\n", ( int ) *s_high, *s_high );
                     return 0;
             }
        }

        else
            *s_low = *s_high;

        s_low++;
        s_high++;
    }

    *s_low = '\0';

    return s;
}


/** \return  the interior of a string literal, with escape sequences undone */
static char *
extract_literal( char *s )
{
    char *s_low = s, *s_high = s_low;

    while ( *s_high != '\0' )
    {
         if ( *s_high == '\\' )
         {
             s_high++;
             switch ( *s_high )
             {
                 case '"':
                     *s_low = '"';
                     break;
                 case '\\':
                     *s_low = '\\';
                     break;
                 default:
                     *s_low = '\\';
                     s_low++;
                     *s_low = *s_high;
             }
        }

        else
            *s_low = *s_high;

        s_low++;
        s_high++;
    }

    *s_low = '\0';

    return s;
}


/******************************************************************************/


%}


/** For command names. */
ALNUM   [A-Za-z0-9]

/** For lexical identifiers. */
IDCHAR  [^_\]\[[:space:]:;(){,}\"\\]


/** Comments and string literals have their own mini-scanners. */
%x SC__STRING SC__COMMENT

/* Wrappers provided below.  Don't use the defaults. */
%option noyyalloc
%option noyyrealloc
%option noyyfree

/*
%option reentrant
%option bison-bridge
*/

%%


<INITIAL>
{
    "(:"  {  /* Begins a comment. */
        comment_level++;
        BEGIN( SC__COMMENT );
        TOKEN( 0 );
    }

    ["]  {  /* Begins a string literal. */
        BEGIN( SC__STRING );
        TOKEN( 0 );
    }

    ":"  {  /* Namespace delimiter. */
        TOKEN( TOK__COLON );
    }

    ";"  {  /* Marks the end of an expression or command. */
        TOKEN( TOK__SEMICOLON );
    }

    ":="  {  /* Leading assignment operator. */
        TOKEN( TOK__L_ASSIGN );
    }

    "=:"  {  /* Trailing assignment operator. */
        TOKEN( TOK__R_ASSIGN );
    }

    "("  {  /* Marks the beginning of a sub-term. */
        TOKEN( TOK__L_PAREN );
    }

    ")"  {  /* Marks the end of a sub-term. */
        TOKEN( TOK__R_PAREN );
    }

    "["  {  /* Marks the beginning of a general sequence. */
        TOKEN( TOK__L_SQ_BRACKET );
    }

    "]"  {  /* Marks the beginning of a general sequence. */
        TOKEN( TOK__R_SQ_BRACKET );
    }

    "{"  {  /* Marks the beginning of a bag. */
        TOKEN( TOK__L_BRACE );
    }

    ","  {  /* Separates elements in a bag. */
        TOKEN( TOK__COMMA );
    }

    "}"  {  /* Marks the end of a bag. */
        TOKEN( TOK__R_BRACE );
    }

    _[0-9A-Za-z]+  {
        yylval.string = yytext + 1;

        TOKEN( TOK__COMMAND_NAME );
    }

    '[^']'  {  /* Matches an ASCII character in single quotes. */
        /* Note: no escaped characters yet. */
        yylval.character = *( ++yytext );
        TOKEN( TOK__CHAR );
    }

    -?(0|([1-9][0-9]*))"."[0-9]+  {  /* A number is interpreted as a TOK__REAL if it
                                    contains a decimal point (as in Forth). */
        /* Note: atof is error-safe (it fails with a return value of 0.0). */
        yylval.real = atof( yytext );
        TOKEN( TOK__REAL );
    }

    0|(-?[1-9][0-9]*)  {
        /* Note: atoi is error-safe (it fails with a return value of 0). */
        yylval.integer = atoi( yytext );
        TOKEN( TOK__INT );
    }

    {IDCHAR}("\\ "*("_"|{IDCHAR}))*  {  /* Matches a name fragment. */
        yylval.string = extract_id( yytext );

        TOKEN( TOK__ID );
    }

    \n  {  /* Ignore newlines, except for formatting. */
        new_line( compiler_global );
        TOKEN( 0 );
    }

    \t  {  /* Ignore tabs. */
        /* Assuming 8-column tabs. */
        column_number += 7;

        TOKEN( 0 );
    }

    [ \r]  {  /* Ignore spaces and carriage returns. */
        TOKEN( 0 );
    }

    <<EOF>>  {
        TOKEN( TOK__EOF );
    }

    .  {  /* Ignore unrecognized characters. */
        TOKEN( 0 );
    }
}


<SC__STRING>
{
    ([^"\n\t\\]*|\\[^\n\t])  {  /* Body of string literal. */
        append_to_buffer( yytext );
        TOKEN( 0 );
    }

    \\?\n  {  /* Newlines are permitted. */
        append_to_buffer( yytext );
        new_line( compiler_global );
        TOKEN( 0 );
    }

    \\?\t  {
        append_to_buffer( yytext );

        /* Assuming 8-column tabs. */
        column_number += 7;

        TOKEN( 0 );
    }

    \"  {  /* End quote after string literal. */
        /* This is only necessary for the null string (""). */
        *buffer_pos = '\0';

        yylval.string = extract_literal( string_buffer );
        buffer_pos = string_buffer;

        BEGIN( INITIAL );
        TOKEN( TOK__STRING );
    }
}


<SC__COMMENT>
{
    "(:"  {  /* Begins a comment within a comment. */
        comment_level++;
        TOKEN( 0 );
    }

    ":)"  {  /* Ends a comment. */
        if (! --comment_level )
        {
            BEGIN( INITIAL );
            TOKEN( 0 );
        }

        else
            TOKEN( 0 );
    }

    \n  {  /* Newlines are ignored, except for formatting. */
        new_line( compiler_global );

        TOKEN( 0 );
    }

    \t  {
        /* Assuming 8-column tabs. */
        column_number += 7;

        TOKEN( 0 );
    }

    .  {  /* Ignore everything else. */
        TOKEN( 0 );
    }
}


%%


void *
yyalloc( yy_size_t size )
{
    return ( void* ) malloc( size );
}


void *
yyrealloc( void *ptr, yy_size_t size )
{
    return ( void* ) realloc( ( char* ) ptr, size );
}


void
yyfree( void *ptr )
{
    free( ( char* ) ptr );
}


int
new_token( int tok, char * text, int length, const char * name )
{
#if DEBUG__LEXER
    if ( tok )
        PRINT( "Matched %s\n", name );
#endif

    sprintf( bufptr, text );
    bufptr += strlen( bufptr );

    last_column_number = column_number;
    column_number += length;

    /* Avoid an 'unused parameter' warning. */
    text = "bogus";

    return tok;
}


void
new_line( Interpreter *c )
{
    line_number++;
    column_number = last_column_number = 0;

    /* Decorate command-line input. */
    if ( !interpreter__quiet( c ) )
    {

        if ( pad_newline )
            PRINT( "\n" );

        pad_newline = FALSE;

        /* Display a line number at the beginning of each new line of input. */
        if ( interpreter__show_line_numbers( c ) )
            PRINT( "%d ", line_number );

        /* Add a prefix to each new line of input, to distinguish it from
           output, and possibly to separate it from the line number. */
#ifdef INPUT_PREFIX
        PRINT( INPUT_PREFIX );
#endif
    }
}


/** Initialize the lexer for a new invocation of yyparse(). */
void
new_parse( Interpreter *c )
{
    compiler_global = c;
    line_number = 0;
    pad_newline = TRUE;
    new_line( c );

    lexer__clear_buffer();
}


/** \return  the current line number.  Starts at 1. */
int
get_line_number( void )
{
    return line_number;
}


/** \return the current "column" number, where tabs only count for one column. */
int
get_char_number( void )
{
    if ( !last_column_number )
        return 1;
    else
        return last_column_number;
}


/* extern YYSTYPE yylval; */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
