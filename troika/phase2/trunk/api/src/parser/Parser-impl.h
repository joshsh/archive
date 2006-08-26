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
#include <collection/Term.h>
#include "../compiler/Apply.h"
#include "../type/Type-impl.h"


/* Avoids C99 warning: implicit declaration of function ‘yylex’ */
extern int
yylex( void );


/* Lexer dependencies *********************************************************/


extern boolean pad_newline;

extern void
new_parse( Interpreter *c );

extern int
get_char_number( void );

extern int
get_line_number( void );


/******************************************************************************/


extern void
parser__handle_command( Parser *p, char *name, Object *args );

extern void
parser__handle_expression( Parser *p, Name *name, Object *expr );

extern void
parser__handle_error( Parser *p );


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
PARSER_REF2OBJ_DECL( BAG );
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
