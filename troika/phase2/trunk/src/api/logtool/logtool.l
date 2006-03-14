%{

/**

\file  bnftool.l

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


#include "logtool.tab.h"

#include <util/p2_array.h>


/* Line and character numbers *************************************************/


int line_number, column_number, last_column_number, statement_number;


void new_token( int length )
{
    last_column_number = column_number;
    column_number += length;
}


void new_line()
{
    line_number++;
    column_number = last_column_number = 0;
}


p2_array *sc_stack;


/** Initialize the lexer for a new invocation of yyparse(). */
void new_parse()
{
    line_number = 0;
    new_line();

    sc_stack = p2_array__new( 0, 0 );
}


void end_parse()
{
    p2_array__delete( sc_stack );
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


/*******************************************************************************/


#define DEBUG__BNFTOOL__LEXER   0


#if DEBUG__BNFTOOL__LEXER
    #define TOKEN( x, y )  { new_token( yyleng ); \
                             if ( y ) printf( "Matched {%s}\n", y ); \
                             if ( x ) return x; }
#else
    #define TOKEN( x, y )  { new_token( yyleng ); \
                             if ( x ) return x; }
#endif


%}


%x SC_PROCS SC_SSHD SC_OTHER

/*
    "("  {
        TOKEN( L_SQ_BRACE, "L_PAREN" );
    }

    ")"  {
        TOKEN( L_SQ_BRACE, "R_PAREN" );
    }

    "["  {
        TOKEN( L_SQ_BRACE, "L_SQ_BRACE" );
    }

    "]"  {
        TOKEN( L_SQ_BRACE, "R_SQ_BRACE" );
    }
*/


%%


<INITIAL>
{
    <<EOF>>  {
        TOKEN( E_O_F, "EOF" );
    }

    [0-9]+  {
        yylval.string_t = STRDUP( yytext );
        TOKEN( NUMBER, "NUMBER" );
    }

    [0-9][0-9]:[0-9][0-9]:[0-9][0-9]  {
        yylval.string_t = STRDUP( yytext );
        TOKEN( TIME, "TIME" );
    }

    "Mar"  {
        yylval.string_t = STRDUP( yytext );
        TOKEN( MONTH, "MONTH" );
    }

    "localhost"  {
        BEGIN( SC_PROCS );
        TOKEN( LOCALHOST, "LOCALHOST" );
    }

    .  {
        TOKEN( 0, ( char* ) 0 );
    }
}


<SC_PROCS>
{
    :  {
        BEGIN( SC_OTHER );
        TOKEN( PROC, "PROC" );
    }

    "sshd"  {
        BEGIN( SC_SSHD );
        TOKEN( SSHD, "SSHD" );
    }

    .  {
        TOKEN( 0, ( char* ) 0 );
    }
}


<SC_SSHD>
{
    "check pass;"  {
        TOKEN( CHECK_PASS, "CHECK_PASS" );
    }

    "session opened"  {
        TOKEN( SESSION_OPEN, "SESSION_OPEN" );
    }

    "session closed"  {
        TOKEN( SESSION_CLOSE, "SESSION_CLOSE" );
    }

    "authentication failure;"  {
        TOKEN( AUTH_FAIL, "AUTH_FAIL" );
    }

    "rhost="[^\n[:space:]]+  {
        yylval.string_t = STRDUP( yytext + 6 );
        TOKEN( RHOST, "RHOST" );
    }

    \n  {
        new_line();
        BEGIN( INITIAL );
        TOKEN( NEWLINE, "NEWLINE" );
    }

    <<EOF>>  {
        TOKEN( E_O_F, "EOF" );
    }

    .
}


<SC_OTHER>
{
    \n  {
        new_line();
        BEGIN( INITIAL );
        TOKEN( NEWLINE, "NEWLINE" );
    }

    <<EOF>>  {
        TOKEN( E_O_F, "EOF" );
    }

    .  {
        TOKEN( 0, ( char* ) 0 );
    }
}


%%


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
