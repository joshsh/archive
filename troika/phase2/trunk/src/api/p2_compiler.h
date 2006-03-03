/**

\file  p2_compiler.h

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

#ifndef P2_COMPILER_H
#define P2_COMPILER_H


#include "p2_environment.h"
#include "parser/p2_ast.h"
#include "parser/p2_parser.h"


/** Bison parser dependency. */
extern int yyparse();


typedef struct _p2_compiler
{
    p2_environment *env;
    p2_namespace__object *cur_ns__obj;

    p2_hash_table *commands;

    int locked;

    p2_type *bag_t, *combinator_t, *char_t, *float_t, *int_t, *string_t, *term_t;

} p2_compiler;


/** Constructor.
    \param env  an environment for the compiler to act upon */
p2_compiler *p2_compiler__new( p2_environment *env );


/** Destructor. */
void p2_compiler__delete( p2_compiler *c );


/** yyparse is invoked here. */
p2_parser__exit_state p2_compiler__parse( p2_compiler *c );


/* Externally linked functions for the parser *********************************/


int p2_compiler__evaluate_command( char *name, p2_ast *args );

int p2_compiler__evaluate_expression( p2_name *name, p2_ast *expr );

int p2_compiler__handle_parse_error( char *msg );

/** \return  whether the lexer and parser are to avoid printing to stdout while
    matching input */
int p2_compiler__suppress_output();

/** \return  whether a line number is printed before each new line of input */
int p2_compiler__show_line_numbers();


#endif  /* P2_COMPILER_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
