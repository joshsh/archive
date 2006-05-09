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

#include "Compiler-impl.h"


/** Bison parser dependency. */
extern int
yyparse( Compiler *c, p2_parser__exit_state *es );


/******************************************************************************/


static boolean instance_exists = FALSE;


Compiler *
compiler__new( Environment *env, boolean quiet )
{
    Compiler *c;

    if ( instance_exists )
    {
        ERROR( "compiler__new: concurrent compiler instances not allowed" );
        c = 0;
        goto finish;
    }

    if ( DEBUG__SAFE && !env )
        abort();

    if ( !( c = new( Compiler ) ) )
    {
        ERROR( "compiler__new: allocation failed" );
        goto finish;
    }

    instance_exists = TRUE;

    c->env = env;
    c->cur_ns_obj = environment__data( env );
    c->locked = FALSE;
    c->quiet = quiet;
    c->show_line_numbers = TRUE;

    /* These basic types are indispensable for the compiler to communicate with
       the parser and with the SK module, and to serialize and deserialize data sets. */
    if ( !( ( c->bag_t = environment__resolve_type( env, BAG__NAME )->value )
         && ( c->char_t = environment__resolve_type( env, CHAR__NAME )->value )
         && ( c->combinator_t = environment__resolve_type( env, COMBINATOR__NAME )->value )
         && ( c->float_t = environment__resolve_type( env, DOUBLE__NAME )->value )
         && ( c->int_t = environment__resolve_type( env, INT__NAME )->value )
         && ( c->ns_t = environment__resolve_type( env, NAMESPACE__NAME )->value )
         && ( c->prim_t = environment__resolve_type( env, PRIMITIVE__NAME )->value )
         && ( c->set_t = environment__resolve_type( env, SET__NAME )->value )
         && ( c->string_t = environment__resolve_type( env, STRING__NAME )->value )
         && ( c->term_t = environment__resolve_type( env, TERM__NAME )->value )
         && ( c->type_t = environment__resolve_type( env, TYPE__NAME )->value ) ) )
    {
        ERROR( "compiler__new: basic type not found" );
        free( c );
        instance_exists = FALSE;
        c = 0;
        goto finish;
    }

    if ( !( c->commands = create_commands() ) )
    {
        ERROR( "compiler__new: allocation failed" );
        free( c );
        instance_exists = FALSE;
        c = 0;
        goto finish;
    }

finish:

    c->save_to_path = 0;

    return c;
}


void
compiler__delete( Compiler *c )
{
    if ( DEBUG__SAFE )
    {
        if ( !c )
            abort();

        else if ( c->locked )
        {
            ERROR( "compiler__delete: can't delete while parsing" );
            abort();
        }
    }

    if ( c->save_to_path )
        free( c->save_to_path );

    delete_commands( c->commands );
    free( c );

    instance_exists = FALSE;
}


Environment *
compiler__environment( Compiler *c )
{
    return c->env;
}


Namespace_o *
compiler__working_namespace( Compiler *c )
{
    return c->cur_ns_obj;
}


p2_parser__exit_state
compiler__parse( Compiler *c )
{
    p2_parser__exit_state exit_state;

    if ( DEBUG__SAFE && !c )
        abort();

    #if DEBUG__COMPILER
    printf( "[...] compiler__parse(%#x)\n", ( int ) c );
    #endif

    if ( c->locked )
        return exit_state__locked_out;

    c->locked = TRUE;

    if ( yyparse( c, &exit_state ) )
        ERROR( "compiler__parse: parser exited abnormally" );

    c->locked = FALSE;

    return exit_state;
}


/******************************************************************************/


boolean
compiler__quiet( Compiler *c )
{
    return c->quiet;
}


boolean
compiler__show_line_numbers( Compiler *c )
{
    return c->show_line_numbers;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
