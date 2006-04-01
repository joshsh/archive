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

#include <time.h>

#include "Compiler-impl.h"


static int
count_args( Ast *args )
{
    Term *term;

    if ( !args )
        return 0;

    #if DEBUG__SAFE
    if ( args->type != TERM_T )
    {
        ERROR( "get_arg: wrong AST type" );
        return 0;
    }
    #endif

    term = ( Term* ) args->value;

    return term__length( term );
}


static Name *
get_arg( Ast *args, unsigned int i )
{
    Term *term, *subterm;
    Ast *a;

    #if DEBUG__SAFE
    if ( args->type != TERM_T )
    {
        ERROR( "get_arg: wrong AST type" );
        return 0;
    }
    #endif

    term = ( Term* ) args->value;

    if ( i >= term__length( term ) )
    {
        printf( "Error: missing argument.\n" );
        a = 0;
    }

    else
    {
        subterm = term__subterm_at( term, i );

        #if DEBUG__SAFE
        if ( ( unsigned int ) *( subterm->head ) != 2 )
        {
            ERROR( "get_arg: argument subterm is not a singleton" );
            a = 0;
        }
        else
        #endif

        a = *( subterm->head + 1 );

        term__delete( subterm );
    }

    if ( a )
    {
        #if DEBUG__SAFE
        if ( a->type != NAME_T )
        {
            ERROR( "get_arg: type mismatch" );
            return 0;
        }
        #endif

        return a->value;
    }

    else
        return 0;
}


/* Command functions **********************************************************/


static int
command_all( Compiler*c, Ast *args )
{
    int n = count_args( args ), i;
    Object *o;
    Name *name;

    if ( !n )
        namespace__show_children( c->cur_ns_obj );

    else
        for ( i = 0; i < n; i++ )
        {
            if ( i )
                printf( "\n" );

            name = get_arg( args, i );
            o = compiler__resolve( c, name );

            if ( o->type != c->env->ns_t )
            {
                printf( "Error: \"" );
                name__print( name );
                printf( "\" is not a namespace.\n" );
            }

            else
                namespace__show_children( o );
        }

    return 0;
}


static int
command_cp( Compiler *c, Ast *args )
{
    Object *o, *o2;
    Name *src, *dest;

    #if DEBUG__COMPILER
    printf( "[] command_cp(%#x, %#x)\n", ( int ) c, ( int ) args );
    #endif

    if ( !( src = get_arg( args, 0 ) )
      || !( dest = get_arg( args, 1 ) ) )
        return 0;

    o = compiler__resolve( c, src );

    if ( o )
    {
        if ( ( o2 = compiler__resolve( c, dest ) ) )
        {
            if ( o2->type == c->env->ns_t )
            {
                array__enqueue( dest, array__dequeue( src ) );
                compiler__define( c, dest, o );
                array__enqueue( src, array__dequeue( dest ) );

                printf( "Assignment from 1 object.\n" );
            }

            else
            {
                printf( "Error: \"" );
                name__print( dest );
                printf( "\" is not a namespace.\n" );
            }
        }
    }

    return 0;
}


static int
command_gc( Compiler *c, Ast *args )
{
    Memory_Manager *m = c->env->manager;
    int size_before, size_after;
    args = 0;

    clock_t t = clock();
    double elapsed_time;

    #if DEBUG__COMPILER
    printf( "[] command_gc(%#x)\n", ( int ) c );
    #endif

    size_before = memory_manager__size( m );
    memory_manager__collect( m );
    size_after = memory_manager__size( m );

    elapsed_time = difftime( clock(), t );

    printf( "Collected %i of %i objects (%.3g%%) in %fms.\n",
        size_before - size_after,
        size_before,
        ( ( size_before - size_after ) * 100 ) / ( double ) size_before,
        elapsed_time * 1000 );

    return 0;
}


static int
command_license( Compiler *c, Ast *args )
{
    FILE *license;
    int ch;
    c = 0;
    args = 0;

    #if DEBUG__COMPILER
    printf( "[] command_license()\n" );
    #endif

    if ( !( license = fopen( "../../LICENSE.txt", "r" ) ) )
    {
        ERROR( "show_license: could not open file" );
        return 0;
    }

    while ( ( ch = fgetc( license ) ) != EOF )
        fputc( ch, stdout );

    fclose( license );

    return 0;
}


static int
command_mv( Compiler *c, Ast *args )
{
    Object *o, *o2;
    Name *src, *dest;

    #if DEBUG__COMPILER
    printf( "[] command_mv(%#x, %#x)\n", ( int ) c, ( int ) args );
    #endif

    if ( !( src = get_arg( args, 0 ) )
      || !( dest = get_arg( args, 1 ) ) )
        return 0;

    o = compiler__resolve( c, src );

    if ( o )
    {
        if ( ( o2 = compiler__resolve( c, dest ) ) )
        {
            if ( o2->type == c->env->ns_t )
            {
                array__enqueue( dest, array__dequeue( src ) );
                compiler__define( c, dest, o );
                array__enqueue( src, array__dequeue( dest ) );

                if ( compiler__define( c, src, 0 ) )
                    printf( "Reassignment from 1 object.\n" );
            }

            else
            {
                printf( "Error: \"" );
                name__print( dest );
                printf( "\" is not a namespace.\n" );
            }
        }
    }

    return 0;
}


static int
command_new( Compiler *c, Ast *args )
{
    Object *o;
    Name *name;

    #if DEBUG__COMPILER
    printf( "[] command_new(%#x, %#x)\n", ( int ) c, ( int ) args );
    #endif

    if ( !( name = get_arg( args, 0 ) ) )
        return 0;

    o = object__new
        ( c->env->ns_t, namespace__new(), 0 );
    memory_manager__add( c->env->manager, o );

    compiler__define( c, name, o );

    return 0;
}


static int
command_ns( Compiler *c, Ast *args )
{
    Object *o;
    Name *name;

    #if DEBUG__COMPILER
    printf( "command_ns(%#x, %#x)\n", ( int ) c, ( int ) args );
    #endif

    if ( !( name = get_arg( args, 0 ) ) )
        return 0;

    if ( ( o = compiler__resolve( c, name ) ) )
    {
        if ( o->type != c->cur_ns_obj->type )
            printf( "Error: not a namespace.\n" );

        else
        {
            c->cur_ns_obj = o;
            printf( "Moved to namespace \"" );
            name__print( name );
            printf( "\".\n" );
        }
    }

    return 0;
}


static int
command_quit( Compiler *c, Ast *args )
{
    c = 0;
    args = 0;

    printf( "End session.\n" );

    return 1;
}


static int
command_rm( Compiler *c, Ast *args )
{
    Name *name;

    #if DEBUG__COMPILER
    printf( "[] command_rm(%#x, %#x)\n", ( int ) c, ( int ) args );
    #endif

    if ( !( name = get_arg( args, 0 ) ) )
        return 0;

    if ( compiler__define( c, name, 0 ) )
        printf( "Unassigned 1 object.\n" );

    return 0;
}


static int
command_saveas( Compiler *c, Ast *args )
{
    Name *name;
    char *path;

    #if DEBUG__COMPILER
    printf( "[] command_saveas(%#x, %#x)\n", ( int ) c, ( int ) args );
    #endif

    if ( !( name = get_arg( args, 0 ) ) )
        return 0;

    path = ( char* ) array__peek( name );

    compiler__serialize( c, path );

    printf( "Saved root:data as \"%s\".\n", path );

    return 0;
}


static int
command_size( Compiler *c, Ast *args )
{
    args = 0;

    printf( "There are %i objects in this environment.\n",
        memory_manager__size( c->env->manager ) );

    return 0;
}


/******************************************************************************/


typedef int ( *CommandFunction )( Compiler *c, Ast *args );


typedef struct Command
{
    CommandFunction f;
    int args_min, args_max;

} Command;


static Command *
add_command( Dictionary *d, char *name, CommandFunction f, int args_min, int args_max )
{
    Command *com;

    if ( !( com = new( Command ) ) )
        return 0;

    com->f = f;
    com->args_min = args_min;
    com->args_max = args_max;

/*
    if ( !dictionary__add( d, name, com ) )
    {
        free( com );
        return 0;
    }

    else
*/
    dictionary__add( d, name, com );

        return com;
}


void
delete_commands( Dictionary *commands )
{
    void *helper( Command **refp )
    {
        free( *refp );
        return 0;
    }

    dictionary__walk( commands, ( Dist_f ) helper );
    dictionary__delete( commands );
}


Dictionary *
create_commands()
{
    Dictionary *d;

    if ( !( d = dictionary__new() ) )
        return 0;

    if ( !( add_command( d, "all",      command_all,        0, -1 )
         && add_command( d, "cp",       command_cp,         2, 2 )
         && add_command( d, "gc",       command_gc,         0, 0 )
         && add_command( d, "license",  command_license,    0, 0 )
         && add_command( d, "mv",       command_mv,         2, 2 )
         && add_command( d, "new",      command_new,        1, 1 )
         && add_command( d, "ns",       command_ns,         1, 1 )
         && add_command( d, "quit",     command_quit,       0, 0 )
         && add_command( d, "rm",       command_rm,         1, -1 )
         && add_command( d, "saveas",   command_saveas,     1, 1 )
         && add_command( d, "size",     command_size,       0, 0 ) ) )
    {
        delete_commands( d );
        return 0;
    }

    else
        return d;
}


/* Externally linked functions for the parser *********************************/


int
compiler__evaluate_command( Compiler *c, char *name, Ast *args )
{
    int result = 0;
    int n = count_args( args );
    Command *com = dictionary__lookup( c->commands, name );

    if ( !com )
        printf( "Error: unknown command: \"%s\".\n", name );
    else if ( n < com->args_min )
        printf( "Error: missing argument(s) to command \"%s\".\n", name );
    else if ( n > com->args_max && com->args_max >= 0 )
        printf( "Error: too many arguments to command \"%s\".\n", name );
    else
        result = com->f( c, args );

    #if DEBUG__COMPILER
    printf( "[%i] compiler__evaluate_command(%#x, %#x, %#x)\n",
        result, ( int ) c, ( int ) name, ( int ) args );
    #endif

    if ( args )
        ast__delete( args );

    free( name );

    return result;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
