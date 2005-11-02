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
void echo_token(char *s);

/** Gets the interior of a string literal, undoing the two escape sequences
    in the process. */
char *extract_literal(char *s);


////////////////////////////////////////////////////////////////////////////////

%}


/** A minimal set of special characters reserved as stand-alone syntax elements.
    \note  The backslash and double quote are also special characters. */
SEMICOLON             [;]
EQUALS                [=]
OPEN_PAREN            [(]
CLOSE_PAREN           [)]
SLASH                 [/]

/** Letter or decimal digit. */
ALNUM                 [A-Za-z0-9]

/** Non-syntactic, non-whitespace character (excluding slash and equals). */
NS_HEAD               [^;=()\0-\40"/]

/** Non-syntactic, non-whitespace character (but including slash and equals). */
NS_TAIL               [^;()\0-\40"]

/** Non-syntactic, non-whitespace, non-alphanumeric character. */
NONALNUM              [^;()\0-\40"A-Za-z0-9]

/** \note  Multi-line literals are accepted. */
STRLIT                ["]([^"\\]|([\\]([\"]|[\\]|[^"\\])))*["]


%%


{SEMICOLON}  {
    ECHO("SEMICOLON");
    advance_character_number(1);
    return SEMICOLON;
}


{EQUALS}  {
    ECHO("EQUALS");
    advance_character_number(1);
    return EQUALS;
}


{OPEN_PAREN}  {
    ECHO("OPEN_PAREN");
    advance_character_number(1);
    return OPEN_PAREN;
}


{CLOSE_PAREN}  {
    ECHO("CLOSE_PAREN");
    advance_character_number(1);
    return CLOSE_PAREN;
}


{SLASH}{ALNUM}{NS_TAIL}*  {  // It's only a command if the slash is followed with a letter or digit.
    ECHO("COMMAND_NAME");
    advance_character_number(strlen(yytext));
    yylval.string = strdup(yytext + 1);  // Free this string outside of the lexer.
    return COMMAND_NAME;
}


({NS_HEAD}|({SLASH}{NONALNUM})){NS_TAIL}*  {  // Simple string literal.
    ECHO("STRING");
    advance_character_number(strlen(yytext));
    yylval.string = strdup(yytext);  // Free this string outside of the lexer.
    return STRING;
}


{STRLIT}  {  // String literal in quotes (to allow whitespace and special characters).
    ECHO("STRING");
    advance_character_number(strlen(yytext));
    yylval.string = extract_literal(yytext);  // Free this string outside of the lexer.
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

void echo_token(char *s)
{
    printf("Found %s.\n", s);
}


char *extract_literal(char *s)
{
    char *s_low = s, *s_high = s_low + 1;

    while (*s_high != '"')
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

