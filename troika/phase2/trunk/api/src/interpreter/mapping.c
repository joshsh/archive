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

#include "Interpreter-impl.h"


static Object *
resolve( OBJ( NAMESPACE ) *nso, Name *name, Manager *m )
{
    return ( COMPILER__NAME_INHERITANCE )
        ? namespace__resolve( nso, name, m )
        : namespace__lookup( nso, name );
}


Object *
interpreter__define( Interpreter *c, Name *name, Object *o )
{
    OBJ( NAMESPACE ) *nso;

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
        err_notdefined( name );
*/

    return o;
}


Object *
interpreter__undefine( Interpreter *c, Name *name )
{
    OBJ( NAMESPACE ) *nso;
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
interpreter__resolve( Interpreter *c, Name *name )
{
    OBJ( NAMESPACE ) *nso;
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
        err_notdefined( name );

    return o;
}


/******************************************************************************/


/* Simplify the name.
   Note: the result is not necessarily as simple as possible.  For instance,
         an object with two possible fully-qualified names, a:b:c:d and x:y:z,
         might yield the name x:y:z even though [...] */
Name *
interpreter__name_of( Interpreter *c, OBJ( NAMESPACE ) *nso, const Object *o )
{
    unsigned int i = 0;
    char *s;

    Name *name = interpreter__name_of__full( c, nso, o );

    if ( name )
    {
        while ( i < array__size( name ) - 1 )
        {
            s = array__remove( name, i );

            if ( interpreter__resolve( c, name) != o )
            /*if ( namespace__resolve( nso, name, m ) != o )*/
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


Name *
interpreter__name_of__full( Interpreter *c, OBJ( NAMESPACE ) *nso, const Object *o )
{
    Name *name;
    Object *root = environment__root( c->env );
    Manager *m = environment__manager( c->env );

    /* Default to the working namespace. */
    if ( !nso )
        nso = c->cur_ns_obj;

    name = namespace__find( nso, o, m );

    /* If the object is not found in the current namespace, try the root
       namespace. */
    if ( !name )
    {
        name = namespace__find( root, o, m );

        /* FIXME: possible collision with a namespace item "root" */
        if ( name )
            array__push( name, STRDUP( "root" ) );

        /* FIXME: possible collision with a namespace item "root" */
        if ( o == root )
        {
            name = name__new();
            array__push( name, STRDUP( "root" ) );
        }
    }

    return name;
}


#define LABEL__MAXLEN  32

/* FIXME: maxlen does nothing.  Eventually, use a String object instead of
   a char array to build the label */
void
interpreter__encode( Interpreter *c, const Object *o, char *buffer, unsigned int maxlen )
{
    if ( DEBUG__SAFE && !c )
        abort();

    if ( o )
    {
        if ( !maxlen )
            maxlen = LABEL__MAXLEN;

        Name *name = interpreter__name_of( c, c->cur_ns_obj, o );

        if ( !name )
            object__encode( o, buffer );

        else
            name__encode( name, buffer );
    }

    else
        sprintf( buffer, "()" );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
