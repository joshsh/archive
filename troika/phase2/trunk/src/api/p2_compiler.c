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
    c->cur_ns__obj = env->data;
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


/******************************************************************************/


static int n_args( p2_ast *args, int n )
{
    int match = ( args )
        ? ( n == p2_ast__size( args ) )
        : !n;

    if ( !match )
        printf( "Error: command expects %i arguments.", n );

    return match;
}


static p2_ast *get_inner_node( p2_ast *ast )
{
    p2_term *term;

    #if DEBUG__SAFE
    if ( ast->type != TERM_T )
    {
        PRINTERR( "get_inner_node: wrong AST type" );
        return 0;
    }
    #endif

    term = ( p2_term* ) ast->value;

    if ( ( unsigned int ) *( term->head ) == 2 )
        /* Singleton term. */
        return ( p2_ast* ) *( term->head + 1 );
    else
        /* Left-associative sequence. */
        return ( p2_ast* ) *( term->head + 2 );
}


/* Command functions **********************************************************/


static void change_namespace( p2_compiler *c, p2_ast *args )
{
/*
printf( "args->type = %s\n", p2_ast__type__name( args->type ) ); fflush( stdout );
*/
    p2_object *o;
    p2_name *name;

    p2_ast *arg = get_inner_node( args );

/*
printf( "arg->type = %s\n", p2_ast__type__name( arg->type ) ); fflush( stdout );
*/
    if ( arg->type != NAME_T )
        printf( "Error: not a namespace." );
    else
    {
        name = ( p2_name* ) arg->value;

        if ( name->size == 1
          && !strcmp( "root", ( char* ) p2_array__peek( name ) ) )
        {
            c->cur_ns__obj = c->env->root;
            printf( "Moved to root namespace." );
        }

        else
        {
            o = p2_namespace__lookup( c->cur_ns__obj, name );
            if ( !o )
                printf( "Error: no such namespace." );
            else if ( o->type != c->cur_ns__obj->type )
                printf( "Error: not a namespace." );
            else
            {
                c->cur_ns__obj = o;
                printf( "Moved to namespace '" );
                p2_ast__print( arg );
                printf( "'." );
            }
        }
    }
}


static void garbage_collect( p2_compiler *c )
{
    p2_memory_manager *m = c->env->manager;
    int size_before, size_after;

printf( "---c gc 1---\n" ); fflush( stdout );

    size_before = p2_memory_manager__size( m );
    p2_memory_manager__mark_and_sweep( m );
    size_after = p2_memory_manager__size( m );
printf( "---c gc 2---\n" ); fflush( stdout );

    printf( "Collected %i of %i objects (%i%%).",
        size_before - size_after,
        size_before,
        ( ( size_before - size_after ) * 100 ) / size_before );
}


/* Externally linked functions for the parser *********************************/


int p2_compiler__evaluate_command( char *name, p2_ast *args )
{
    int ret = 0;

    if ( !strcmp( name, "gc" ) )
    {
        if ( n_args( args, 0 ) )
            garbage_collect( compiler );
    }

    else if ( !strcmp( name, "ns" ) )
    {
        if ( n_args( args, 1 ) )
            change_namespace( compiler, args );
    }

    else if ( !strcmp( name, "quit" ) )
    {
        if ( n_args( args, 0 ) )
            ret = 1;
    }

    else if ( !strcmp( name, "show" ) )
    {
        if ( n_args( args, 0 ) )
        {
            p2_namespace__show_children( compiler->cur_ns__obj );
/*
printf( "root:\n" );
p2_namespace__show_children( ( p2_namespace* ) compiler->env->root->value );
printf( "data:\n" );
p2_namespace__show_children( ( p2_namespace* ) compiler->env->data->value );
printf( "primitives:\n" );
p2_namespace__show_children( ( p2_namespace* ) compiler->env->primitives->value );
printf( "types:\n" );
p2_namespace__show_children( ( p2_namespace* ) compiler->env->types->value );
*/
        }
    }

    else if ( !strcmp( name, "size" ) )
    {
        if ( n_args( args, 0 ) )
            printf( "There are %i objects in this environment.",
                p2_memory_manager__size( compiler->env->manager ) );
    }

    else
        printf( "Error: unknown command." );

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
