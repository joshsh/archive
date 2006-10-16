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


typedef int ( *CommandFunction )( Interpreter *itp, Array *args );


typedef struct Command
{
    CommandFunction f;
    int args_min, args_max;

    char *name;
    char *args;
    char *description;

} Command;


/* Command helper functions ***************************************************/


static int
command_cp_helper( Interpreter *itp, Name *src, Name *dest )
{
    Object *o1, *o2;

    if ( DEBUG__SAFE && ( !itp || !src || !dest ) )
        abort();

    o1 = interpreter__resolve( itp, src );

    if ( o1 )
    {
        if ( ( o2 = interpreter__resolve( itp, dest ) ) )
        {
            if ( o2->type == itp->cur_ns_obj->type )
            {
                array__enqueue( dest, array__dequeue( src ) );

                interpreter__define( itp, dest, o1 );
                array__enqueue( src, array__dequeue( dest ) );

/*
                if ( itp->verbose && !itp->quiet )
                    PRINT(
*/
            }

            else
                err_notns( dest );
        }
    }

    return 0;
}


static int
command_rm_helper( Interpreter *itp, Name *name )
{
    if ( DEBUG__SAFE && ( !itp || !name ) )
        abort();

    interpreter__undefine( itp, name );

    return 0;
}


/* Command functions **********************************************************/


static int
command_cp( Interpreter *itp, Array *args )
{
    Name *src, *dest;

    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    src = object__value( array__get( args, 0 ) );
    dest = object__value( array__get( args, 1 ) );

    return command_cp_helper( itp, src, dest );
}


#define HAS_KUICKSHOW   1

/* TODO: allow for other image viewers besides Kuickshow */
static int
command_draw( Interpreter *itp, Array *args )
{
    Object *o;
    char *s;
    Name *name;

    FILE *f;

    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    name = object__value( array__get( args, 0 ) );

    o = interpreter__resolve( itp, name );

    if ( o )
    {
        s = interpreter__draw( itp, o );

        if ( !itp->quiet )
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
command_gc( Interpreter *itp, Array *args )
{
    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    manager__collect( environment__manager( itp->env ), TRUE, !itp->quiet );

    return 0;
}


static int
command_help( Interpreter *itp, Array *args )
{
    ACTION helper( char **refp )
    {
        Command *com = dictionary__lookup( itp->commands, *refp );

        PRINT( "  _%s", com->name );
        if ( com->args )
            PRINT( " %s", com->args );
        PRINT( " -- %s\n", com->description );

        return CONTINUE;
    }

    Array *a;

    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    if ( !itp->quiet )
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

        a = dictionary__keys( itp->commands );
        array__sort( a, ( Comparator ) strcmp );

        PRINT( "Commands:\n" );
        array__walk( a, ( Visitor ) helper );

        array__free( a );

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
command_history( Interpreter *itp, Array *args )
{
    char *path;
    Name *name;

    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    name = array__size( args )
        ? object__value( array__get( args, 0 ) )
        : 0;

    if ( name )
    {
        path = array__peek( name );

        if ( !itp->quiet )
            PRINT( "Saving history to \"%s\".\n", path );

        interpreter__write_history( path );
    }

    else
    {
        if ( !itp->quiet )
            interpreter__print_history();
    }

    return 0;
}


static int
command_license( Interpreter *itp, Array *args )
{
    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    if ( !itp->quiet )
        PRINT( "%s\n", gpl_license );

    return 0;
}


static int
command_mv( Interpreter *itp, Array *args )
{
    Name *src, *dest;

    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    src = object__value( array__get( args, 0 ) );
    dest = object__value( array__get( args, 1 ) );

    return
         command_cp_helper( itp, src, dest )
      || command_rm_helper( itp, src );

/*

    o = interpreter__resolve( c, src );

    if ( o )
    {
        if ( ( o2 = interpreter__resolve( c, dest ) ) )
        {
            if ( o2->type == c->cur_ns_obj->type )
            {
                namespace__add_simple( o2->value, array__peek( src ), o );

                if ( interpreter__undefine( c, src ) && !c->quiet )
                    PRINT( "Reassignment from 1 object.\n" );
            }

            else
                err_notns( dest );
        }
    }
*/

    return 0;
}


static int
command_new( Interpreter *itp, Array *args )
{
    Object *o;
    Name *name;

    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    name = array__size( args )
        ? object__value( array__get( args, 0 ) )
        : 0;

    if ( name )
    {
        o = manager__object( environment__manager( itp->env ),
            itp->cur_ns_obj->type, namespace__new(), 0 );

        if ( o )
            o = interpreter__define( itp, name, o );
    }

    return 0;
}


static int
command_ns( Interpreter *itp, Array *args )
{
    Object *o;
    Name *name, *fullname;

    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    name = object__value( array__get( args, 0 ) );

    if ( ( o = interpreter__resolve( itp, name ) ) )
    {
        if ( o->type != itp->cur_ns_obj->type )
            err_notns( name );

        else
        {
            itp->cur_ns_obj = o;

/*
            if ( !c->quiet )
            {
                fullname = interpreter__name_of__full( c, 0, o );

                if ( fullname )
                {
                    PRINT( "Moved to namespace " );
                    name__print( fullname );
                    PRINT( ".\n" );
                    name__free( fullname );
                }
            }
*/
        }
    }

    return 0;
}


static int
command_quit( Interpreter *itp, Array *args )
{
    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

/*
    if ( !c->quiet )
        PRINT( "Bye.\n" );
*/

    return 1;
}


static int
command_rm( Interpreter *itp, Array *args )
{
    Name *name;

    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    name = object__value( array__get( args, 0 ) );

    return command_rm_helper( itp, name );
}


static int
command_save( Interpreter *itp, Array *args )
{
    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    /* NOTE: the save_to_path member will normally change before this function
             returns, so we dereference it multiple times. */
    if ( !itp->save_to_path )
    {
        ERROR( "use _saveas to specify an output path" );
        return 0;
    }

    else
    {
        interpreter__serialize( itp, itp->save_to_path );
        if ( !itp->quiet )
            PRINT( "Saved root:data as \"%s\".\n", itp->save_to_path );
        return 0;
    }
}


static int
command_saveas( Interpreter *itp, Array *args )
{
    Name *name;
    char *path;

    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    name = object__value( array__get( args, 0 ) );

    path = array__peek( name );

    interpreter__serialize( itp, path );

    if ( !itp->quiet )
        PRINT( "Saved root:data as \"%s\".\n", path );

    return 0;
}


static int
command_size( Interpreter *itp, Array *args )
{
    if ( DEBUG__SAFE && ( !itp || !args ) )
        abort();

    if ( !itp->quiet )
        PRINT( "There are %i objects in this environment.\n",
            manager__size( environment__manager( itp->env ) ) );

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
    dictionary__free( commands );
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


int
interpreter__evaluate_command( Interpreter *itp,
                               OBJ( STRING ) *name,
                               OBJ( ARRAY ) *args,
                               const char *text )
{
    int result, n;
    Command *com;
    Array *a;
    char *nameval = object__value( name );

    if ( DEBUG__SAFE && !itp )
        abort();

    interpreter__add_to_history( text );

    a = ( args )
        ? object__value( args )
        : 0;

    n = ( a )
        ? array__size( a )
        : 0;

    result = 0;
    com = dictionary__lookup( itp->commands, nameval );

    if ( !com )
        ERROR( "unknown command: \"%s\"\n", nameval );
    else if ( n < com->args_min )
        ERROR( "missing argument(s) to command \"%s\"\n", nameval );
    else if ( n > com->args_max && com->args_max >= 0 )
        ERROR( "too many arguments to command \"%s\"\n", nameval );
    else
        result = com->f( itp, a );

    manager__collect( environment__manager( itp->env ), FALSE, FALSE );

    return result;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
