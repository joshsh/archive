%{

/**

\file  p2_parser.l

\brief  Flex input file for the command line interface.

This lexer deals with only four special tokens...

    ( ) = ;

...as well as command names (which are preceded with a slash) and string
literals.  String literals may be simple (e.g. foo) or quoted
(e.g. "foo = bar"), where the quoted form is just a trick to allow the user to
input literal tokens which may contain whitespace and other special characters.
The only escape sequences are \" and \\, which are un-done before they are
passed up to the parser, e.g.

    "Say \"hello\"!\\n" --> Say "hello"!\n

Literal tokens must be separated either by white space or by parentheses, e.g.

    + a b  or  (+ a)b  or  ((+)a)b  but not  +ab

(the lexer will interpret this last expression as a single token).

\note  There is no call for comments in Phase2, as you're not expected to be
working off of an "input file".  If desired, an anonymous string literal will do
the trick in the current implementation, e.g.

    "This is a comment.  It does nothing." ;

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

#include "p2_parser.tab.h"
#include "p2_parser-aux.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strdup


// extern YYSTYPE yylval;


extern void advance_character_number(int nchars);
extern void advance_line_number();
extern void print_input_prefix();

extern int suppress_output, show_line_numbers;


////////////////////////////////////////////////////////////////////////////////


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


/** Buffer size is arbitary... */
#define STRING_BUFFER__SIZE  0xFFF

char string_buffer[STRING_BUFFER__SIZE];
char *buffer_limit = string_buffer + STRING_BUFFER__SIZE - 1;
char *buffer_pos = string_buffer;
//string_buffer[0] = '\0';

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


////////////////////////////////////////////////////////////////////////////////

%}


/** Letter or decimal digit. */
ALNUM                 [A-Za-z0-9]

/** Non-syntactic, non-whitespace character (excluding slash and equals). */
NS_HEAD               [^;=()\0-\40"/]

/** Non-syntactic, non-whitespace character (but including slash and equals). */
NS_TAIL               [^;()\0-\40"]

/** Non-syntactic, non-whitespace, non-alphanumeric character. */
NONALNUM              [^;()\0-\40"A-Za-z0-9]

/** Comments and string literals have their own mini-scanners. */
%x SC_STRING SC_COMMENT

%%


<SC_STRING>
{
    ([^"\n\\]*|\\[^\n])  {  // Body of string literal.
        advance_character_number(2);
        append_to_buffer( yytext );
    }

    (\\?)\n  {  // Newlines are permitted in string literals.
        append_to_buffer( yytext );
        advance_line_number();
        print_input_prefix();
    }

    "\""  {  // End quote after string literal.
        advance_character_number(1);

        // Note: you must free this string outside of the lexer.
        yylval.string = strdup(yytext);

        BEGIN( INITIAL );
        yylval.string = extract_literal( string_buffer );
        buffer_pos = string_buffer;
        return STRING;
    }
}


<SC_COMMENT>
{
    [^*\n]*        /* eat anything that's not a '*' */

    "*"[^*/\n]*   /* eat up '*'s not followed by '/'s */

    \n  {
        advance_line_number();
        print_input_prefix();
    }

    "*/"  {
        BEGIN( INITIAL );
    }
}


"/*"  {  // Begins a C-style comment.
    ECHO( "SC_COMMENT" );
    BEGIN( SC_COMMENT );
}


["] {  // Begins a string literal.
    advance_character_number(1);
    BEGIN( SC_STRING );
}


";"  {  // Marks the end of an expression or command.
    ECHO("SEMICOLON");
    advance_character_number(1);
    return SEMICOLON;
}


"="  {  // Assignment operator.
    ECHO("EQUALS");
    advance_character_number(1);
    return EQUALS;
}

"("  {
    ECHO("OPEN_PAREN");
    advance_character_number(1);
    return OPEN_PAREN;
}


")"  {
    ECHO("CLOSE_PAREN");
    advance_character_number(1);
    return CLOSE_PAREN;
}


"{"  {
    ECHO("OPEN_CURLY_BRACKET");
    advance_character_number(1);
    return OPEN_CURLY_BRACKET;
}


","  {
    ECHO("COMMA");
    advance_character_number(1);
    return COMMA;
}


"}"  {
    ECHO("CLOSE_CURLY_BRACKET");
    advance_character_number(1);
    return CLOSE_CURLY_BRACKET;
}


"/"{ALNUM}{NS_TAIL}*  {  // It's only a command if the slash is followed with a letter or digit.
    ECHO("COMMAND_NAME");
    advance_character_number(strlen(yytext));
    yylval.string = strdup(yytext + 1);  // Free this string outside of the lexer.
    return COMMAND_NAME;
}


"/"|(({NS_HEAD}|("/"{NONALNUM})){NS_TAIL}*)  {  // Simple string literal.
    ECHO("STRING");
    advance_character_number(strlen(yytext));
    yylval.string = strdup(yytext);  // Free this string outside of the lexer.
    return STRING;
}


\n  {  // Report newlines to the parser (but not as tokens).
    ECHO("[newline]");
    advance_line_number();
    print_input_prefix();
}


[ \t]  {  // Ignore whitespace and carriage returns (unless debugging the lexer).
    ECHO("[whitespace]");
    advance_character_number(1);
}


<<EOF>>  {
    ECHO("EOF");

    return E_O_F;
}


.  {  // Ignore unrecognized characters (unless debugging the lexer).
    ECHO("[unrecognized character]");
    advance_character_number(1);
}


%%


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
