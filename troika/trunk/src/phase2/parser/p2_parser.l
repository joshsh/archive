%{

/**
    \file  p2_parser.l

    \brief  Flex input file for the command line interface.

    This lexer deals with only four special tokens...

        ( ) = ;

    ...as well as command names (which always begin with a slash) and string
    literals.  String literals may be simple (e.g. foo) or quoted
    (e.g. "foo = bar"), where the quoted form is just a trick to allow the user
    to input literal tokens which may contain whitespace and other special
    characters.

    \note  There is no call for comments in Phase2, as you're not expected to
    be working off of an "input file".  If desired, an anonymous string literal
    will do the trick in the current implementation:

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
#include "p2_parser-macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strdup


// extern YYSTYPE yylval;


#ifdef LEXER_DEBUG
    #define ECHO echo_token
#else
    #define ECHO
#endif


void echo_token(char *s);
char *extract_literal(char *s);


////////////////////////////////////////////////////////////////////////////////

%}


/** A minimal set of special characters reserved as stand-alone syntax elements.
    \note  The slash, backslash and double quote are also special characters. */
SEMICOLON             [;]
EQUALS                [=]
OPEN_PAREN            [(]
CLOSE_PAREN           [)]

/** Letter or decimal digit. */
ALNUM                 [A-Za-z0-9]

/** Non-syntactic, non-whitespace character. */
NS                    [^;=()\0-\40"]

/** Non-syntactic, non-whitespace character which is also not a slash. */
NOSLASH               [^;=()\0-\40"/]

/** Non-syntactic, non-whitespace, non-alphanumeric character. */
NONALNUM              [^;=()\0-\40"A-Za-z0-9]

/** \note  Multi-line literals are accepted.
    \note  The only escape sequences are \" and \\, and are un-done before they
    are passed up to the parser, e.g.
        "Say \"hello\"!\\n" --> Say "hello"!\n
*/
STRLIT                ["]([^"\\]|([\\]([\"]|[\\]|[^"\\])))*["]


%%


{SEMICOLON}             { ECHO("SEMICOLON"); return SEMICOLON; }
{EQUALS}                { ECHO("EQUALS"); return EQUALS; }
{OPEN_PAREN}            { ECHO("OPEN_PAREN"); return OPEN_PAREN; }
{CLOSE_PAREN}           { ECHO("CLOSE_PAREN"); return CLOSE_PAREN; }


[/]{ALNUM}{NS}*  {  // It's only a command if it starts with a letter or digit.
    ECHO("COMMAND");
    yylval.string = strdup(yytext + 1);  // Free this string in the parser.
    return COMMAND;
}


({NOSLASH}|([/]{NONALNUM})){NS}*  {  // Simple string literal.
    ECHO("STRING");
    yylval.string = strdup(yytext);  // Free this string in the parser.
    return STRING;
}


{STRLIT}  {  // String literal in quotes (to allow whitespace and special characters).
    ECHO("STRING");
    yylval.string = extract_literal(yytext);  // Free this string in the parser.
    return STRING;
}


\n  {
    ECHO("[newline]");

    #ifdef INPUT_PREFIX
        printf(INPUT_PREFIX);
    #endif
}


.  {  // Ignore unrecognized characters (unless debugging).
       ECHO("[unrecognized character]");
   }


%%

////////////////////////////////////////////////////////////////////////////////


/** Debugging output. */
void echo_token(char *s)
{
    printf("Found %s.\n", s);
}


/** Gets the interior of a string literal, undoing the two escape sequences. */
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

