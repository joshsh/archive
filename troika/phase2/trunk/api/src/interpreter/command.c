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

/*
#if USE_NCURSES
#    include <ncurses.h>
#endif
*/

#include "Interpreter-impl.h"
#include "license.h"


typedef int ( *CommandFunction )( Interpreter *c, Array *args );


typedef struct Command
{
    CommandFunction f;
    int args_min, args_max;

    char *name;
    char *args;
    char *description;

} Command;


/* Command functions **********************************************************/


static int
command_cp( Interpreter *c, Array *args )
{
    Object *o, *o2;
    Name *src, *dest;

    if ( !( src = array__get( args, 0 ) )
      || !( dest = array__get( args, 1 ) ) )
        return 0;

    o = interpreter__resolve( c, src );

    if ( o )
    {
        if ( ( o2 = interpreter__resolve( c, dest ) ) )
        {
            if ( o2->type == c->cur_ns_obj->type )
            {
                array__enqueue( dest, array__dequeue( src ) );
                interpreter__define( c, dest, o );
                array__enqueue( src, array__dequeue( dest ) );

                if ( !c->quiet )
                    PRINT( "Assignment from 1 object.\n" );
            }

            else
                err_notns( dest );
        }
    }

    return 0;
}


#define HAS_KUICKSHOW   1

static int
command_draw( Interpreter *c, Array *args )
{
    Object *o;
    char *s;
    Name *name;

    FILE *f;

    if ( !( name = array__get( args, 0 ) ) )
        return 0;

    o = interpreter__resolve( c, name );

    if ( o )
    {
        s = interpreter__draw( c, o );

        if ( !c->quiet )
        {
            PRINT( "%s\n", s );
        }

        if ( HAS_KUICKSHOW )
        {
            f = fopen( "/tmp/p2out.dot", "w" );
            fprintf( f, s );
            fclose( f );
            system( "dot -Tpng -o /tmp/p2out.png /tmp/p2out.dot" );
            system( "kuickshow /tmp/p2out.png" );
        }

        free( s );
    }

    return 0;
}


static int
command_gc( Interpreter *c, Array *args )
{
    args = 0;

    memory_manager__collect( environment__manager( c->env ), TRUE, !c->quiet );

    return 0;
}


static int
command_help( Interpreter *c, Array *args )
{
    ACTION helper( char **refp )
    {
        Command *com = dictionary__lookup( c->commands, *refp );

        PRINT( "  _%s", com->name );
        if ( com->args )
            PRINT( " %s", com->args );
        PRINT( " -- %s\n", com->description );

        return CONTINUE;
    }

    Array *a;

    args = 0;

    if ( !c->quiet )
    {
        PRINT( "\
Syntax:\n\
  stmt:  (cmnd | expr | name ':=' expr | expr '=:' name) ';'\n\
  cmnd:  '_' ID {name}\n\
  expr:  obj | expr obj\n\
  obj:   CHAR | FLOAT | INT | STRING | bag | name | '(' [obj] ')'\n\
  bag:   '{' [expr {',' expr}] '}'\n\
  name:  ID {':' ID}\n" );

        PRINT( "\n" );

        a = dictionary__keys( c->commands );
        array__sort( a, ( Comparator ) strcmp );

        PRINT( "Commands:\n" );
        array__walk( a, ( Visitor ) helper );

        array__delete( a );

        PRINT( "\n" );

        PRINT( "\
Example:\n\
  _new floatmath;\n\
  floatmath:pi := 3.14159;\n\
  _ns floatmath;\n\
  * := double_double__mul;\n\
  * (* 2.0 pi) =: circumference;\n\
  _ns root:data;\n\
  circumference 10.0 =: result;\n\
  floatmath;\n\
  here;\n\
  root;\n\
  _saveas foo.p2;\n" );

    }

    return 0;
}


static int
command_history( Interpreter *c, Array *args )
{
    char *path;
    Name *name = array__size( args )
        ? array__get( args, 0 ) : 0;

    if ( name )
    {
        path = array__peek( name );

        if ( !c->quiet )
            PRINT( "Saving history to \"%s\".\n", path );

        interpreter__write_history( path );
    }

    else
    {
        if ( !c->quiet )
            interpreter__print_history();
    }

    return 0;
}


static int
command_license( Interpreter *c, Array *args )
{
    args = 0;

    if ( !c->quiet )
        PRINT( "%s\n", gpl_license );

    return 0;
}


static int
command_mv( Interpreter *c, Array *args )
{
    Object *o, *o2;
    Name *src, *dest;

    if ( !( src = array__get( args, 0 ) )
      || !( dest = array__get( args, 1 ) ) )
        return 0;

    o = interpreter__resolve( c, src );

    if ( o )
    {
        if ( ( o2 = interpreter__resolve( c, dest ) ) )
        {
            if ( o2->type == c->cur_ns_obj->type )
            {
                namespace__add_simple( o2->value, array__peek( src ), o );
/*
                array__enqueue( dest, array__dequeue( src ) );
                interpreter__define( c, dest, o );
                array__enqueue( src, array__dequeue( dest ) );
*/
                if ( interpreter__undefine( c, src ) && !c->quiet )
                    PRINT( "Reassignment from 1 object.\n" );
            }

            else
                err_notns( dest );
        }
    }

    return 0;
}


#define putchar 

static int
command_new( Interpreter *c, Array *args )
{
    Object *o;
    Name *name;
putchar('\'');FFLUSH;

    name = array__size( args )
        ? array__get( args, 0 ) : 0;

    if ( !name )
        return 0;
putchar('*');FFLUSH;
/*
    if ( interpreter__resolve( c, name ) )
    {
        ERROR( "name maps to an existing object" );
        return 0;
    }
*/

    o = memory_manager__object( environment__manager( c->env ),
        c->cur_ns_obj->type, namespace__new(), 0 );

    if ( o )
        o = interpreter__define( c, name, o );

    if ( o && !c->quiet )
    {
        PRINT( "Created namespace " );
        name__print( name );
        PRINT( ".\n" );
    }

    return 0;
}


static int
command_ns( Interpreter *c, Array *args )
{
    Object *o;
    Name *name, *fullname;

    if ( !( name = array__get( args, 0 ) ) )
        return 0;
putchar(',');FFLUSH;

    if ( ( o = interpreter__resolve( c, name ) ) )
    {
        if ( o->type != c->cur_ns_obj->type )
            err_notns( name );

        else
        {
            c->cur_ns_obj = o;

            if ( !c->quiet )
            {
                fullname = interpreter__name_of__full( c, 0, o );

                if ( fullname )
                {
                    PRINT( "Moved to namespace " );
                    name__print( fullname );
/*                    name__print( name );*/
                    PRINT( ".\n" );
                    name__delete( fullname );
                }
            }
        }
    }

    return 0;
}


static int
command_quit( Interpreter *c, Array *args )
{
    args = 0;

    if ( !c->quiet )
        PRINT( "Bye.\n" );

    return 1;
}


static int
command_rm( Interpreter *c, Array *args )
{
    Name *name;

    if ( !( name = array__get( args, 0 ) ) )
        return 0;

    if ( interpreter__undefine( c, name ) && !c->quiet )
        PRINT( "Unassigned 1 object.\n" );

    return 0;
}


static int
command_save( Interpreter *c, Array *args )
{
    char *path = c->save_to_path;

    args = 0;

    if ( !path )
    {
        ERROR( "use _saveas to specify an output path" );
        return 0;
    }

    else
    {
        interpreter__serialize( c, path );
        if ( !c->quiet )
            PRINT( "Saved root:data as \"%s\".\n", path );
        return 0;
    }
}


static int
command_saveas( Interpreter *c, Array *args )
{
    Name *name;
    char *path;

    if ( !( name = array__get( args, 0 ) ) )
        return 0;

    path = array__peek( name );

    interpreter__serialize( c, path );

    if ( !c->quiet )
        PRINT( "Saved root:data as \"%s\".\n", path );

    return 0;
}


static int
command_size( Interpreter *c, Array *args )
{
    args = 0;

    if ( !c->quiet )
        PRINT( "There are %i objects in this environment.\n",
            memory_manager__size( environment__manager( c->env ) ) );

    return 0;
}


/******************************************************************************/


#define cp(to, from)  if ((from) && !(to = STRDUP(from))) goto failure;

static Command *
add_command( Dictionary *d, char *name, CommandFunction f, int args_min, int args_max, char *args, char *description )
{
    Command *com = 0;
    char *n = 0, *a = 0, *s = 0;

    cp( n, name );
    cp( a, args );
    cp( s, description );

    com = NEW( Command );
    if ( !com )
        goto failure;

    com->f = f;
    com->args_min = args_min;
    com->args_max = args_max;
    com->name = n;
    com->args = a;
    com->description = s;

    if ( !dictionary__add( d, name, com ) )
        goto failure;

    return com;

failure:

    if ( n )
        free( n );
    if ( a )
        free( a );
    if ( s )
        free( s );
    if ( com )
        free( com );

    return 0;
}


void
delete_commands( Dictionary *commands )
{
    ACTION helper( Command **refp )
    {
        Command *c = *refp;

        if ( c->name )
            free( c->name );
        if ( c->args )
            free( c->args );
        if ( c->description )
            free( c->description );

        free( c );

        return CONTINUE;
    }

    dictionary__walk( commands, ( Visitor ) helper );
    dictionary__delete( commands );
}


Dictionary *
create_commands()
{
    Dictionary *d = dictionary__new();

    if ( !d )
        goto failure;

    if ( add_command( d, "cp",       command_cp,         2, 2,  "<obj> <ns>", "copy an object to the given namespace" )
      && add_command( d, "draw",     command_draw,       1, 1,  "<obj>", "draw an object as a dot graph" )
      && add_command( d, "gc",       command_gc,         0, 0,  0, "invoke (force) the garbage collector" )
      && add_command( d, "help",     command_help,       0, 0,  0, "display this help info" )
      && add_command( d, "license",  command_license,    0, 0,  0, "display the GPL" )
      && add_command( d, "mv",       command_mv,         2, 2,  "<obj> <ns>", "move an object to the given namespace" )
      && add_command( d, "new",      command_new,        1, 1,  "<name>", "create a new namespace" )
      && add_command( d, "ns",       command_ns,         1, 1,  "<ns>", "move to the given namespace" )
      && add_command( d, "quit",     command_quit,       0, 0,  0, "exit the application" )
      && add_command( d, "rm",       command_rm,         1, -1, "<obj>", "remove an object from its parent namespace" )
      && add_command( d, "saveas",   command_saveas,     1, 1,  "<path>", "save environment to a given file" )
      && add_command( d, "save",     command_save,       0, 0,  0, "save environment to original file" )
      && add_command( d, "size",     command_size,       0, 0,  0, "the number of reachable objects in the environment" )
#if HAVE_LIBREADLINE
      && add_command( d, "history",  command_history,    0, 1,  "[path]", "print interpreter history" )
#endif
      )
    {
        return d;
    }

failure:

    if ( d )
        delete_commands( d );

    return 0;
}


/******************************************************************************/


/* Kludge to get the arguments as an Array instead of a Term. */
static Array *
get_arg_array( Ast *args )
{
    Array *a;
    Term *term, *subterm;
    int i, n;
    Ast *arg;

    if ( !args )
        a = array__new( 0, 0 );

    else
    {
        if ( DEBUG__SAFE && args->type != TERM_T )
            abort();

        term = ( Term* ) args->value;
        n = term__length( term );
        a = array__new( n, 0 );

        for ( i = 0; i < n; i++ )
        {
            subterm = term__subterm_at( term, i );

            if ( DEBUG__SAFE && term__length( subterm ) > 1 )
                abort();

            arg = term__head( subterm );
            if ( DEBUG__SAFE && arg->type != NAME_T )
                abort();
            else
                array__enqueue( a, arg->value );

            term__delete( subterm );
        }
    }

    return a;
}


int
interpreter__evaluate_command( Interpreter *c, char *name, Ast *args, const char *text )
{
    int result, n;
    Command *com;
    Array *a;

    if ( DEBUG__SAFE && !c )
        abort();

    interpreter__add_to_history( text );

    a = get_arg_array( args );
    n = array__size( a );

    result = 0;
    com = dictionary__lookup( c->commands, name );

    if ( !com )
        ERROR( "unknown command: \"%s\"\n", name );
    else if ( n < com->args_min )
        ERROR( "missing argument(s) to command \"%s\"\n", name );
    else if ( n > com->args_max && com->args_max >= 0 )
        ERROR( "too many arguments to command \"%s\"\n", name );
    else
        result = com->f( c, a );

    if ( args )
        ast__delete( args );

    free( name );

    memory_manager__collect( environment__manager( c->env ), FALSE, FALSE );

    array__delete( a );

    return result;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
