/**

\file  Interpreter.h

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

#ifndef INTERPRETER_H
#define INTERPRETER_H


#include <Environment.h>
#include <Ast.h>
#include <collection/Dictionary.h>
#include <collection/Name.h>


/** An object which serves as a user interface to a Phase2 environment. */
typedef struct Interpreter Interpreter;


/** Constructor.
    \param env  an environment for the compiler to act upon */
extern Interpreter *
interpreter__new( Environment *env, boolean quiet );


/** Destructor. */
extern void
interpreter__delete( Interpreter *c );


extern Environment *
interpreter__environment( Interpreter *c );


extern Namespace_o *
interpreter__working_namespace( Interpreter *c );


/** yyparse is invoked here. */
extern p2_parser__exit_state
interpreter__parse( Interpreter *c );


/******************************************************************************/

extern Name *
interpreter__name_of( Interpreter *c, Namespace_o *nso, Object *o );

extern Name *
interpreter__name_of__full( Interpreter *c, Namespace_o *nso, Object *o );


/******************************************************************************/


extern void
interpreter__serialize( Interpreter *c, char *path );

extern void
interpreter__deserialize( Interpreter *c, char *path );


/* Externally linked functions for the parser *********************************/


extern int
interpreter__evaluate_command( Interpreter *c, char *name, Ast *args );

extern int
interpreter__evaluate_expression( Interpreter *c, Name *name, Ast *expr );

/** \note  Error messages are not suppressed by --quiet */
extern int
interpreter__handle_parse_error( Interpreter *c, char *msg );

/** \return  whether the lexer and parser are to avoid printing to stdout while
    matching input */
extern boolean
interpreter__quiet( Interpreter *c );

/** \return  whether a line number is printed before each new line of input */
extern boolean
interpreter__show_line_numbers( Interpreter *c );


#endif  /* INTERPRETER_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
