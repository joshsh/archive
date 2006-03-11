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


/* Compiler object is global because flex/bison are not thread safe. */
p2_compiler *compiler = 0;


/* Find a data type in the compiler environment's "types" namespace. */
static p2_type *lookup_type( p2_environment *env, const char *name )
{
    p2_object *o = p2_namespace__lookup_simple( env->types, name );

    if ( !o )
        return 0;

    #if DEBUG__SAFE
    if ( o->type != env->type_t )
    {
        ERROR( "lookup_type: type mismatch" );
        return 0;
    }
    #endif

    return ( p2_type* ) o->value;
}


p2_compiler *p2_compiler__new( p2_environment *env )
{
    p2_compiler *c;

    if ( compiler )
    {
        ERROR( "p2_compiler__new: concurrent compiler instances not allowed" );
        return 0;
    }

    #if DEBUG__SAFE
    if ( !env )
    {
        ERROR( "p2_compiler__new: null environment" );
        return 0;
    }
    #endif

    if ( !( c = new( p2_compiler ) ) )
    {
        ERROR( "p2_compiler__new: allocation failed" );
        return 0;
    }

    /* These basic types are indispensable for the compiler to communicate with
       the parser. */
    if ( !( c->bag_t = lookup_type( env, "bag" ) )
      || !( c->char_t = lookup_type( env, "char" ) )
      || !( c->combinator_t = lookup_type( env, "combinator" ) )
      || !( c->float_t = lookup_type( env, "double" ) )
      || !( c->int_t = lookup_type( env, "int" ) )
      || !( c->string_t = lookup_type( env, "cstring" ) )
      || !( c->term_t = lookup_type( env, "term" ) ) )
    {
        ERROR( "p2_compiler__new: basic type not found" );
        free( c );
        return 0;
    }

    if ( !( c->commands = p2_dictionary__new() ) )
    {
        ERROR( "p2_compiler__new: allocation failed" );
        free( c );
        return 0;
    }

    c->env = env;
    c->cur_ns_obj = env->data;
    c->locked = 0;
    c->suppress_output = boolean__false;
    c->show_line_numbers = boolean__true;

    compiler = c;

    return c;
}


void p2_compiler__delete( p2_compiler *c )
{
    #if DEBUG__SAFE
    if ( !c )
    {
        ERROR( "p2_compiler__delete: null compiler" );
        return;
    }
    else if ( c->locked )
    {
        ERROR( "p2_compiler__delete: can't delete while parsing" );
        return;
    }
    #endif

    p2_dictionary__delete( c->commands );
    free( c );

    compiler = 0;
}


p2_parser__exit_state p2_compiler__parse( p2_compiler *c )
{
    p2_parser__exit_state exit_state;

    #if DEBUG__SAFE
    if ( !c )
    {
        ERROR( "p2_compiler__parse: null compiler" );
        return 1;
    }
    #endif

    if ( c->locked )
        return exit_state__locked_out;

    c->locked = 1;

    if ( yyparse( &exit_state ) )
        ERROR( "p2_compiler__parse: parser exited abnormally" );

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
        ERROR( "get_inner_node: wrong AST type" );
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


static p2_object *resolve_name( p2_compiler *c, p2_name *name )
{
    p2_namespace_o *ns_obj;
    p2_object *o;

    char *first = ( char* ) p2_name__pop( name );
    if ( !strcmp( first, "root" ) )
    {
        ns_obj = c->env->root;
        o = p2_namespace__lookup( ns_obj, name );
        p2_name__push( name, first );
    }

    else
    {
        ns_obj = c->cur_ns_obj;
        p2_name__push( name, first );
        o = p2_namespace__lookup( ns_obj, name );
    }

    if ( !o )
    {
        printf( "Error: '" );
        p2_name__print( name );
        printf( "' is not defined in this namespace.\n" );
    }

    return o;
}


typedef struct _subst_st
{
    p2_action *action;
    p2_procedure *subst_proc;
    boolean sofarsogood;

} subst_st;


static p2_object *object_for_ast( p2_ast* ast, subst_st *state );

static p2_action * substitute_object_for_ast( p2_ast *ast, subst_st *state )
{
    p2_object *o = object_for_ast( ast, state );
    if ( !o )
        state->sofarsogood = boolean__false;

    state->action->value = 0;
    return state->action;
}


/* Transforms a p2_ast into a p2_object, deleting the p2_ast along the way. */
static p2_object *object_for_ast( p2_ast* ast, subst_st *state )
{
    p2_object *o;
    p2_type *type;
    void *value;
    int flags = 0;

    switch ( ast->type )
    {
        case BAG_T:

            type = compiler->bag_t;
            value = ast->value;
            p2_array__distribute( ( p2_array* ) value, state->subst_proc );
            flags = OBJECT__IS_OBJ_COLL;
            break;

        case CHAR_T:

            type = compiler->char_t;
            value = ast->value;
            break;

        case FLOAT_T:

            type = compiler->float_t;
            value = ast->value;
            break;

        case INT_T:

            type = compiler->int_t;
            value = ast->value;
            break;

        case NAME_T:

            /* Retrieve an existing object and exit. */
            o = resolve_name( compiler, ( p2_name* ) ast->value );
            p2_ast__delete( ast );
            return o;

        case STRING_T:

            type = compiler->string_t;
            value = ast->value;
            break;

        case TERM_T:

            type = compiler->term_t;
            value = ast->value;
            p2_term__distribute( ( p2_term* ) value, state->subst_proc );
            flags = OBJECT__IS_OBJ_COLL;
            break;

        #if DEBUG__SAFE
        default:

            ERROR( "object_for_ast: bad AST type" );
            free( ast );
            return 0;
        #endif
    }

    free( ast );

    /* Create and register a new object. */
    o = p2_object__new( type, value, flags );
    p2_memory_manager__add( compiler->env->manager, o );

    return o;
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
    #if DEBUG__SAFE
    if ( arg->type != NAME_T )
    {
        ERROR( "change_namespace: AST type mismatch" );
        return;
    }
    #endif

    name = ( p2_name* ) arg->value;

    if ( ( o = resolve_name( c, name ) ) )
    {
        if ( o->type != c->cur_ns_obj->type )
            printf( "Error: not a namespace.\n" );

        else
        {
            c->cur_ns_obj = o;
            printf( "Moved to namespace '" );
            p2_ast__print( arg );
            printf( "'." );
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

    printf( "Collected %i of %i objects (%.3g%%).",
        size_before - size_after,
        size_before,
        ( ( size_before - size_after ) * 100 ) / ( double ) size_before );
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
            p2_namespace__show_children( compiler->cur_ns_obj );
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
    subst_st state;
    p2_action action;
    p2_procedure subst_proc;

    int ret = 0;
    p2_ast *a = 0;
    p2_object *o;
    char print_buffer[1000];

    if ( name )
        a = p2_ast__name( name );

    #if DEBUG__SAFE
    if ( !expr )
    {
        ERROR( "p2_compiler__evaluate_expression: null AST node" );
        if ( a )
            p2_ast__delete( a );
        return 0;
    }
    #endif


    state.sofarsogood = boolean__true;

    action.type = p2_action__type__replace;
    state.action = &action;

    subst_proc.execute = ( procedure ) substitute_object_for_ast;
    subst_proc.state = &state;
    state.subst_proc = &subst_proc;


printf( "\n" );
    o = object_for_ast( expr, &state );

    printf( "%#x : %s ", ( int ) o, o->type->name );
    if ( a )
    {
        p2_ast__print( a );
        p2_ast__delete( a );
    }
    else
        printf( "[]" );
    printf( " = " );

    if ( !state.sofarsogood )
        printf( "*** Error in term. ***" );

    else
    {
        o->type->encode( o->value, print_buffer );
        printf( print_buffer );
    }

    printf( "\n" );

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
    return compiler->suppress_output;
}


int p2_compiler__show_line_numbers()
{
    return compiler->show_line_numbers;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
