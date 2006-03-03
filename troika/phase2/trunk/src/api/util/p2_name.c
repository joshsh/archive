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

#include "p2_name.h"
#include <stdio.h>


void p2_name__print( p2_name *n )
{
    char *s;
    int i, size = n->size;

    #if DEBUG__SAFE
    if ( !n )
    {
        PRINTERR( "p2_name__print: null name" );
        return;
    }
    #endif

    for ( i = 0; i < size; i++ )
    {
        if ( i )
            printf( ":" );

        s = ( char* ) p2_array__get( n, i );

        while ( *s )
        {
            if ( *s == ' ' )
                printf( "\\" );

            printf( "%c", *s );
            s++;
        }
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
