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


Namespace *
namespace__new()
{
    Namespace *ns;
    Dictionary *d = dictionary__new();

    if ( !d )
        ns = 0;

    else
    {
        ns = NEW( Namespace );

        if ( ns )
            ns->children = d;
    }

    return ns;
}


void
namespace__delete( Namespace *ns )
{
    if ( DEBUG__SAFE && !ns )
        abort();

    dictionary__delete( ns->children );

    free( ns );
}


unsigned int
namespace__size( Namespace *ns )
{
    if ( DEBUG__SAFE && !ns )
        abort();

    return hash_table__size( ns->children );
}


Array *
namespace__keys( Namespace *ns )
{
    if ( DEBUG__SAFE && !ns )
        abort();

    return dictionary__keys( ns->children );
}


void
namespace__walk( Namespace *ns, Dist_f f )
{
    if ( DEBUG__SAFE && ( !ns || !f ) )
        abort();

    dictionary__walk( ns->children, f );
}


/******************************************************************************/


void
namespace__encode( Namespace *ns, char *buffer )
{
    Dictionary *dict;
    unsigned int size;
    Array *keys;
    int maxlen = 0;

    void *find_maxlen( Object **opp )
    {
        int len = strlen( object__type( *opp )->name );
        if ( len > maxlen )
            maxlen = len;
        return 0;
    }

    void *print( char **key )
    {
        Object *o = dictionary__lookup( dict, *key );
        int i, lim = maxlen - strlen( object__type( o )->name );

        sprintf( buffer, "    " );
        buffer += strlen( buffer );

        if ( COMPILER__SHOW_ADDRESS )
        {
            sprintf( buffer, "%p ", ( void* ) o );
            buffer += strlen( buffer );
        }

        sprintf( buffer, "<%s> ", object__type( o )->name );
        buffer += strlen( buffer );

        for ( i = 0; i < lim; i++ )
        {
            sprintf( buffer, " " );
            buffer++;
        }

        sprintf( buffer, "%s\n", *key );
        buffer += strlen( buffer );

        return 0;
    }

    if ( DEBUG__SAFE && ( !buffer || !ns ) )
        abort();

    dict = ns->children;
    size = hash_table__size( dict );

    if ( size )
    {
        sprintf( buffer, "\n{\n" );
        buffer += 3;

        /* Get alphabetized dictionary keys. */
        keys = dictionary__keys( dict );

        namespace__walk( ns, ( Dist_f ) find_maxlen );

        /* Print children. */
        array__walk( keys, ( Dist_f ) print );
        array__delete( keys );

        sprintf( buffer, "}" );
    }

    else
        sprintf( buffer, "{}" );
}


/******************************************************************************/


Type *
namespace__create_type( const char *name, int flags )
{
    Type *t = type__new( name, flags );

    if ( t )
    {
        t->destroy = ( Destructor ) namespace__delete;
        t->encode = ( Encoder ) namespace__encode;
        t->size = ( Size_Of ) namespace__size;
        t->walk = ( Walker ) namespace__walk;
    }

    return t;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
