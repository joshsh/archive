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
#include "license.h"


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
        fprintf( stderr, "Error: missing argument\n" );
        a = 0;
    }

    else
    {
        subterm = term__subterm_at( term, i );

        if ( DEBUG__SAFE && ( unsigned int ) *( subterm->head ) != 2 )
            abort();

        a = *( subterm->head + 1 );

        term__delete( subterm );
    }

    if ( a )
    {
        if ( DEBUG__SAFE && a->type != NAME_T )
            abort();

        else
            return a->value;
    }

    else
        return 0;
}


/* Command functions **********************************************************/


static int
command_all( Compiler *c, Ast *args )
{
    int n = count_args( args ), i;
    Object *o;
    Name *name;

    if ( !n )
    {
        if ( !c->quiet )
            namespace__show_children( c->cur_ns_obj );
    }

    else
    {
        for ( i = 0; i < n; i++ )
        {
            if ( i && !c->quiet )
                printf( "\n" );

            name = get_arg( args, i );
            o = compiler__resolve( c, name );

            if ( ( o ) && o->type != c->cur_ns_obj->type )
            {
                /* FIXME: should print to stderr */
                printf( "Error: \"" );
                name__print( name );
                printf( "\" is not a namespace\n" );
            }

            else if ( !c->quiet )
                namespace__show_children( o );
        }
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
            if ( o2->type == c->cur_ns_obj->type )
            {
                array__enqueue( dest, array__dequeue( src ) );
                compiler__define( c, dest, o );
                array__enqueue( src, array__dequeue( dest ) );

                if ( !c->quiet )
                    printf( "Assignment from 1 object.\n" );
            }

            else
            {
                /* FIXME: should print to stderr */
                printf( "Error: \"" );
                name__print( dest );
                printf( "\" is not a namespace\n" );
            }
        }
    }

    return 0;
}


static int
command_gc( Compiler *c, Ast *args )
{
    Memory_Manager *m = environment__manager( c->env );
    int size_before, size_after;
    double elapsed_time;

    clock_t t = clock();

    args = 0;

    #if DEBUG__COMPILER
    printf( "[] command_gc(%#x)\n", ( int ) c );
    #endif

    size_before = memory_manager__size( m );
    memory_manager__collect( m );
    size_after = memory_manager__size( m );

    elapsed_time = difftime( clock(), t );

    if ( !c->quiet )
    {
        printf( "Collected %i of %i objects (%.3g%%) in %fms.\n",
            size_before - size_after,
            size_before,
            ( ( size_before - size_after ) * 100 ) / ( double ) size_before,
            elapsed_time * 1000 );
    }

    return 0;
}


static int
command_license( Compiler *c, Ast *args )
{
    args = 0;

    if ( !c->quiet )
        printf( "%s\n", gpl_license );

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
            if ( o2->type == c->cur_ns_obj->type )
            {
                namespace__add_simple( o2->value, array__peek( src ), o );
/*
                array__enqueue( dest, array__dequeue( src ) );
                compiler__define( c, dest, o );
                array__enqueue( src, array__dequeue( dest ) );
*/
                if ( compiler__undefine( c, src ) && !c->quiet )
                    printf( "Reassignment from 1 object.\n" );
            }

            else
            {
                /* FIXME: should print to stderr */
                printf( "Error: \"" );
                name__print( dest );
                printf( "\" is not a namespace\n" );
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
        ( c->cur_ns_obj->type, namespace__new(), 0 );
    memory_manager__add( environment__manager( c->env ), o );

    compiler__define( c, name, o );

    return 0;
}


static int
command_ns( Compiler *c, Ast *args )
{
    Object *o;
    Name *name;

    if ( !( name = get_arg( args, 0 ) ) )
        return 0;

    if ( ( o = compiler__resolve( c, name ) ) )
    {
        if ( o->type != c->cur_ns_obj->type )
            fprintf( stderr, "Error: not a namespace\n" );

        else
        {
            c->cur_ns_obj = o;

            if ( !c->quiet )
            {
                printf( "Moved to namespace \"" );
                name__print( name );
                printf( "\".\n" );
            }
        }
    }

    return 0;
}


static int
command_quit( Compiler *c, Ast *args )
{
    args = 0;

    if ( !c->quiet )
        printf( "Bye.\n" );

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

    if ( compiler__undefine( c, name ) && !c->quiet )
        printf( "Unassigned 1 object.\n" );

    return 0;
}


static int
command_save( Compiler *c, Ast *args )
{
    Name *name;
    char *path = c->save_to_path;

    if ( !path )
    {
        fprintf( stderr, "Error: use _saveas to specify an output path\n" );
        return 0;
    }

    else
    {
        compiler__serialize( c, path );
        if ( !c->quiet )
            printf( "Saved root:data as \"%s\".\n", path );
        return 0;
    }
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

    path = array__peek( name );

    compiler__serialize( c, path );

    if ( !c->quiet )
        printf( "Saved root:data as \"%s\".\n", path );

    return 0;
}


static int
command_size( Compiler *c, Ast *args )
{
    args = 0;

    if ( !c->quiet )
        printf( "There are %i objects in this environment.\n",
            memory_manager__size( environment__manager( c->env ) ) );

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

    if ( !dictionary__add( d, name, com ) )
    {
        free( com );
        return 0;
    }

    else
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

    if ( add_command( d, "all",      command_all,        0, -1 )
         && add_command( d, "cp",       command_cp,         2, 2 )
         && add_command( d, "gc",       command_gc,         0, 0 )
         && add_command( d, "license",  command_license,    0, 0 )
         && add_command( d, "mv",       command_mv,         2, 2 )
         && add_command( d, "new",      command_new,        1, 1 )
         && add_command( d, "ns",       command_ns,         1, 1 )
         && add_command( d, "quit",     command_quit,       0, 0 )
         && add_command( d, "rm",       command_rm,         1, -1 )
         && add_command( d, "saveas",   command_saveas,     1, 1 )
         && add_command( d, "save",     command_save,       0, 0 )
         && add_command( d, "size",     command_size,       0, 0 ) )
    {
        return d;
    }

    else
    {
        delete_commands( d );
        return 0;
    }
}


/************************************************8*****************************/


int
compiler__evaluate_command( Compiler *c, char *name, Ast *args )
{
    int result = 0;
    int n = count_args( args );
    Command *com = dictionary__lookup( c->commands, name );

    if ( !com )
        fprintf( stderr, "Error: unknown command: \"%s\"\n", name );
    else if ( n < com->args_min )
        fprintf( stderr, "Error: missing argument(s) to command \"%s\"\n", name );
    else if ( n > com->args_max && com->args_max >= 0 )
        fprintf( stderr, "Error: too many arguments to command \"%s\"\n", name );
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
