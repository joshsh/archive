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
compiler__new( Environment *env )
{
    Compiler *c;

    if ( instance_exists )
    {
        ERROR( "compiler__new: concurrent compiler instances not allowed" );
        c = 0;
        goto finish;
    }

    #if DEBUG__SAFE
    if ( !env )
    {
        ERROR( "compiler__new: null environment" );
        c = 0;
        goto finish;
    }
    #endif

    if ( !( c = new( Compiler ) ) )
    {
        ERROR( "compiler__new: allocation failed" );
        goto finish;
    }

    instance_exists = TRUE;

    c->env = env;
    c->cur_ns_obj = env->data;
    c->locked = FALSE;
    c->suppress_output = FALSE;
    c->show_line_numbers = TRUE;

    #if DEBUG__SAFE
    /* These basic types are indispensable for the compiler to communicate with
       the parser. */
    if ( !( environment__resolve_type( env, "Bag" )
         && environment__resolve_type( env, "char" )
         && environment__resolve_type( env, "cstring" )
         && environment__resolve_type( env, "double" )
         && environment__resolve_type( env, "int" )
         && environment__resolve_type( env, "Term" ) ) )
    {
        ERROR( "compiler__new: basic type not found" );
        free( c );
        instance_exists = FALSE;
        c = 0;
        goto finish;
    }
    #endif

    if ( !( c->commands = create_commands() ) )
    {
        ERROR( "compiler__new: allocation failed" );
        free( c );
        instance_exists = FALSE;
        c = 0;
        goto finish;
    }

finish:

    #if DEBUG__COMPILER
    printf( "[%#x] compiler__new(%#x)\n", ( int ) c, ( int ) env );
    #endif

    return c;
}


void
compiler__delete( Compiler *c )
{
    #if DEBUG__SAFE
    if ( !c )
    {
        ERROR( "compiler__delete: null argument" );
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

    #if DEBUG__SAFE
    if ( !c )
    {
        ERROR( "compiler__parse: null argument" );
        return 1;
    }
    #endif

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


static void
undef_error( Name *name )
{
    printf( "Error: \"" );
    name__print( name );
    printf( "\" is not defined in this namespace.\n" );
}


Object *
compiler__define( Compiler *c, Name *name, Object *o )
{
    Namespace_o *ns_obj;

    #if DEBUG__COMPILER
    Object *o_orig = o;
    #endif

    char *first = name__pop( name );

    if ( !strcmp( first, "root" ) )
    {
        ns_obj = c->env->root;

        if ( o )
            o = namespace__add( ns_obj, name, o );
        else
            o = namespace__remove( ns_obj, name );

        name__push( name, first );
    }

    else if ( !strcmp( first, "here" ) )
    {
        ns_obj = c->cur_ns_obj;

        if ( o )
            o = namespace__add( ns_obj, name, o );
        else
            o = namespace__remove( ns_obj, name );

        name__push( name, first );
    }

    else
    {
        ns_obj = c->cur_ns_obj;
        name__push( name, first );

        if ( o )
            o = namespace__add( ns_obj, name, o );
        else
            o = namespace__remove( ns_obj, name );
    }

    if ( !o )
        undef_error( name );

    #if DEBUG__COMPILER
    printf( "[%#x] compiler__define(%#x, ", ( int ) o, ( int ) c );
    name__print( name );
    printf( ", %#x)\n", ( int ) o_orig );
    #endif

    return o;
}


Object *
compiler__resolve( Compiler *c, Name *name )
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

    else if ( !strcmp( first, "here" ) )
    {
        ns_obj = c->cur_ns_obj;
        o = namespace__lookup( ns_obj, name );
        name__push( name, first );
    }

    else
    {
        ns_obj = c->cur_ns_obj;
        name__push( name, first );

        #if COMPILER__NAME_INHERITANCE
        o = namespace__resolve( ns_obj, name, c->env->manager );
        #else
        o = namespace__lookup( ns_obj, name );
        #endif
    }

    if ( !o )
        undef_error( name );

    #if DEBUG__COMPILER
    printf( "[%#x] compiler__resolve(%#x, ", ( int ) o, ( int ) c );
    name__print( name );
    printf( ")\n" );
    #endif

    return o;
}


/******************************************************************************/


boolean
compiler__suppress_output( Compiler *c )
{
    return c->suppress_output;
}


boolean
compiler__show_line_numbers( Compiler *c )
{
    return c->show_line_numbers;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
