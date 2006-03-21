%{

/**

\file  bnftool.y

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


#include <util/Array.h>
#include <util/Dictionary.h>


extern void new_parse();
extern void end_parse();
extern int get_char_number();
extern int get_line_number();


#define DEBUG__LOGTOOL__PARSER  0

static void production( char *s )
{
    printf( "Matched %s\n", s );
}


Dictionary *dict;

static void register_failed_login( const char *ip )
{
    int attempts = ( int ) dictionary__lookup( dict, ip );
    dictionary__add( dict, ip, ( void* ) attempts + 1 );
}

static p2_action *print_offender( char *ip, Dictionary *d )
{
    printf( "%s (%i)\n", ip, ( int ) dictionary__lookup( dict, ip ) );
    return 0;
}


/******************************************************************************/


int yywrap()
{
    return 1;
}


void yyerror( const char *msg )
{
    printf( "line %d, char %d: %s\n",
        get_line_number(), get_char_number(), msg );
}


/******************************************************************************/


%}


%union
{
    char *string_t;
    int int_t;
}


%token SSHD PROC LOCALHOST
%token AUTH_FAIL CHECK_PASS SESSION_OPEN SESSION_CLOSE
%token COLON
%token NEWLINE E_O_F

%token <string_t> MONTH TIME NUMBER RHOST


/** Report more detailed parse error messages. */
%error-verbose

%start input


%%


input:

    messages E_O_F
    {
        p2_procedure proc;

        Array *keys = dictionary__keys( dict );
        proc.execute = ( procedure ) print_offender;
        proc.state = dict;
        array__distribute( keys, &proc );
        array__delete( keys );

        dictionary__delete( dict );
        YYACCEPT;
    }
    ;


messages:

    /* This production precedes input. */
    {
        dict = dictionary__new();
    }

    | messages message
    ;


message:

    month day time ip process NEWLINE
    ;


process:

    sshd
    | other_process
    ;


sshd:

    SSHD sshd_outcome
    ;


sshd_outcome:

    AUTH_FAIL RHOST /* L_PAREN STR R_PAREN L_SQ_BRACE INT R_SQ_BRACE */
    {
        register_failed_login( $2 );
        free( $2 );
    }

    | CHECK_PASS

    | SESSION_OPEN

    | SESSION_CLOSE
    ;


other_process:

    PROC
    ;


ip:

    LOCALHOST
    ;


month:

    MONTH
    {
        free( $1 );
    };


day:

    NUMBER
    {
        free( $1 );
    };


time:

    TIME
    {
        free( $1 );
    };


%%


int main()
{
    return yyparse();
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
