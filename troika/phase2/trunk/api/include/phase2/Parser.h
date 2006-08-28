/**

\file  Parser.h

\brief  A parser class to provide expressions and commands to an Interpreter.

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

#ifndef PARSER_H
#define PARSER_H


#include <Interpreter.h>


typedef struct Parser Parser;

typedef enum exit_state exit_state;

enum exit_state
{
    exit_state__aborted,
    exit_state__end_of_input,
    exit_state__locked_out,
    exit_state__parse_failure

};


/** \param itp  the Interpreter to which the Parser will pass the expressions
    it builds
    \return  a new Parser instance
    \note  the Parser does not own its Interpreter or vice versa, so they must
    be freed individually */
extern Parser *
parser__new( Interpreter *itp );

extern void
parser__free( Parser *p );

/** yyparse is invoked here. */
extern exit_state
parser__parse( Parser *p );

/** Passes text input to a Parser.
    \param s  a string to add to the parser buffer.  This does not need to be
    a complete expression in itself; consecutive calls to parser__feed build
    a series of expressions which are evaluated as they are completed. */
extern void
parser__feed( Parser *p, const char *s );


#endif  /* PARSER_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
