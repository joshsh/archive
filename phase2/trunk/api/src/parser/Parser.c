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

#include "Parser-impl.h"


static boolean instance_exists = FALSE;


Parser *
parser__new( Interpreter *itp )
{
    Parser *p;
    Environment *e;

    if ( DEBUG__SAFE && !itp )
        abort();

    if ( instance_exists )
    {
        ERROR( "parser__new: concurrent parser instances are not allowed" );
        return 0;
    }

    p = NEW( Parser );
    if ( !p )
        return 0;

    p->interpreter = itp;

    e = interpreter__environment( itp );

#define BIND(x)    p->TYPEOBJ( x ) = environment__resolve_type( e, NAMEOF( x ), FALSE )

    if ( !(
        ( BIND( APPLY ) )
     && ( BIND( ARRAY ) )
     && ( BIND( CHARACTER ) )
     && ( BIND( DOUBLE ) )
     && ( BIND( INTEGER ) )
     && ( BIND( NAME ) )
     && ( BIND( STRING ) )
     && ( BIND( TERM ) ) ) )
    {
        free( p );
        return 0;
    }

#undef BIND

    p->manager = environment__manager( e );
    p->locked = FALSE;
    p->exit_early = FALSE;

    instance_exists = TRUE;

    return p;
}


void
parser__free( Parser *p )
{
    if ( DEBUG__SAFE && !p )
        abort();

    if ( p->locked )
    {
        ERROR( "parser__free: can't delete while parsing" );
        abort();
    }

    free( p );

    instance_exists = FALSE;
}


exit_state
parser__parse( Parser *p )
{
    exit_state exit_state;

    if ( DEBUG__SAFE && !p )
        abort();

    if ( p->locked )
        return exit_state__locked_out;

    p->locked = TRUE;

    if ( yyparse( p, &exit_state ) )
        ERROR( "parser__parse: parser exited abnormally" );

    p->locked = FALSE;

    return exit_state;
}


/******************************************************************************/


void
parser__handle_command( Parser *p, OBJ( STRING ) *name, OBJ( ARRAY ) *args )
{
    if ( !interpreter__quiet( p->interpreter ) )
    {
        PRINT( "\n" );

#ifdef COMMAND_OUTPUT_PREFIX
        PRINT( COMMAND_OUTPUT_PREFIX );
#endif
    }

    p->exit_early = p->exit_early || interpreter__evaluate_command( p->interpreter, name, args, lexer__get_buffer() );

    if ( !interpreter__quiet( p->interpreter ) )
    {
#ifdef COMMAND_OUTPUT_SUFFIX
        PRINT( COMMAND_OUTPUT_SUFFIX );
#endif
    }

    lexer__clear_buffer();
}


void
parser__handle_expression( Parser *p, OBJ( NAME ) *name, Object *expr )
{
    if ( !interpreter__quiet( p->interpreter ) )
    {
        PRINT( "\n" );

#ifdef EXPRESSION_OUTPUT_PREFIX
        PRINT( EXPRESSION_OUTPUT_PREFIX );
#endif
    }

    p->exit_early = p->exit_early || interpreter__evaluate_expression( p->interpreter, name, expr, lexer__get_buffer() );

    if ( !interpreter__quiet( p->interpreter ) )
    {
#ifdef EXPRESSION_OUTPUT_SUFFIX
        PRINT( EXPRESSION_OUTPUT_SUFFIX );
#endif
    }

    lexer__clear_buffer();
}


void
parser__handle_error( Parser *p, const char *msg )
{
    char error_msg[ ERROR_BUFFER__SIZE + 0x20 ];

    if ( !interpreter__quiet( p->interpreter ) )
    {
        PRINT( "\n" );

#ifdef ERROR_OUTPUT_PREFIX
        PRINT( ERROR_OUTPUT_PREFIX );
#endif
    }

    sprintf( error_msg, "line %d, column %d: %s",
        error_line_number, error_character_number, msg );

    p->exit_early = p->exit_early || interpreter__handle_parse_error( p->interpreter, error_msg );

    if ( !interpreter__quiet( p->interpreter ) )
    {
#ifdef ERROR_OUTPUT_SUFFIX
        PRINT( ERROR_OUTPUT_SUFFIX );
#endif
    }

    lexer__clear_buffer();
}


/******************************************************************************/


/* E.g.
extern Object *
parser__Array_ref_to_object( Parser *p, Array *ref )
{
    Object *o = manager__object( p->manager, object__value( p->ot_char ), ref, NOFLAGS );
    if ( o )
        return o;
    else
    {
        ( ( Type* ) object__value( p->ot_char ) )->destroy( ref );
        return 0;
    }
}
*/
#define PARSER_REF2OBJ_DEF0(x)  PARSER_REF2OBJ_DECL(x)\
{\
    Object *o = manager__object( p->manager, object__value( p->TYPEOBJ( x ) ), ref, NOFLAGS );\
    if ( o )\
        return o;\
    else\
    {\
        ( ( Type * ) object__value( p->TYPEOBJ( x ) ) )->destroy( ref );\
        return 0;\
    }\
}
#define PARSER_REF2OBJ_DEF(x)   PARSER_REF2OBJ_DEF0(x)

PARSER_REF2OBJ_DEF( APPLY )
PARSER_REF2OBJ_DEF( ARRAY )
PARSER_REF2OBJ_DEF( CHARACTER )
PARSER_REF2OBJ_DEF( DOUBLE )
PARSER_REF2OBJ_DEF( INTEGER )
PARSER_REF2OBJ_DEF( NAME )
PARSER_REF2OBJ_DEF( STRING )
PARSER_REF2OBJ_DEF( TERM )


/* E.g.
extern Object *
parser__int_val_to_object( Parser *p, int val )
{
    int *ref = malloc( sizeof ( int ) );
    if ( ref )
    {
        *ref = val;
        return parser__int_ref_to_object( p, ref );
    }

    else
        return 0;
}
*/
#define PARSER_VAL2OBJ_DEF0(x)  PARSER_VAL2OBJ_DECL(x)\
{\
    x *ref = NEW( x );\
    if ( ref )\
    {\
        *ref = val;\
        return PARSER_REF2OBJ(x)( p, ref );\
    }\
\
    else\
        return 0;\
}
#define PARSER_VAL2OBJ_DEF(x)   PARSER_VAL2OBJ_DEF0(x)

PARSER_VAL2OBJ_DEF( CHARACTER )
PARSER_VAL2OBJ_DEF( DOUBLE )
PARSER_VAL2OBJ_DEF( INTEGER )


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
