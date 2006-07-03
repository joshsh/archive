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

#include "Namespace-impl.h"


Object *
namespace__add( Namespace_o *ns_obj, Name *name, Object *o )
{
    Namespace *ns = object__value( ns_obj );

    Object *child_ns_obj;
    char *key;

    if ( DEBUG__SAFE && ( !ns || !name || !array__size( name ) ) )
        abort();

    if ( array__size( name ) == 1 )
    {
        if ( object__immutable( ns_obj ) )
        {
            ERROR( "namespace__add: namespace is write-protected" );
            o = 0;
        }

        else
            dictionary__add( ns->children, array__peek( name ), o );
    }

    else
    {
        key = array__pop( name );

        child_ns_obj = dictionary__lookup( ns->children, key );

        if ( object__type( child_ns_obj ) != object__type( ns_obj ) )
        {
            ERROR( "namespace__add: not a namespace" );
            o = 0;
        }

        else
            o = namespace__add( child_ns_obj, name, o );

        array__push( name, key );
    }

    return o;
}


Object *
namespace__add_simple( Namespace *ns, const char *name, Object *o )
{
    if ( DEBUG__SAFE && ( !ns|| !name ) )
        abort();

    return dictionary__add( ns->children, name, o );
}


void
namespace__add_all( Namespace *dest, Namespace *src )
{
    dictionary__add_all( dest->children, src->children );
}


Object *
namespace__lookup( Namespace_o *ns_obj, Name *name )
{
    Namespace *ns;
    Object *o;
    char *key;

    if ( DEBUG__SAFE && !ns_obj )
        abort();

    ns = object__value( ns_obj );

    if ( !name || !array__size( name ) )
        return ns_obj;

    o = ( Object* ) dictionary__lookup( ns->children, array__peek( name ) );

    /* Look for the object in a nested namespace. */
    if ( o && array__size( name ) > 1 )
    {
        /* Always check for this error, as namespace references may come directly
           from the user. */
        if ( object__type( o ) != object__type( ns_obj ) )
        {
            ERROR( "namespace__lookup: not a namespace" );
            o = 0;
        }

        else
        {
            key = ( char* ) array__pop( name );
            o = namespace__lookup( o, name );
            array__push( name, key );
        }
    }

    return o;
}


Object *
namespace__lookup_simple( Namespace *ns, const char *name )
{
    if ( DEBUG__SAFE && ( !ns | !name ) )
        abort();

    return ( Object* ) dictionary__lookup( ns->children, name );
}


Object *
namespace__remove( Namespace_o *ns_obj, Name *name )
{
    Namespace *ns;

    Object *child_ns_obj, *o;
    char *key;

    if ( DEBUG__SAFE && ( !ns_obj || !name || !array__size( name ) ) )
        abort();

    ns = object__value( ns_obj );

    if ( array__size( name ) == 1 )
    {
        if ( object__immutable( ns_obj ) )
        {
            ERROR( "namespace__remove: namespace is write-protected" );
            return 0;
        }

        o = dictionary__remove( ns->children, ( char* ) array__peek( name ) );
    }

    else
    {
        key = ( char* ) array__pop( name );

        child_ns_obj = dictionary__lookup( ns->children, key );

        if ( !child_ns_obj )
        {
            ERROR( "namespace__remove: namespace does not exist" );
            o = 0;
        }

        else if ( object__type( child_ns_obj ) != object__type( ns_obj ) )
        {
            ERROR( "namespace__remove: not a namespace" );
            o = 0;
        }

        else
            o = namespace__remove( child_ns_obj, name );

        array__push( name, key );
    }

    return o;
}


Object *
namespace__remove_simple( Namespace *ns, char *name )
{
    if ( DEBUG__SAFE && ( !ns || !name) )
        abort();

    return dictionary__remove( ns->children, name );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
