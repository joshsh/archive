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


static void
nodef_error()
{
    printf( "Error: trivial identifier.\n" );
}


static void
notns_error( Name *name )
{
    printf( "Error: \"" );
    name__print( name );
    printf( "\" is not a namespace.\n" );
}


static void
undef_error( Name *name )
{
    printf( "Error: \"" );
    name__print( name );
    printf( "\" is not defined in this namespace.\n" );
}


static Object *
define( Namespace_o *nso, Name *name, Object *o, Memory_Manager *m )
{
    void *key;
    Namespace_o *local;

    if ( !array__size( name ) )
    {
        nodef_error();
        return 0;
    }

    #if COMPILER__NAME_INHERITANCE
    key = array__dequeue( name );
    local = namespace__resolve( nso, name, m );

    if ( !local )
    {
        undef_error( name );
        o = 0;
    }

    else if ( local->type != nso->type )
    {
        array__enqueue( name, key );
        notns_error( name );
        o = 0;
    }
    else
    {
        nso = local;

        o = ( o )
            ? namespace__add_simple( nso->value, key, o )
            : namespace__remove_simple( nso->value, key );

        array__enqueue( name, key );
    }
    #else
        o = ( o )
            ? namespace__add( nso, name, o )
            : namespace__remove( nso, name );
    #endif
    return o;
}


static Object *
resolve( Namespace_o *nso, Name *name, Memory_Manager *m )
{
    Object *o;

    #if COMPILER__NAME_INHERITANCE
    o = namespace__resolve( nso, name, m );
    #else
    o = namespace__lookup( nso, name );
    #endif

    return o;
}


Object *
compiler__define( Compiler *c, Name *name, Object *o )
{
    Namespace_o *ns_obj;

    #if DEBUG__COMPILER
    Object *o_orig = o;
    #endif

    char *first = name__pop( name );
    ns_obj = c->cur_ns_obj;

    if ( !strcmp( first, "root" ) )
    {
        ns_obj = environment__root( c->env );
        define( ns_obj, name, o, environment__manager( c->env ) );
        name__push( name, first );
    }

    else if ( !strcmp( first, "here" ) )
    {
        define( ns_obj, name, o, environment__manager( c->env ) );
        name__push( name, first );
    }

    else
    {
        name__push( name, first );
        define( ns_obj, name, o, environment__manager( c->env ) );
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
        ns_obj = environment__root( c->env );
        o = resolve( ns_obj, name, environment__manager( c->env ) );
        name__push( name, first );
    }

    else if ( !strcmp( first, "here" ) )
    {
        ns_obj = c->cur_ns_obj;
        o = resolve( ns_obj, name, environment__manager( c->env ) );
        name__push( name, first );
    }

    else
    {
        ns_obj = c->cur_ns_obj;
        name__push( name, first );

        o = resolve( ns_obj, name, environment__manager( c->env ) );
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


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
