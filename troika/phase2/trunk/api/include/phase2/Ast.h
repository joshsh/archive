/**

\file  Ast.h

\brief  Defines an abstract syntax tree.

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

#ifndef P2_AST_H
#define P2_AST_H


#include <util/Name.h>
#include <util/Array.h>
#include <util/Term.h>


enum p2_parser__exit_state
{
    exit_state__aborted,
    exit_state__end_of_input,
    exit_state__locked_out,
    exit_state__parse_failure

};

typedef enum p2_parser__exit_state p2_parser__exit_state;


enum Ast__Type
{
    BAG_T = 0,  /**< A bag composed of terms. */
    CHAR_T,
    FLOAT_T,
    INT_T,
    NAME_T,     /**< A URN-like sequence of name fragments. */
    NULL_T,
    STRING_T,
    TERM_T,     /**< A term composed of ASTs. */
    VOID_T      /**< A generic pointer to data not owned by the AST. */
};

typedef enum Ast__Type Ast__Type;


extern const char *Ast__Type__name( Ast__Type type );


typedef struct Ast Ast;

/** The root node of a Phase2 abstract syntax tree. */
struct Ast
{
    Ast__Type type;

    void *value;
};


/******************************************************************************/


/** \param bag  a Array of AST nodes
    \return  a new AST node of type BAG */
extern Ast *
ast__bag( Array *bag );

extern Ast *
ast__char( char c );

extern Ast *
ast__float( double f );

extern Ast *
ast__int( int i );

/** \param name  a Array of C strings representing a name
    \return  a new AST node of type NAME */
extern Ast *
ast__name( Name *name );

extern Ast *
ast__null();

extern Ast *
ast__string( char *s );

/** \param term  a Term of AST nodes
    \return  a new AST node of type TERM */
extern Ast *
ast__term( Term *term );


/******************************************************************************/


extern Ast *
ast__void( void *p );


extern int
ast__size( Ast *ast );


/** \param ast  AST node to free */
extern void
ast__delete( Ast *ast );


/** \param ast  AST node to print */
extern void
ast__print( Ast *ast );


#endif  /* P2_AST_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
