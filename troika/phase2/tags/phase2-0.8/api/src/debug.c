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

#include "settings.h"


#ifdef calloc
#   undef calloc
#endif
#ifdef free
#   undef free
#endif
#ifdef malloc
#   undef malloc
#endif
#ifdef realloc
#   undef realloc
#endif


static int
    n_calloc    = 0,
    n_free      = 0,
    n_malloc    = 0,
    n_realloc   = 0;


void *
debug__calloc( size_t nelem, size_t elsize, const char *file, int line )
{
    void *p;

    if ( DEBUG__SAFE && ( !nelem || !elsize ) )
        ABORT;

    p = calloc( nelem, elsize );

    n_calloc++;

    if ( DEBUG__ALLOC__VERBOSE )
        PRINT( "CALLOC: %p [%s:%d]\n", p, file, line );

    return p;
}


void *
debug__malloc( size_t size, const char *file, int line )
{
    void *p;

    if ( DEBUG__SAFE && !size )
        ABORT;

    p = malloc( size );
    if ( !p )
        WARNING( "allocation failure" );

    n_malloc++;

    if ( DEBUG__ALLOC__VERBOSE )
        PRINT( "MALLOC: %p [%s:%d]\n", p, file, line );

    return p;
}


void *
debug__realloc( void *ptr, size_t size, const char *file, int line )
{
    void *p;

    if ( DEBUG__SAFE && ( !ptr || !size ) )
        ABORT;

    p = realloc( ptr, size );
    if ( !p )
        WARNING( "allocation failure" );

    n_realloc++;

    if ( DEBUG__ALLOC__VERBOSE )
        PRINT( "REALLOC: %p --> %p [%s:%d]\n", ptr, p, file, line );

    return p;
}


void
debug__free( void *ptr, const char *file, int line )
{
    if ( DEBUG__ALLOC__VERBOSE )
        PRINT( "FREE: %p [%s:%d]\n", ptr, file, line );

    if ( DEBUG__SAFE && !ptr )
        ABORT;

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
    PRINT( "*%i\tnot-freed object(s)\n", n_calloc + n_malloc + n_realloc - n_free );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
