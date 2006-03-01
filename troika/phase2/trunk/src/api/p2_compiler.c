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

#include "p2_compiler.h"
#include "p2_name.h"
#include "parser/p2_ast.h"


/* Compiler object is global because flex/bison are not thread safe. */
p2_compiler *compiler = 0;


p2_compiler *p2_compiler__new( p2_environment *env )
{
    p2_compiler *c;

    if ( compiler )
    {
        PRINTERR( "p2_compiler__new: concurrent compiler instances not allowed" );
        return 0;
    }

    #if DEBUG__SAFE
    if ( !env )
    {
        PRINTERR( "p2_compiler__new: null environment" );
        return 0;
    }
    #endif

    if ( !( c = new( p2_compiler ) ) )
    {
        PRINTERR( "p2_compiler__new: allocation failed" );
        return 0;
    }

    if ( !( c->commands = p2_hash_table__new( 0, 0, 0, STRING_DEFAULTS ) ) )
    {
        PRINTERR( "p2_compiler__new: allocation failed" );
        free( c );
        return 0;
    }

    c->env = env;
    c->cur_ns = env->data;
    c->locked = 0;

    compiler = c;

    return c;
}


static void *p2_free( void *p )
{
    free( p );
    return p;
}


void p2_compiler__delete( p2_compiler *c )
{
    #if DEBUG__SAFE
    if ( !c )
    {
        PRINTERR( "p2_compiler__delete: null compiler" );
        return;
    }
    else if ( c->locked )
    {
        PRINTERR( "p2_compiler__delete: can't delete while parsing" );
        return;
    }
    #endif

    p2_hash_table__for_all_keys( c->commands, p2_free );
    p2_hash_table__delete( c->commands );
    free( c );

    compiler = 0;
}


p2_parser__exit_state p2_compiler__parse( p2_compiler *c )
{
    p2_parser__exit_state exit_state;

    #if DEBUG__SAFE
    if ( !c )
    {
        PRINTERR( "p2_compiler__parse: null compiler" );
        return 1;
    }
    #endif

    if ( c->locked )
        return exit_state__locked_out;

    c->locked = 1;

    if ( yyparse( &exit_state ) )
        PRINTERR( "p2_compiler__parse: parser exited abnormally" );

    c->locked = 0;

    return exit_state;
}


/* Externally linked functions for the parser *********************************/


int p2_compiler__evaluate_command( char *name, p2_ast *args )
{
    int ret = 0;

    if ( args )
    {
        printf( "Evaluate command \"%s\" :  ", name );
        p2_ast__print( args );
    }

    else
        printf( "Evaluate command \"%s\"", name );


    /* Debugger recognizes just one command. */
    if ( !strcmp( name, "exit" ) )
        ret = 1;

    if ( args )
        p2_ast__delete( args );

    free( name );

    return ret;
}


int p2_compiler__evaluate_expression( p2_name *name, p2_ast *expr )
{
    int ret = 0;
    p2_ast *a;

    if ( name )
    {
        a = p2_ast__name( name );
        printf( "Evaluate expression \"" );
        p2_ast__print( a );
        p2_ast__delete( a );
        printf( "\" :  ");
    }

    else
        printf( "Evaluate anonymous expression :  " );

    p2_ast__print( expr );
    p2_ast__delete( expr );

    return ret;
}


int p2_compiler__handle_parse_error( char *msg )
{
    int ret = 0;

    if ( msg )
    {
        printf( "Handle parse error :  %s", msg );
        free( msg );
    }

    else
        printf( "Handle parse error" );

    return ret;
}


int p2_compiler__suppress_output()
{
    return 0;
}


int p2_compiler__show_line_numbers()
{
    return 1;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
