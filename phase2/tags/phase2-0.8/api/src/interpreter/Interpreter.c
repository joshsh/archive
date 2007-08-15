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

#include "Interpreter-impl.h"


Interpreter *
interpreter__new( Environment *env, boolean quiet, boolean verbose )
{
    Interpreter *c;

    if ( DEBUG__SAFE && !env )
        ABORT;

    if ( !( c = NEW( Interpreter ) ) )
    {
        ERROR( "interpreter__new: allocation failed" );
        goto finish;
    }

    c->env = env;
    c->cur_ns_obj = environment__data( env );
    c->quiet = quiet;
    c->verbose = verbose;
    c->show_line_numbers = TRUE;
#if HAVE_LIBREADLINE
    c->use_readline = TRUE;
#else
    c->use_readline = FALSE;
#endif

    /* These basic types are indispensable for the compiler to communicate with
       the parser and with the SK module, and to serialize and deserialize data sets. */
    if ( !( ( c->apply_t = environment__resolve_type( env, NAMEOF( APPLY ), FALSE )->value )
         && ( c->bag_t = environment__resolve_type( env, NAMEOF( ARRAY ), FALSE )->value )
         && ( c->char_t = environment__resolve_type( env, NAMEOF( CHARACTER ), FALSE )->value )
         && ( c->combinator_t = environment__resolve_type( env, NAMEOF( COMBINATOR ), FALSE )->value )
         && ( c->float_t = environment__resolve_type( env, NAMEOF( DOUBLE ), FALSE )->value )
         && ( c->indirection_t = environment__resolve_type( env, NAMEOF( INDIRECTION ), FALSE )->value )
         && ( c->int_t = environment__resolve_type( env, NAMEOF( INTEGER ), FALSE )->value )
         && ( c->name_t = environment__resolve_type( env, NAMEOF( NAME ), FALSE  )->value )
         && ( c->ns_t = environment__resolve_type( env, NAMEOF( NAMESPACE ), FALSE )->value )
         && ( c->prim_t = environment__resolve_type( env, NAMEOF( PRIMITIVE ), FALSE )->value )
         && ( c->set_t = environment__resolve_type( env, NAMEOF( SET ), FALSE )->value )
         && ( c->string_t = environment__resolve_type( env, NAMEOF( STRING ), FALSE )->value )
         && ( c->term_t = environment__resolve_type( env, NAMEOF( TERM ), FALSE )->value )
         && ( c->type_t = environment__resolve_type( env, NAMEOF( TYPE ), FALSE )->value ) ) )
    {
        ERROR( "interpreter__new: basic type not found" );
        free( c );
        c = 0;
        goto finish;
    }

    if ( !( c->commands = create_commands() ) )
    {
        ERROR( "interpreter__new: allocation failed" );
        free( c );
        c = 0;
        goto finish;
    }

    graph_init( c->combinator_t, c->prim_t, c->apply_t, c->indirection_t );

finish:

    c->save_to_path = 0;

    return c;
}


void
interpreter__delete( Interpreter *itp )
{
    if ( DEBUG__SAFE && !itp )
        ABORT;

    if ( itp->save_to_path )
        free( itp->save_to_path );

    delete_commands( itp->commands );
    free( itp );
}


Environment *
interpreter__environment( Interpreter *c )
{
    return c->env;
}


OBJ( NAMESPACE ) *
interpreter__working_namespace( Interpreter *c )
{
    return c->cur_ns_obj;
}


/******************************************************************************/


boolean
interpreter__quiet( Interpreter *c )
{
    return c->quiet;
}


boolean
interpreter__show_line_numbers( Interpreter *c )
{
    return c->show_line_numbers;
}


/******************************************************************************/


#define ERRBUFSIZ  1000


void
err_notdefined( Name *name )
{
    char buff[ERRBUFSIZ];

    if ( DEBUG__SAFE && !name )
        ABORT;

    name__encode( name, buff );
    ERROR( "\"%s\" is not defined in this namespace", buff );
}


void
err_notns( Name *name )
{
    char buff[ERRBUFSIZ];

    if ( DEBUG__SAFE && !name )
        ABORT;

    name__encode( name, buff );
    ERROR( "\"%s\" is not a namespace", buff );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
