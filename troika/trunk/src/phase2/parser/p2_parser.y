%{

/**
    \file  p2_parse.y

    \brief  Bison grammar file for the command line interface.

    The actual parser behavior is pretty minimal; it just matches user input
    against a simple "expression" and "command" syntax, encodes it as XML and
    sends it to the p2_itf.cpp module, which tells it what to print to the
    console.

    \note  don't forget to include curly braces after each Bison grammar clause
    (even if the action is empty), otherwise be prepared for errors of this ilk:
        j.y:54: type clash (`' `number') on default action

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

#include <stdio.h>
#include <stdlib.h>  // malloc
#include <string.h>

#include "p2_parser-macros.h"



/** Command evaluator from the semantic module. */
extern void p2_evaluate_command(char *name, char *args);

/** Expression evaluator from the semantic module. */
extern void p2_evaluate_expression(char *name, char *expr);



void yyerror(const char *str)
{
    fprintf(stderr,"Parse error: %s\n", str);
}

int yywrap()
{
    return 1;
}

/*  main() { yyparse(); }  */



// While the gSOAP DOM parser is still struggling with namespaces...
#ifdef OMIT_XMLNS

    #define TOK_    "<string>"
    #define _TOK    "</string>\n"

    #define SEQ_    "<Sequence sequenceType=\"immediate\">\n"
    #define _SEQ    "</Sequence>\n"

    #define SEQD_    "<Sequence sequenceType=\"delayed\">\n"
    #define _SEQD    "</Sequence>\n"

#else

    #define TOK_    "<xs:string>"
    #define _TOK    "</xs:string>\n"

    #define SEQ_    "<P2:Sequence sequenceType=\"immediate\">\n"
    #define _SEQ    "</P2:Sequence>\n"

    #define SEQD_    "<P2:Sequence sequenceType=\"delayed\">\n"
    #define _SEQD    "</P2:Sequence>\n"

#endif



//! Not safe as-is.  E.g. a string literal with a '"' would be a
//  problem.  Eventually token_tag will need to transform XML-unfriendly strings.
char *token_tag(char *contents)
{
    const int len = 1 + strlen(TOK_) + strlen(_TOK);
    char *s = (char *) malloc(sizeof(char) * (strlen(contents) + len));
    sprintf(s, "%s%s%s", TOK_, contents, _TOK);
    return s;
}



char *sequence_tag(char *contents)
{
    const int len = 1 + strlen(SEQ_) + strlen(_SEQ);
    char *s = (char *) malloc(sizeof(char) * (strlen(contents) + len));
    sprintf(s, "%s%s%s", SEQ_, contents, _SEQ);
    return s;
}



char *sequence_tag_delayed(char *contents)
{
    const int len = 1 + strlen(SEQD_) + strlen(_SEQD);
    char *s = (char *) malloc(sizeof(char) * (strlen(contents) + len));
    sprintf(s, "%s%s%s", SEQD_, contents, _SEQD);
    return s;
}



void handle_command(char *name, char *args)
{
    printf("\n");

    #ifdef ENCLOSE_COMMAND_OUTPUT
        printf("\t>> ");
        p2_evaluate_command(name, args);
        printf(" <<");
    #else
        p2_evaluate_command(name, args);
    #endif

    printf("\n");
}

void handle_expression(char *name, char *expr)
{
    printf("\n");

    #ifdef ENCLOSE_EXPRESSION_OUTPUT
        printf("\t>> ");
        p2_evaluate_expression(name, expr);
        printf(" <<");
    #else
        p2_evaluate_expression(name, expr);
    #endif

    printf("\n");
}



%}

%token SEMICOLON EQUALS
%token OPEN_PAREN CLOSE_PAREN
%token OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET
%token OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET
%token NEWLINE

%union
{
    char *string;
}

%token <string> LITERAL
%token <string> IDENTIFIER
%token <string> COMMAND

%type <string> token term subterm expression parameter_list command

%%


parser_input:

    /* empty */
    {
        // Global variables could be initialized here.

        #ifdef INPUT_PREFIX
            printf(INPUT_PREFIX);  // Can the lexer do this?
        #else
            printf("\n");
            // printf("\t>> <<\n\n");
        #endif
    }
    | parser_input command
    {
        DEBUG_PRINTF("Found {parser_input command}.\n");
    }
    | parser_input expression
    {
        DEBUG_PRINTF("Found {parser_input expression}.\n");
    };


command:

    COMMAND SEMICOLON
    {
        char *s = sequence_tag("");
        DEBUG_PRINTF("Found {COMMAND ;}.\n");
        handle_command($1, s);
        free(s);
    }
    | COMMAND parameter_list SEMICOLON
    {
        char *s = sequence_tag($2);
        free($2);
        DEBUG_PRINTF("Found {COMMAND parameter_list;}.\n");
        handle_command($1, s);
        free(s);
    };



parameter_list:

    token
    {
        DEBUG_PRINTF("Found {token}.\n");
        $$ = token_tag($1);
        free($1);
    }
    | parameter_list token
    {
        char *t = token_tag($2);
        free($2);
        DEBUG_PRINTF("Found {parameter_list token}.\n");
        $$ = (char *) malloc(sizeof(char) * (1 + strlen($1) + strlen(t)));
        sprintf($$, "%s%s", $1, t);
        free($1); free(t);
    };



expression:

    term SEMICOLON
    {
        char *s = sequence_tag($1);
        DEBUG_PRINTF("Found {term;}.\n");
        free($1);
        handle_expression(0, s);
        free(s);
    }
    | term EQUALS token SEMICOLON
    {
        char *s = sequence_tag($1);
        DEBUG_PRINTF("Found {term;}.\n");
        free($1);
        handle_expression($3, s);
        free($3); free(s);
    };



term:

    subterm
    {
        $$ = $1;
    }
    | term subterm
    {
        DEBUG_PRINTF("Found {term subterm}.\n");
        $$ = (char *) malloc(sizeof(char) * (1 + strlen($1) + strlen($2)));
        sprintf($$, "%s%s", $1, $2);
        free($1); free($2);
    };



subterm:

    token
    {
        $$ = token_tag($1);
        free($1);
        DEBUG_PRINTF("Found {token}.\n");
    }
    | OPEN_PAREN term CLOSE_PAREN
    {
        DEBUG_PRINTF("Found {(term)}.\n");
        $$ = sequence_tag($2);
        free($2);
    }
    | OPEN_SQUARE_BRACKET term CLOSE_SQUARE_BRACKET
    {
        DEBUG_PRINTF("Found {(term)}.\n");
        $$ = sequence_tag_delayed($2);
        free($2);
    };



token:

    IDENTIFIER
    {
        $$ = $1;
    }
    | LITERAL
    {
        $$ = $1;
    };


