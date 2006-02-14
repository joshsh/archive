%{

/**

\file  p2_parser.l

\brief  Flex input file for the command line interface.

\note  White space is always relevant in Phase2 expressions.  Indentifiers
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

#include "p2_parser.tab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strdup


// extern YYSTYPE yylval;


// Line and character numbers //////////////////////////////////////////////////

extern int suppress_output, show_line_numbers;

int line_number, yy_n_chars__at_last_newline;


/** Decorate command-line input. */
void print_input_prefix()
{
    if ( !suppress_output )
    {
        // Display a line number at the beginning of each new line of input.
        if ( show_line_numbers )
            printf( "%d ", line_number );

        // Add a prefix to each new line of input, to distinguish it from
        // output, and possibly to separate it from the line number.
        #ifdef INPUT_PREFIX
            printf( INPUT_PREFIX );
        #endif
    }
}


/** Initialize the lexer for a new invocation of yyparse(). */
void new_parse()
{
    line_number = 1;
    yy_n_chars__at_last_newline = 0;

    print_input_prefix();
}


int buf_pos()
{
    //return ( int ) ( YY_CURRENT_BUFFER->yy_buf_pos - YY_CURRENT_BUFFER->yy_ch_buf );
    return YY_CURRENT_BUFFER->yy_n_chars;
}


void new_line()
{
    line_number++;

    yy_n_chars__at_last_newline = buf_pos();

    print_input_prefix();
}


/** \return  the current line number.  Starts at 0. */
int get_line_number()
{
    return line_number;
}


/** \return the current "column" number, where tabs only count for one column. */
int get_char_number()
{
    //return buf_pos() - yy_n_chars__at_last_newline;

    //int i = 0;
    //while ( yytext[i] != '\0' )
    //    i++;
    //return i;

    return 0;
}


// Debugging output ////////////////////////////////////////////////////////////

#ifdef LEXER_DEBUG
    #define ECHO echo_token
#else
    #define ECHO
#endif


/** Debugging output. */
void echo_token( char *s )
{
    printf( "Found %s.\n", s );
}


// Comments ////////////////////////////////////////////////////////////////////

/** Allows nested comments. */
int comment_level = 0;


// Quoted identifiers //////////////////////////////////////////////////////////

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


////////////////////////////////////////////////////////////////////////////////

%}


/** For command names. */
LETTER                [A-Za-z]
ALNUM                 [A-Za-z0-9]

/** For lexical identifiers. */
NAME_CHAR             [^\0-\40/*:;=(){,}\"\\]


/** Comments and string literals have their own mini-scanners. */
%x SC_STRING SC_COMMENT

%option yylineno


%%


<INITIAL>
{
    "/"+"*"  {  // Begins a C-style comment.
        ECHO( "COMMENT" );
        comment_level++;
        BEGIN( SC_COMMENT );
    }

    ["]  {  // Begins a string literal.
        //advance_character_number(1);
        BEGIN( SC_STRING );
    }

    ":"  {  // Namespace delimiter.
        ECHO("COLON");
        //advance_character_number(1);
        return COLON;
    }

    ";"  {  // Marks the end of an expression or command.
        ECHO("SEMICOLON");
        //advance_character_number(1);
        return SEMICOLON;
    }

    "="  {  // Assignment operator.
        ECHO("EQUALS");
        //advance_character_number(1);
        return EQUALS;
    }

    "("  {  // Marks the beginning of a sub-term.
        ECHO("OPEN_PAREN");
        //advance_character_number(1);
        return OPEN_PAREN;
    }

    ")"  {  // Marks the end of a sub-term.
        ECHO("CLOSE_PAREN");
        //advance_character_number(1);
        return CLOSE_PAREN;
    }

    "{"  {  // Marks the beginning of an enumerated collection.
        ECHO("OPEN_CURLY_BRACKET");
        //advance_character_number(1);
        return OPEN_CURLY_BRACKET;
    }

    ","  {  // Separates elements in an enumerated collection.
        ECHO("COMMA");
        //advance_character_number(1);
        return COMMA;
    }

    "}"  {  // Marks the end of an enumerated collection.
        ECHO("CLOSE_CURLY_BRACKET");
        //advance_character_number(1);
        return CLOSE_CURLY_BRACKET;
    }

    "\\"{LETTER}{ALNUM}*  {
        ECHO("COMMAND_NAME");
        //advance_character_number(strlen(yytext));
        yylval.string = strdup(yytext + 1);  // Free this string outside of the lexer.
        return COMMAND_NAME;
    }

    (("/"*{NAME_CHAR}+)|"*"+)?"/"*  {  // Note: matches the empty string.
        ECHO("STRING");
        //advance_character_number(strlen(yytext));
        yylval.string = strdup(yytext);  // Free this string outside of the lexer.
        return STRING;
    }

    \n  {  // Report newlines to the parser (but not as tokens).
        ECHO("[newline]");
        new_line();
    }

    [ \t]  {  // Ignore whitespace and carriage returns (unless debugging the lexer).
        ECHO("[whitespace]");
        //advance_character_number(1);
    }

    <<EOF>>  {
        ECHO("EOF");

        return E_O_F;
    }

    .  {  // Ignore unrecognized characters (unless debugging the lexer).
        ECHO("[unrecognized character]");
        //advance_character_number(1);
    }
}


<SC_STRING>
{
    ([^"\n\\]*|\\[^\n])  {  // Body of string literal.
        //advance_character_number(2);
        append_to_buffer( yytext );
    }

    \\?\n  {  // Newlines are permitted.
        append_to_buffer( yytext );
        new_line();
    }

    \"  {  // End quote after string literal.
        //advance_character_number(1);
        yylval.string = strdup(yytext);  // Free this string outside of the lexer.
        BEGIN( INITIAL );
        yylval.string = extract_literal( string_buffer );
        buffer_pos = string_buffer;
        return STRING;
    }
}


<SC_COMMENT>
{
    "/*"  {  // Begins a nested C-style comment.
        ECHO( "COMMENT (nested)" );
        comment_level++;
    }

    [^*\n]        /* Eat anything that's not a '*'. */

    "*"[^*/\n]*   /* Eat up '*'s not followed by '/'s. */

    \n  {
        new_line();
    }

    "*/"  {
        if (! --comment_level )
            BEGIN( INITIAL );
    }
}


%%


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
