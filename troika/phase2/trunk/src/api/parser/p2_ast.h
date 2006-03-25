/**

\file  p2_ast.h

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


enum p2_ast__type
{
    BAG_T = 0,  /**< A bag composed of terms. */
    CHAR_T,
    FLOAT_T,
    INT_T,
    NAME_T,     /**< A URN-like sequence of name fragments. */
    STRING_T,
    TERM_T,     /**< A term composed of p2_asts. */
    VOID_T      /**< A generic pointer to data not owned by the AST. */
};

typedef enum p2_ast__type p2_ast__type;


extern const char *p2_ast__type__name( p2_ast__type type );


typedef struct p2_ast p2_ast;

/** The root node of a Phase2 abstract syntax tree. */
struct p2_ast
{
    p2_ast__type type;

    void *value;
};


/** \param bag  a Array of AST nodes
    \return  a new AST node of type BAG */
extern p2_ast *
p2_ast__bag( Array *bag );

extern p2_ast *
p2_ast__char( char c );

extern p2_ast *
p2_ast__float( double f );

extern p2_ast *
p2_ast__int( int i );

/** \param name  a Array of C strings representing a name
    \return  a new AST node of type NAME */
extern p2_ast *
p2_ast__name( Name *name );

extern p2_ast *
p2_ast__string( char *s );

/** \param term  a Term of AST nodes
    \return  a new AST node of type TERM */
extern p2_ast *
p2_ast__term( Term *term );


extern p2_ast *
p2_ast__void( void *p );


extern int
p2_ast__size( p2_ast *ast );


/** \param ast  AST node to free */
extern void
p2_ast__delete( p2_ast *ast );


/** \param ast  AST node to print */
extern void
p2_ast__print( p2_ast *ast );


#endif  /* P2_AST_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
