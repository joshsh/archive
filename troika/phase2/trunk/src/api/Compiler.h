/**

\file  Compiler.h

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

#ifndef COMPILER_H
#define COMPILER_H


#include <Environment.h>
#include <parser/p2_ast.h>
#include <parser/p2_parser.h>
#include <util/Dictionary.h>


/** Bison parser dependency. */
extern int yyparse();


typedef struct Compiler Compiler;

struct Compiler
{
    Environment *env;
    Namespace_o *cur_ns_obj;

    Dictionary *commands;

    int locked;

    /* Parser types. */
    Type *bag_t, *char_t, *float_t, *int_t, *string_t, *term_t;

    boolean suppress_output, show_line_numbers;
};


/** Constructor.
    \param env  an environment for the compiler to act upon */
extern Compiler *compiler__new( Environment *env );


/** Destructor. */
extern void compiler__delete( Compiler *c );


/** yyparse is invoked here. */
extern p2_parser__exit_state compiler__parse( Compiler *c );


/* Externally linked functions for the parser *********************************/


extern int compiler__evaluate_command( char *name, p2_ast *args );

extern int compiler__evaluate_expression( Name *name, p2_ast *expr );

extern int compiler__handle_parse_error( char *msg );

/** \return  whether the lexer and parser are to avoid printing to stdout while
    matching input */
extern int compiler__suppress_output();

/** \return  whether a line number is printed before each new line of input */
extern int compiler__show_line_numbers();


#endif  /* COMPILER_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
