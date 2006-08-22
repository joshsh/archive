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


/** Bison parser dependency. */
extern int
yyparse( Interpreter *c, exit_state *es );


/******************************************************************************/


static boolean instance_exists = FALSE;


Interpreter *
interpreter__new( Environment *env, boolean quiet )
{
    Interpreter *c;

    if ( instance_exists )
    {
        ERROR( "interpreter__new: concurrent interpreter instances are not allowed" );
        c = 0;
        goto finish;
    }

    if ( DEBUG__SAFE && !env )
        abort();

    if ( !( c = NEW( Interpreter ) ) )
    {
        ERROR( "interpreter__new: allocation failed" );
        goto finish;
    }

    instance_exists = TRUE;

    c->env = env;
    c->cur_ns_obj = environment__data( env );
    c->locked = FALSE;
    c->quiet = quiet;
    c->show_line_numbers = TRUE;
#if HAVE_LIBREADLINE
    c->use_readline = TRUE;
#else
    c->use_readline = FALSE;
#endif

    /* These basic types are indispensable for the compiler to communicate with
       the parser and with the SK module, and to serialize and deserialize data sets. */
    if ( !( ( c->bag_t = environment__resolve_type( env, NAMEOF( BAG ) )->value )
         && ( c->char_t = environment__resolve_type( env, NAMEOF( CHARACTER ) )->value )
         && ( c->combinator_t = environment__resolve_type( env, NAMEOF( COMBINATOR ) )->value )
         && ( c->float_t = environment__resolve_type( env, NAMEOF( DOUBLE ) )->value )
         && ( c->int_t = environment__resolve_type( env, NAMEOF( INTEGER ) )->value )
         && ( c->ns_t = environment__resolve_type( env, NAMEOF( NAMESPACE ) )->value )
         && ( c->prim_t = environment__resolve_type( env, NAMEOF( PRIMITIVE ) )->value )
         && ( c->set_t = environment__resolve_type( env, NAMEOF( SET ) )->value )
         && ( c->string_t = environment__resolve_type( env, NAMEOF( STRING ) )->value )
         && ( c->term_t = environment__resolve_type( env, NAMEOF( TERM ) )->value )
         && ( c->type_t = environment__resolve_type( env, NAMEOF( TYPE ) )->value ) ) )
    {
        ERROR( "interpreter__new: basic type not found" );
        free( c );
        instance_exists = FALSE;
        c = 0;
        goto finish;
    }

    if ( !( c->commands = create_commands() ) )
    {
        ERROR( "interpreter__new: allocation failed" );
        free( c );
        instance_exists = FALSE;
        c = 0;
        goto finish;
    }

    /* FIXME */
    graph_init( c->combinator_t, c->prim_t );

finish:

    c->save_to_path = 0;

    return c;
}


void
interpreter__delete( Interpreter *c )
{
    if ( DEBUG__SAFE )
    {
        if ( !c )
            abort();

        else if ( c->locked )
        {
            ERROR( "interpreter__delete: can't delete while parsing" );
            abort();
        }
    }

    if ( c->save_to_path )
        free( c->save_to_path );

    delete_commands( c->commands );
    free( c );

    instance_exists = FALSE;

    /* FIXME */
    graph_end();
}


Environment *
interpreter__environment( Interpreter *c )
{
    return c->env;
}


Namespace_o *
interpreter__working_namespace( Interpreter *c )
{
    return c->cur_ns_obj;
}


exit_state
interpreter__parse( Interpreter *c )
{
    exit_state exit_state;

    if ( DEBUG__SAFE && !c )
        abort();

    if ( c->locked )
        return exit_state__locked_out;

    c->locked = TRUE;

    if ( yyparse( c, &exit_state ) )
        ERROR( "interpreter__parse: parser exited abnormally" );

    c->locked = FALSE;

    return exit_state;
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
        abort();

    name__encode( name, buff );
    ERROR( "\"%s\" is not defined in this namespace", buff );
}


void
err_notns( Name *name )
{
    char buff[ERRBUFSIZ];

    if ( DEBUG__SAFE && !name )
        abort();

    name__encode( name, buff );
    ERROR( "\"%s\" is not a namespace", buff );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
