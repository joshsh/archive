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
undef_error( Name *name )
{
    printf( "Error: \"" );
    name__print( name );
    printf( "\" is not defined in this namespace.\n" );
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
    Namespace_o *nso;

    char *key = name__pop( name );
    nso = c->cur_ns_obj;

    if ( !strcmp( key, "root" ) )
    {
        nso = environment__root( c->env );
        o = namespace__define( nso, name, o, environment__manager( c->env ) );
        name__push( name, key );
    }

    else if ( !strcmp( key, "here" ) )
    {
        o = namespace__define( nso, name, o, environment__manager( c->env ) );
        name__push( name, key );
    }

    else
    {
        name__push( name, key );
        o = namespace__define( nso, name, o, environment__manager( c->env ) );
    }
/*
    if ( !o )
        undef_error( name );
*/

    return o;
}


Object *
compiler__undefine( Compiler *c, Name *name )
{
    Namespace_o *nso;
    Object *o;

    char *key = name__pop( name );
    nso = c->cur_ns_obj;

    if ( !strcmp( key, "root" ) )
    {
        nso = environment__root( c->env );
        o = namespace__undefine( nso, name, environment__manager( c->env ) );
        name__push( name, key );
    }

    else if ( !strcmp( key, "here" ) )
    {
        o = namespace__undefine( nso, name, environment__manager( c->env ) );
        name__push( name, key );
    }

    else
    {
        name__push( name, key );
        o = namespace__undefine( nso, name, environment__manager( c->env ) );
    }

    return o;
}


Object *
compiler__resolve( Compiler *c, Name *name )
{
    Namespace_o *nso;
    Object *o;

    char *key = ( char* ) name__pop( name );
    if ( !strcmp( key, "root" ) )
    {
        nso = environment__root( c->env );
        o = resolve( nso, name, environment__manager( c->env ) );
        name__push( name, key );
    }

    else if ( !strcmp( key, "here" ) )
    {
        nso = c->cur_ns_obj;
        o = resolve( nso, name, environment__manager( c->env ) );
        name__push( name, key );
    }

    else
    {
        nso = c->cur_ns_obj;
        name__push( name, key );

        o = resolve( nso, name, environment__manager( c->env ) );

        /* Try the root namespace secondly. */
        if ( !o )
            o = resolve( environment__root( c->env ), name, environment__manager( c->env ) );
    }

    if ( !o )
        undef_error( name );

    return o;
}


/******************************************************************************/


Name *
compiler__name_of( Compiler *c, Namespace_o *nso, const Object *o )
{
    unsigned int i;
    char *s;
    Name *name;
    Memory_Manager *m = environment__manager( c->env );

    if ( !nso )
        nso = environment__root( c->env );

    name = namespace__find( nso, o, m );

    /* Simplify the name.
       Note: the result is not necessarily as simple as possible.  For instance,
           an object with two possible fully-qualified names, a:b:c:d and x:y:z,
           might yield the name x:y:z even though */
    if ( name )
    {
        i = 0;
        while ( i < array__size( name ) - 1 )
        {
            s = array__remove( name, i );

            if ( namespace__resolve( nso, name, m ) != o )
            {
                array__insert_before( name, i, s );
                i++;
            }

            else
                free( s );
        }
    }

    return name;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
