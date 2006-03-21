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


#include "bnftool.tab.h"

#include "../util/p2_array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  /* strdup */


int braces_level;


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


#define DEBUG__BNFTOOL__LEXER   1


#if DEBUG__BNFTOOL__LEXER
    #define TOKEN( x, y )  { new_token( yyleng ); \
                             if ( y ) printf( "Matched {%s}\n", y ); \
                             if ( x ) return x; }
#else
    #define TOKEN( x, y )  { new_token( yyleng ); \
                             if ( x ) return x; }
#endif


%}


NAMECHAR  [-_A-Za-z0-9]


%x SC_GRAMMAR SC_ACTION SC_COMMENT


%%


<INITIAL>
{
    "%%"  {  /* Warning: very touchy */
        p2_array__push( sc_stack, ( void* ) INITIAL );
        BEGIN( SC_GRAMMAR );
        TOKEN( 0, "grammar (begin)" );
    }

    \n  {
        new_line();
        TOKEN( 0, ( char* ) 0 );
    }

    <<EOF>>  {
        TOKEN( E_O_F, "EOF" );
    }

    .  {
        TOKEN( 0, ( char* ) 0 );
    }
}


<SC_GRAMMAR>
{
    ":"  {
        TOKEN( COLON, "COLON" );
    }

    ";"  {
        TOKEN( SEMICOLON, "SEMICOLON" );
    }

    "|"  {
        TOKEN( OR, "OR" );
    }

    "/*"  {
        p2_array__push( sc_stack, ( void* ) SC_GRAMMAR );
        BEGIN( SC_COMMENT );
        TOKEN( 0, "comment (begin)" );
    }

    "%%"  {
        p2_array__pop( sc_stack );
        BEGIN( INITIAL );
        TOKEN( 0, "grammar (end)" );
    }

    {NAMECHAR}+  {
        yylval.string_t = STRDUP( yytext );
        TOKEN( NAME, "NAME" );
    }

    "{" {
        braces_level = 1;
        p2_array__push( sc_stack, ( void* ) SC_GRAMMAR );
        BEGIN( SC_ACTION );
        TOKEN( 0, "action (begin)" );
    }

    \n  {
        new_line();
        TOKEN( 0, ( char* ) 0 );
    }

    [ \r\t]  {
        TOKEN( 0, ( char* ) 0 );
    }

    <<EOF>>  {
        TOKEN( E_O_F, "EOF" );
    }

    .  {
        TOKEN( 0, ( char* ) 0 );
    }
}


<SC_ACTION>
{
    \n  {
        new_line();
        TOKEN( 0, ( char* ) 0 );
    }

    "/*"  {
        p2_array__push( sc_stack, ( void* ) SC_ACTION );
        BEGIN( SC_COMMENT );
        TOKEN( 0, "comment (begin)" );
    }

    "{" {
        braces_level++;
        TOKEN( 0, ( char* ) 0 );
    }

    [^}]    /* Eat up action contents. */

    "}"  {
        if (! --braces_level )
        {
            BEGIN( ( int ) p2_array__pop( sc_stack ) );
            TOKEN( ACTION, "action (end)" );
        }

        else
            TOKEN( 0, ( char* ) 0 );
    }
}


<SC_COMMENT>
{
    "*/"  {
        BEGIN( ( int ) p2_array__pop( sc_stack ) );
        TOKEN( 0, "comment (end)" );
    }

    \n  {
        new_line();
        TOKEN( 0, ( char* ) 0 );
    }

    .  {  /* Ignore everything else. */
        TOKEN( 0, ( char* ) 0 );
    }
}


%%


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
