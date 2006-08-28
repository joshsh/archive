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

#ifndef PARSER_IMPL_H
#define PARSER_IMPL_H


#include <Parser.h>
#include <collection/Name.h>
#include <collection/Array.h>
#include "../compiler/Apply.h"
#include "../type/Type-impl.h"


#define TYPEOBJ0(x) ot_##x
#define TYPEOBJ(x)  TYPEOBJ0(x)

/** Buffer size is arbitary... */
#define ERROR_BUFFER__SIZE  0xFF

/** Output decoration. */
/* #define COMMAND_OUTPUT_PREFIX "\t>> "    */
/* #define EXPRESSION_OUTPUT_PREFIX "\t>> " */
/* #define ERROR_OUTPUT_PREFIX "\t>> "      */
/* #define COMMAND_OUTPUT_SUFFIX " <<"      */
/* #define EXPRESSION_OUTPUT_SUFFIX " <<"   */
/* #define ERROR_OUTPUT_SUFFIX " <<"        */


struct Parser
{
    Object
        *TYPEOBJ( APPLY ),
        *TYPEOBJ( ARRAY ),
        *TYPEOBJ( CHARACTER ),
        *TYPEOBJ( DOUBLE ),
        *TYPEOBJ( INTEGER ),
        *TYPEOBJ( NAME ),
        *TYPEOBJ( STRING ),
        *TYPEOBJ( TERM );

    Interpreter *interpreter;

    /* Shortcut to the Interpreter's Environment's Memory_Manager. */
    Memory_Manager *manager;

    boolean locked;
    boolean exit_early;
};


/******************************************************************************/


/* Avoids C99 warning: implicit declaration of function ‘yylex’ */
extern int
yylex( void );

/** Bison parser dependency. */
extern int
yyparse( Parser *p, exit_state *es );


/* Lexer dependencies *********************************************************/


extern void
lexer__clear_buffer();

extern char *
lexer__get_buffer();

extern boolean pad_newline;

extern void
new_parse( Parser *p );

extern int
get_char_number( void );

extern int
get_line_number( void );


/******************************************************************************/


extern int error_character_number, error_line_number;

extern void
parser__handle_command( Parser *p, OBJ( STRING ) *name, OBJ( ARRAY ) *args );

extern void
parser__handle_expression( Parser *p, OBJ( NAME ) *name, Object *expr );

extern void
parser__handle_error( Parser *p, const char *msg );


/******************************************************************************/

/* E.g.
parser__Array_ref_to_object
*/
#define PARSER_REF2OBJ0(x)       parser__##x##_ref_to_object
#define PARSER_REF2OBJ(x)        PARSER_REF2OBJ0(x)

/* E.g.
parser__int_val_to_object
*/
#define PARSER_VAL2OBJ0(x)       parser__##x##_val_to_object
#define PARSER_VAL2OBJ(x)        PARSER_VAL2OBJ0(x)

/* E.g.
extern Object *
parser__Array_ref_to_object( Parser *p, Array *ref ) */
#define PARSER_REF2OBJ_DECL0(x)  extern Object *\
PARSER_REF2OBJ(x)( Parser *p, x *ref )
#define PARSER_REF2OBJ_DECL(x)  PARSER_REF2OBJ_DECL0(x)

PARSER_REF2OBJ_DECL( APPLY );
PARSER_REF2OBJ_DECL( ARRAY );
PARSER_REF2OBJ_DECL( CHARACTER );
PARSER_REF2OBJ_DECL( DOUBLE );
PARSER_REF2OBJ_DECL( INTEGER );
PARSER_REF2OBJ_DECL( NAME );
PARSER_REF2OBJ_DECL( STRING );
PARSER_REF2OBJ_DECL( TERM );

/* E.g.
extern Object *
parser__Array_ref_to_object( Parser *p, Array *ref ) */
#define PARSER_VAL2OBJ_DECL0(x) extern Object *\
PARSER_VAL2OBJ(x)( Parser *p, x val )
#define PARSER_VAL2OBJ_DECL(x)  PARSER_VAL2OBJ_DECL0(x)

PARSER_VAL2OBJ_DECL( CHARACTER );
PARSER_VAL2OBJ_DECL( DOUBLE );
PARSER_VAL2OBJ_DECL( INTEGER );


#endif  /* PARSER_IMPL_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
