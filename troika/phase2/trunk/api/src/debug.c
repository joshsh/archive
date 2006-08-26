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

#include <stdlib.h>

#define DEBUG__SAFE 1
















/* Terminal *******************************************************************/

/* TODO: USE_NCURSES is not yet supported */
#define USE_NCURSES                             0

#if USE_NCURSES
#    include <ncurses.h>
#    define PRINT                               printw
#    define REFRESH                             refresh
#else
#    define PRINT                               printf
#    define REFRESH
#endif


/* Errors *********************************************************************/


#include <stdio.h>  /* fprintf */

#define FFLUSH  { fflush( stdout ); fflush( stderr ); }

/* Note: the GCC extension is used here instead of __VA_ARGS__ for compatibility
   with old GCC cross-compilers. */

#define ERROR(args...) (                                                    \
    fprintf( stderr, "ERROR: " ),                                           \
    fprintf( stderr , ##args ),                                             \
    fprintf( stderr, "\n" ),                                                \
    fflush( stderr ) )

#define WARNING(args...) (                                                  \
    fprintf( stderr, "WARNING: " ),                                         \
    fprintf( stderr , ##args ),                                             \
    fprintf( stderr, "\n" ),                                                \
    fflush( stderr ) )

#define WARNING__ALLOC WARNING

























static int
    n_calloc    = 0,
    n_free      = 0,
    n_malloc    = 0,
    n_realloc   = 0;

void *
debug__calloc( size_t nelem, size_t elsize )
{
    void *p;

    if ( DEBUG__SAFE && ( !nelem || !elsize ) )
        abort();

    p = calloc( nelem, elsize );

    n_calloc++;

    return p;
}


void *
debug__malloc( size_t size )
{
    void *p;

    if ( DEBUG__SAFE && !size )
        abort();

    p = malloc( size );
    if ( !p )
        WARNING( "allocation failure" );

    n_malloc++;

    return p;
}


void *
debug__realloc( void *ptr, size_t size )
{
    void *p;

    if ( DEBUG__SAFE && ( !ptr || !size ) )
        abort();

    p = realloc( ptr, size );
    if ( !p )
        WARNING( "allocation failure" );

    n_realloc++;

    return p;
}


void
debug__free( void *ptr )
{
    if ( DEBUG__SAFE && !ptr )
        abort();

    free( ptr );

    n_free++;
}


void
debug__memcheck()
{
    PRINT( "%i\tcalls to calloc\n", n_calloc );
    PRINT( "%i\tcalls to malloc\n", n_malloc );
    PRINT( "%i\tcalls to realloc\n", n_realloc );
    PRINT( "%i\tcalls to free\n", n_free );
    PRINT( "===========================\n" );
    PRINT( "%i\tnot-freed object(s)\n", n_calloc + n_malloc + n_realloc - n_free );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
