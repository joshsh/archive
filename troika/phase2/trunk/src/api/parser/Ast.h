/**

\file  Ast.h

\brief  Defines a Phase2 abstract syntax tree.

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

#ifndef P2_AST_H
#define P2_AST_H


#include <util/Name.h>
#include <util/Array.h>
#include <util/Term.h>


enum ast__type
{
    BAG_T = 0,  /**< A bag composed of terms. */
    CHAR_T,
    FLOAT_T,
    INT_T,
    NAME_T,     /**< A URN-like sequence of name fragments. */
    STRING_T,
    TERM_T,     /**< A term composed of Asts. */
    VOID_T      /**< A generic pointer to data not owned by the AST. */
};

typedef enum ast__type ast__type;


extern const char *ast__type__name( ast__type type );


typedef struct Ast Ast;

/** The root node of a Phase2 abstract syntax tree. */
struct Ast
{
    ast__type type;

    void *value;
};


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
ast__string( char *s );

/** \param term  a Term of AST nodes
    \return  a new AST node of type TERM */
extern Ast *
ast__term( Term *term );


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
