%{

/**

\file  p2_parser.l

\brief  Flex input file for the command line interface.

Comments are delimited by "(:" and ":)", and may be nested to any level.

\note  White space is always relevant in Phase2 expressions.  Identifiers
must be separated either by white space or by appropriate syntax tokens, e.g.

    + a b  or  (+ a)b  or  ((+)a)b  but not  +ab

(the lexer will interpret this last expression as a single token, "+ab").
To preserve white space within an identifier itself, enclose it in quotation
marks, e.g.

    >> 1  print README.txt;

but

    >> 1  print "Untitled document #3";

To include a quote character in such a string, use {\"}.  For the backslash
character, use {\\}.  There are no other escape sequences in quoted identifiers.

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n

********************************************************************************

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

#include "debug.h"

#include "p2_parser.tab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  /* strdup */


/** Added to each row of input (if defined). */
#define INPUT_PREFIX  ">>  "


/* Line and character numbers *************************************************/


int line_number, column_number, last_column_number, statement_number;


/** Decorate command-line input. */
void print_input_prefix()
{
    if ( !p2_parser__suppress_output() )
    {
        /* Display a line number at the beginning of each new line of input. */
        if ( p2_parser__show_line_numbers() )
            printf( "%d ", line_number );

        /* Add a prefix to each new line of input, to distinguish it from
           output, and possibly to separate it from the line number. */
        #ifdef INPUT_PREFIX
            printf( INPUT_PREFIX );
        #endif
    }
}


void new_token( int length )
{
    last_column_number = column_number;
    column_number += length;
}


void new_line()
{
    line_number++;
    column_number = last_column_number = 0;
    statement_number = 0;

    print_input_prefix();
}


/** Initialize the lexer for a new invocation of yyparse(). */
void new_parse()
{
    line_number = 0;
    new_line();
}


/** \return  the current line number.  Starts at 1. */
int get_line_number()
{
    return line_number;
}


/** \return the current "column" number, where tabs only count for one column. */
int get_char_number()
{
    if ( !last_column_number )
        return 1;
    else
        return last_column_number;
}


/* Debugging output ***********************************************************/


/** TOKEN macro handles debugging messages and column count as well as return
    values from rules.  It must be the last statement in a rule action. */
#if DEBUG__LEXER
    #define TOKEN( x, y )  { new_token( yyleng ); \
                              if ( y ) printf( "Matched {%s}\n", y ); \
                              if ( x ) return x; }
#else
    #define TOKEN( x, y )  { new_token( yyleng ); \
                            if ( x ) return x; }
#endif


/* Comments *******************************************************************/


/** Allows nested comments. */
int comment_level = 0;


/* Quoted name fragments ******************************************************/

/** Buffer size is arbitary... */
#define STRING_BUFFER__SIZE  0xFFF

char string_buffer[STRING_BUFFER__SIZE];
char *buffer_limit = string_buffer + STRING_BUFFER__SIZE - 1;
char *buffer_pos = string_buffer;


void append_to_buffer( char *s )
{
    int len = strlen( s );

    if ( buffer_pos + len < buffer_limit )
    {
        strcpy( buffer_pos, s );
        buffer_limit += len;
        buffer_pos += len;
    }
}


/** \return  the interior of a string literal, with escape sequences undone */
char *extract_literal(char *s)
{
    char *s_low = s, *s_high = s_low;

    while (*s_high != '\0')
    {
         if (*s_high == '\\')
         {
             s_high++;
             switch (*s_high)
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

    return strdup(s);
}


/******************************************************************************/


%}


/** For command names. */
LETTER                [A-Za-z]
ALNUM                 [A-Za-z0-9]

/** For lexical identifiers. */
NAME_CHAR             [^][[:space:]:;=(){,}\"\\]


/** Comments and string literals have their own mini-scanners. */
%x SC_STRING SC_COMMENT


%%


<INITIAL>
{
    "(:"  {  /* Begins a comment. */
        comment_level++;
        BEGIN( SC_COMMENT );
        TOKEN( 0, "comment (begin)" );
    }

    ["]  {  /* Begins a string literal. */
        BEGIN( SC_STRING );
        TOKEN( 0, "quoted STRING (begin)" );
    }

    ":"  {  /* Namespace delimiter. */
        TOKEN( COLON, "COLON" );
    }

    ";"  {  /* Marks the end of an expression or command. */
        TOKEN( SEMICOLON, "SEMICOLON" );
    }

    "="  {  /* Assignment operator. */
        TOKEN( EQUALS, "EQUALS" );
    }

    "("  {  /* Marks the beginning of a sub-term. */
        TOKEN( OPEN_PAREN, "OPEN_PAREN" );
    }

    ")"  {  /* Marks the end of a sub-term. */
        TOKEN( CLOSE_PAREN, "CLOSE_PAREN" );
    }

    "["  {  /* Marks the beginning of a general sequence. */
        TOKEN( OPEN_SQUARE_BRACKET, "OPEN_SQUARE_BRACKET" );
    }

    "]"  {  /* Marks the beginning of a general sequence. */
        TOKEN( CLOSE_SQUARE_BRACKET, "CLOSE_SQUARE_BRACKET" );
    }

    "{"  {  /* Marks the beginning of a bag. */
        TOKEN( OPEN_CURLY_BRACKET, "OPEN_CURLY_BRACKET" );
    }

    ","  {  /* Separates elements in a bag. */
        TOKEN( COMMA, "COMMA" );
    }

    "}"  {  /* Marks the end of a bag. */
        TOKEN( CLOSE_CURLY_BRACKET, "CLOSE_CURLY_BRACKET" );
    }

    "\\"{LETTER}{ALNUM}*  {

        /* Ownership of this string is conferred to the parser. */
        yylval.string = strdup( yytext + 1 );

        TOKEN( COMMAND_NAME, "COMMAND_NAME" );
    }

    {NAME_CHAR}+  {  /* Matches a name fragment. */

        /* Ownership of this string is conferred to the parser. */
        yylval.string = strdup( yytext );

        TOKEN( STRING, "STRING" );
    }

    \n  {  /* Ignore newlines, except for formatting. */
        new_line();
        TOKEN( 0, "newline" );
    }

    [ \t]  {  /* Ignore whitespace and carriage returns. */
        TOKEN( 0, "whitespace" );
    }

    <<EOF>>  {
        TOKEN( E_O_F, "EOF" );
    }

    .  {  /* Ignore unrecognized characters. */
        TOKEN( 0, "unrecognized character" );
    }
}


<SC_STRING>
{
    ([^"\n\\]*|\\[^\n])  {  /* Body of string literal. */
        append_to_buffer( yytext );
        TOKEN( 0, 0 );
    }

    \\?\n  {  /* Newlines are permitted. */
        append_to_buffer( yytext );
        new_line();
        TOKEN( 0, "newline in SC_STRING" );
    }

    \"  {  /* End quote after string literal. */

        /* Ownership of this string is conferred to the parser. */
        yylval.string = extract_literal( string_buffer );
        buffer_pos = string_buffer;

        BEGIN( INITIAL );

        TOKEN( STRING, "quoted STRING (end)" );
    }
}


<SC_COMMENT>
{
    "(:"  {  /* Begins a nested comment. */
        comment_level++;
        TOKEN( 0, "comment (begin nested)" );
    }

    ":)"  {  /* Ends a comment. */
        if (! --comment_level )
        {
            BEGIN( INITIAL );
            TOKEN( 0, "comment (end)" );
        }

        else
            TOKEN( 0, "comment (end nested)" );
    }

    \n  {  /*  Newlines are ignored, except for formatting. */
        new_line();
        TOKEN( 0, "newline in SC_COMMENT" );
    }

    .  {  /* Ignore everything else. */
        TOKEN( 0, 0 );
    }
}


%%


/* extern YYSTYPE yylval; */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
