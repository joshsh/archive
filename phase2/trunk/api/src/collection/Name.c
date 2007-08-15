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

#include <phase2/Name.h>
#include "../settings.h"


Name *
name__new( void )
{
    Name *name = array__new( 0, 0 );

    return name;
}


void
name__free( Name *name )
{
    ACTION helper( char **sp )
    {
        free( *sp );
        return CONTINUE;
    }

    if ( DEBUG__SAFE && !name )
        abort();

    array__walk( name, ( Visitor ) helper );

    array__free( name );
}


void
name__append( Name *name, const char *key )
{
    array__enqueue( name, STRDUP( key ) );
}


void
name__encode( Name *name, char *buffer )
{
    char *s;
    int i, size = array__size( name );

    if ( DEBUG__SAFE && !name )
        abort();

    for ( i = 0; i < size; i++ )
    {
        if ( i )
        {
            sprintf( buffer, ":" );
            buffer++;
        }

        s = ( char* ) array__get( name, i );

        while ( *s )
        {
            if ( *s == ' ' )
            {
                sprintf( buffer, "\\" );
                buffer++;
            }

            sprintf( buffer, "%c", *s );
            buffer++;
            s++;
        }
    }
}


void
name__print( Name *n )
{
    char buffer[ENCODING__BUFFER_SIZE];

    name__encode( n, buffer );

    PRINT( buffer );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
