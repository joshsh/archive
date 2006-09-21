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

#include <phase2/collection/String.h>
#include "../settings.h"


struct String
{
    /** The buffer array. */
    char *buffer;

    /** The size of the occupied portion of the buffer. */
    unsigned int size;

    /** The overall size of the buffer. */
    unsigned int buffer_size;

};


/* By default, expand() doubles the size of the array. */
#define DEFAULT_EXPANSION_FACTOR    2.0

#define BUFFER_NEW( size )  malloc( ( size ) * sizeof( char ) )

/** The String expands by this factor whenever it outgrows its buffer.
    Memory copying into the new buffer is expensive, so beware of setting
    the expansion factor too low.
    \note  Has a fixed value for now. */
static double expansion = DEFAULT_EXPANSION_FACTOR;


static char *
buffer_copy( String *s )
{
    char *buffer;
    size_t size = s->buffer_size * sizeof( char );

    if ( ( buffer = malloc( size ) ) )
        memcpy( buffer, s->buffer, size + 1 );

    return buffer /*@null@*/;
}


static unsigned int
max3( unsigned int a, unsigned int b, unsigned int c )
{
    if ( a > b )
        b = a;
    return b > c ? b : c;
}


static String *
sizeup( String *s, unsigned int minsize )
{
    char *buffer_new;

    unsigned int buffer_size_new = max3(
        ( unsigned int ) ( expansion * s->buffer_size ),
        minsize,
        s->buffer_size + 1 );

    if ( !( buffer_new = BUFFER_NEW( buffer_size_new ) ) )
    {
        ERROR( "allocation failure" );
        return 0;
    }

    else
    {
        strcpy( buffer_new, s->buffer );

        free( s->buffer );

        s->buffer = buffer_new;
        s->buffer_size = buffer_size_new;

        return s;
    }
}


/* Constructors and destructor ************************************************/


String *
string__new( unsigned int buffer_size )
{
    String *s;

    if ( !( s = NEW( String ) ) )
        return 0;

    else
    {
        /* Buffer size must be positive. */
        s->buffer_size = ( buffer_size )
            ? buffer_size : 1;

        if ( !( s->buffer = BUFFER_NEW( s->buffer_size ) ) )
        {
            free( s );
            return 0;
        }

        else
        {
            s->size = 0;
            *s->buffer = '\0';

            return s;
        }
    }
}


String *
string__copy( String *s )
{
    String *s_copy;

    if ( !( s_copy = NEW( String ) ) )
        return 0;

    *s_copy = *s;

    if ( !( s_copy->buffer = buffer_copy( s ) ) )
    {
        free( s_copy );
        return 0;
    }

    return s_copy;
}


void
string__delete( String *s )
{
    free( s->buffer );
    free( s );
}


/******************************************************************************/


unsigned int
string__size( const String *s )
{
    return s->size;
}


char *
string__to_cstring( const String *s )
{
    return STRDUP( s->buffer );
}


char *
string__append( String *s, char *cstring )
{
    unsigned int len = s->size + strlen( cstring );

    if ( len >= s->buffer_size && !sizeup( s, len + 1 ) )
        return 0;

    else
    {
        strcpy( s->buffer + s->size, cstring );
        s->size = len;

        return cstring;
    }
}

/*
extern boolean
string__sprintf( String *s, unsigned int minlen, ... )
{
    va_list ap;

    unsigned int len = s->size + minlen;

    if ( len >= s->buffer_size && !sizeup( s, len + 1 ) )
        return TRUE;

    else
    {
        sprintf( s->buffer + s->size, __VA_ARGS__ );
        return FALSE;
    }
}
*/

/* Miscellaneous **************************************************************/


String *
string__clear( String *a )
{
    a->size = 0;
    return a;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
