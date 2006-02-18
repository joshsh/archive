#ifndef P2_AST_H
#define P2_AST_H


#include "../util/p2_array.h"
#include "../util/p2_term.h"


enum p2_ast__type
{
    BAG,  /**< A bag composed of terms. */
    NAME, /**< A URN-like sequence of name fragments. */
    TERM  /**< A term composed of p2_asts. */
};


/** The root node of a Phase2 abstract syntax tree. */
typedef struct _p2_ast
{
    enum p2_ast__type type;

    void *value;

} p2_ast;


/** \return  a non-zero value indicates an error */
int p2_ast__init();


/** \return  a non-zero value indicates an error */
int p2_ast__end();


/** \param bag  a p2_array of AST nodes
    \return  a new AST node of type BAG */
p2_ast *p2_ast__bag( p2_array *bag );


/** \param term  a p2_term of AST nodes
    \return  a new AST node of type TERM */
p2_ast *p2_ast__term( p2_term *term );


/** \param name  a p2_array of C strings representing a name
    \return  a new AST node of type NAME */
p2_ast *p2_ast__name( p2_array *name );


/** \param ast  AST node to free */
void *p2_ast__delete( p2_ast *ast );


/** \param ast  AST node to print */
void p2_ast__print( p2_ast *ast );


#endif  // P2_AST_H

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
