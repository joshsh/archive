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


p2_name *p2_name__new()
{
    p2_name *name = p2_array__new( 0, 0 );
    return name;
}


static p2_action * delete_string( char *s, void *ignored )
{
printf( "---name ds 1---\n" ); fflush( stdout );

    free( s );
    return 0;
}


void p2_name__delete( p2_name *name )
{
    p2_procedure p = { ( procedure ) delete_string, 0 };
printf( "---name d 1---\n" ); fflush( stdout );

    #if DEBUG__SAFE
    if ( !name )
    {
        PRINTERR( "p2_name__delete: null name" );
        return;
    }
    #endif

    #if DEBUG__NAME
    printf( "p2_name__delete(0x%X)\n", ( int ) name );  fflush( stdout );
    #endif
printf( "---name d 2---\n" ); fflush( stdout );
    p2_array__distribute( name, &p );
printf( "---name d 3---\n" ); fflush( stdout );

    p2_array__delete( name );
printf( "---name d 4---\n" ); fflush( stdout );

}


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
