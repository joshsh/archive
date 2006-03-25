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

#include <Compiler.h>
#include <Closure.h>
#include <serial.h>
#include <sk/sk.h>

#include <time.h>


struct Compiler
{
    Environment *env;
    Namespace_o *cur_ns_obj;

    Dictionary *commands;

    int locked;

    boolean suppress_output, show_line_numbers;
};


/* Compiler object is global because flex/bison are not thread safe. */
static Compiler *compiler = 0;


/******************************************************************************/


static void
char__encode__alt( char *c, char *buffer )
{
    sprintf( buffer, "'%c'", *c );
}


static void
double__encode__alt( double *d, char *buffer )
{
    if ( *d - ( double ) ( ( int ) *d ) )
        sprintf( buffer, "%g", *d );
    else
        sprintf( buffer, "%g.0", *d );
}


static void
string__encode__alt( char *s, char *buffer )
{
    sprintf( buffer, "\"%s\"", s );
}


static void
term__encode__alt( Term *t, char *buffer )
{
    sprintf( buffer, "[ " );
    buffer += 2;

    term__encode( t, buffer );

    buffer += strlen( buffer );
    sprintf( buffer, " ]" );
}


/******************************************************************************/


Compiler *
compiler__new( Environment *env )
{
    Compiler *c;

    if ( compiler )
    {
        ERROR( "compiler__new: concurrent compiler instances not allowed" );
        return 0;
    }

    #if DEBUG__SAFE
    if ( !env )
    {
        ERROR( "compiler__new: null environment" );
        return 0;
    }
    #endif

    if ( !( c = new( Compiler ) ) )
    {
        ERROR( "compiler__new: allocation failed" );
        return 0;
    }

    #if DEBUG__COMPILER
    printf( "[%#x] compiler__new(%#x)\n", ( int ) c, ( int ) env );
    #endif

    c->env = env;
    c->cur_ns_obj = env->data;
    c->locked = 0;
    c->suppress_output = FALSE;
    c->show_line_numbers = TRUE;

    /* These basic types are indispensable for the compiler to communicate with
       the parser. */
    if ( !( env->bag_t = environment__resolve_type( env, "bag" ) )
      || !( env->char_t = environment__resolve_type( env, "char" ) )
      || !( env->string_t = environment__resolve_type( env, "cstring" ) )
      || !( env->float_t = environment__resolve_type( env, "double" ) )
      || !( env->int_t = environment__resolve_type( env, "int" ) )
      || !( env->term_t = environment__resolve_type( env, "term" ) ) )
    {
        ERROR( "compiler__new: basic type not found" );
        free( c );
        return 0;
    }

    if ( !( c->commands = dictionary__new() ) )
    {
        ERROR( "compiler__new: allocation failed" );
        free( c );
        return 0;
    }

    compiler = c;

    return c;
}


void
compiler__delete( Compiler *c )
{
    #if DEBUG__SAFE
    if ( !c )
    {
        ERROR( "compiler__delete: null compiler" );
        return;
    }
    else if ( c->locked )
    {
        ERROR( "compiler__delete: can't delete while parsing" );
        return;
    }
    #endif

    #if DEBUG__COMPILER
    printf( "[] compiler__delete(%#x)\n", ( int ) c );
    #endif

    dictionary__delete( c->commands );
    free( c );

    compiler = 0;
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

    #if DEBUG__SAFE
    if ( !c )
    {
        ERROR( "compiler__parse: null compiler" );
        return 1;
    }
    #endif

    #if DEBUG__COMPILER
    printf( "[...] compiler__parse(%#x)\n", ( int ) c );
    #endif

    if ( c->locked )
        return exit_state__locked_out;

    c->locked = 1;

    if ( yyparse( &exit_state ) )
        ERROR( "compiler__parse: parser exited abnormally" );

    c->locked = 0;

    return exit_state;
}


/******************************************************************************/


static int
n_args( p2_ast *args, int n )
{
    int match = ( args )
        ? ( n == p2_ast__size( args ) )
        : !n;

    if ( !match )
        printf( "Error: command expects %i arguments.", n );

    return match;
}


static p2_ast *
get_inner_node( p2_ast *ast )
{
    Term *term;

    #if DEBUG__SAFE
    if ( ast->type != TERM_T )
    {
        ERROR( "get_inner_node: wrong AST type" );
        return 0;
    }
    #endif

    term = ( Term* ) ast->value;

    if ( ( unsigned int ) *( term->head ) == 2 )
        /* Singleton term. */
        return ( p2_ast* ) *( term->head + 1 );
    else
        /* Left-associative sequence. */
        return ( p2_ast* ) *( term->head + 2 );
}


static Object *
assign_name( Compiler *c, Name *name, Object *o )
{
    Namespace_o *ns_obj;

    char *first = ( char* ) name__pop( name );
    if ( !strcmp( first, "root" ) )
    {
        ns_obj = c->env->root;
        o = namespace__add( ns_obj, name, o );
        name__push( name, first );
    }

    else
    {
        ns_obj = c->cur_ns_obj;
        name__push( name, first );
        o = namespace__add( ns_obj, name, o );
    }

    return o;
}


static Object *
resolve_name( Compiler *c, Name *name )
{
    Namespace_o *ns_obj;
    Object *o;

    char *first = ( char* ) name__pop( name );
    if ( !strcmp( first, "root" ) )
    {
        ns_obj = c->env->root;
        o = namespace__lookup( ns_obj, name );
        name__push( name, first );
    }

    else
    {
        ns_obj = c->cur_ns_obj;
        name__push( name, first );
        o = namespace__lookup( ns_obj, name );
    }

    if ( !o )
    {
        printf( "Error: '" );
        name__print( name );
        printf( "' is not defined in this namespace.\n" );
    }

    return o;
}


typedef struct Subst_Ctx
{
    Closure *subst_proc;
    boolean sofarsogood;

} Subst_Ctx;


static Object *
object_for_ast( p2_ast* ast, Subst_Ctx *state );

static void *
substitute_object_for_ast( p2_ast **ast_p, Subst_Ctx *state )
{
    Object *o = object_for_ast( *ast_p, state );
    if ( !o )
        state->sofarsogood = FALSE;
    *ast_p = ( p2_ast* ) o;

    return 0;
}


/* Transforms a p2_ast into a Object, deleting the p2_ast along the way. */
static Object *
object_for_ast( p2_ast* ast, Subst_Ctx *state )
{
    Object *o;
    Type *type;
    void *value;
    int flags = 0;

    switch ( ast->type )
    {
        case BAG_T:

            type = compiler->env->bag_t;
            value = ast->value;
            array__distribute( ( Array* ) value, state->subst_proc );
            break;

        case CHAR_T:

            type = compiler->env->char_t;
            value = ast->value;
            break;

        case FLOAT_T:

            type = compiler->env->float_t;
            value = ast->value;
            break;

        case INT_T:

            type = compiler->env->int_t;
            value = ast->value;
            break;

        case NAME_T:

            /* Retrieve an existing object and exit. */
            o = resolve_name( compiler, ( Name* ) ast->value );
            p2_ast__delete( ast );
            return o;

        case STRING_T:

            type = compiler->env->string_t;
            value = ast->value;
            break;

        case TERM_T:

            type = compiler->env->term_t;
            value = ast->value;
            term__distribute( ( Term* ) value, state->subst_proc );
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
    o = object__new( type, value, flags );

    memory_manager__add( compiler->env->manager, o );

    return o;
}


/* Command functions **********************************************************/


static void
change_namespace( Compiler *c, p2_ast *args )
{
/*
printf( "args->type = %s\n", p2_ast__type__name( args->type ) ); fflush( stdout );
*/
    Object *o;
    Name *name;

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

    #if DEBUG__COMPILER
    printf( "change_namespace(%#x, %#x)\n", ( int ) c, ( int ) args );
    #endif

    name = ( Name* ) arg->value;

    if ( ( o = resolve_name( c, name ) ) )
    {
        if ( o->type != c->cur_ns_obj->type )
            printf( "Error: not a namespace.\n" );

        else
        {
            c->cur_ns_obj = o;
            printf( "Moved to namespace '" );
            p2_ast__print( arg );
            printf( "'.\n" );
        }
    }
}


static void
show_license()
{
    FILE *license;
    int c;

    #if DEBUG__COMPILER
    printf( "[] show_license()\n" );
    #endif

    if ( !( license = fopen( "../../LICENSE.txt", "r" ) ) )
    {
        ERROR( "show_licence: could not open file" );
        return;
    }

    while ( ( c = fgetc( license ) ) != EOF )
        fputc( c, stdout );

    fclose( license );
}


static void
new_namespace( Compiler *c, p2_ast *args )
{
    Object *o;
    Name *name;

    p2_ast *arg = get_inner_node( args );

    #if DEBUG__SAFE
    if ( arg->type != NAME_T )
    {
        ERROR( "new_namespace: AST type mismatch" );
        return;
    }
    #endif

    #if DEBUG__COMPILER
    printf( "[] new_namespace(%#x, %#x)\n", ( int ) c, ( int ) args );
    #endif

    name = ( Name* ) arg->value;
    o = object__new
        ( c->env->ns_t, namespace__new(), 0 );
    memory_manager__add( c->env->manager, o );

    assign_name( c, name, o );
}


static void
remove_ns_item( Compiler *c, p2_ast *args )
{
    Name *name;
    p2_ast *arg = get_inner_node( args );

    #if DEBUG__SAFE
    if ( arg->type != NAME_T )
    {
        ERROR( "remove_ns_item: AST type mismatch" );
        return;
    }
    #endif

    #if DEBUG__COMPILER
    printf( "[] remove_ns_item(%#x, %#x)\n", ( int ) c, ( int ) args );
    #endif

    name = ( Name* ) arg->value;

    namespace__remove( c->cur_ns_obj, name );
}


static void
save_as( Compiler *c, p2_ast *args )
{
    char *path;
    p2_ast *arg = get_inner_node( args );
    Name *name = ( Name* ) arg->value;

    #if DEBUG__COMPILER
    printf( "[] save_as(%#x, %#x)\n", ( int ) c, ( int ) args );
    #endif

    path = ( char* ) array__peek( name );

    compiler__serialize( c, path );
}


static void
garbage_collect( Compiler *c )
{
    Memory_Manager *m = c->env->manager;
    int size_before, size_after;

    clock_t t = clock();
    double elapsed_time;

    #if DEBUG__COMPILER
    printf( "[] garbage_collect(%#x)\n", ( int ) c );
    #endif

printf( "---c gc 1---\n" ); fflush( stdout );

    size_before = memory_manager__size( m );
    memory_manager__collect( m );
    size_after = memory_manager__size( m );
printf( "---c gc 2---\n" ); fflush( stdout );

    elapsed_time = difftime( clock(), t );

    printf( "Collected %i of %i objects (%.3g%%) in %fus.\n",
        size_before - size_after,
        size_before,
        ( ( size_before - size_after ) * 100 ) / ( double ) size_before,
        elapsed_time * 1000000 );
}


/* Externally linked functions for the parser *********************************/


int
compiler__evaluate_command( char *name, p2_ast *args )
{
    int ret = 0;

    if ( !strcmp( name, "gc" ) )
    {
        if ( n_args( args, 0 ) )
            garbage_collect( compiler );
    }

    else if ( !strcmp( name, "license" ) )
    {
        if ( n_args( args, 0 ) )
            show_license();
    }

    else if ( !strcmp( name, "new" ) )
    {
        if ( n_args( args, 1 ) )
            new_namespace( compiler, args );
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

    else if ( !strcmp( name, "rm" ) )
    {
        if ( n_args( args, 1 ) )
            remove_ns_item( compiler, args );
    }

    else if ( !strcmp( name, "saveas" ) )
    {
        if ( n_args( args, 1 ) )
            save_as( compiler, args );
    }

    else if ( !strcmp( name, "all" ) )
    {
        if ( n_args( args, 0 ) )
        {
            namespace__show_children( compiler->cur_ns_obj );
        }
    }

    else if ( !strcmp( name, "size" ) )
    {
        if ( n_args( args, 0 ) )
            printf( "There are %i objects in this environment.\n",
                memory_manager__size( compiler->env->manager ) );
    }

    else
        printf( "Error: unknown command.\n" );

    if ( args )
        p2_ast__delete( args );

    free( name );

    return ret;
}


int
compiler__evaluate_expression( Name *name, p2_ast *expr )
{
    Subst_Ctx state;
    Closure *subst_proc;

    int ret = 0;
    p2_ast *a = 0;
    Object *o;
    char print_buffer[1000];
    Term *t;

    Encoder char__encode, double__encode, string__encode, term__encode;

    #if DEBUG__SAFE
    if ( !expr )
    {
        ERROR( "compiler__evaluate_expression: null AST node" );
        return 0;
    }
    #endif

    #if DEBUG__COMPILER
    printf( "compiler__evaluate_expression(%#x, %#x)\n", ( int ) name, ( int ) expr );
    #endif

    char__encode = compiler->env->char_t->encode;
    double__encode = compiler->env->float_t->encode;
    string__encode = compiler->env->string_t->encode;
    term__encode = compiler->env->term_t->encode;
    compiler->env->char_t->encode = ( Encoder ) char__encode__alt;
    compiler->env->float_t->encode = ( Encoder ) double__encode__alt;
    compiler->env->string_t->encode = ( Encoder ) string__encode__alt;
    compiler->env->term_t->encode = ( Encoder ) term__encode__alt;

    if ( name )
        a = p2_ast__name( name );

    state.sofarsogood = TRUE;

    subst_proc = closure__new( ( procedure ) substitute_object_for_ast, &state );
    state.subst_proc = subst_proc;

    o = object_for_ast( expr, &state );
    closure__delete( subst_proc );

    /* If a term, reduce. */
    if ( o && state.sofarsogood && o->type == compiler->env->term_t )
    {
        t = SK_reduce( ( Term* ) o->value,
            compiler->env->manager,
            compiler->env->term_t,
            compiler->env->prim_t,
            compiler->env->combinator_t, 0 );

        if ( t )
            o->value = t;

        else
            o = 0;
    }

    if ( o && state.sofarsogood )
    {
        printf( "%#x : %s ", ( int ) o, o->type->name );
        if ( a )
        {
            assign_name( compiler, name, o );
            p2_ast__print( a );
        }
        else
            printf( "[]" );

        printf( " = " );

        o->type->encode( o->value, print_buffer );
        printf( print_buffer );

        printf( "\n" );
    }

    if ( a )
        p2_ast__delete( a );

    compiler->env->char_t->encode = char__encode;
    compiler->env->float_t->encode = double__encode;
    compiler->env->string_t->encode = string__encode;
    compiler->env->term_t->encode = term__encode;

    return ret;
}


int
compiler__handle_parse_error( char *msg )
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


int
compiler__suppress_output()
{
    return compiler->suppress_output;
}


int
compiler__show_line_numbers()
{
    return compiler->show_line_numbers;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
