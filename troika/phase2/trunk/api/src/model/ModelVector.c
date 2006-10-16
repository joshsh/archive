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


#include "Model-impl.h"


static int cell_size = sizeof( index );

/* TODO: qualify these with a sizeof() criterion. */
#define OCCUPIED(c)     *(c).intval
#define VACATE(c)       *(c).intval = 0
/* Trivial hash should do the trick for pointers... */
#define hash(p)         (p)->intval

#define WRAP(a,c)       (a)->buffer + (((c) - (a)->buffer) % (a)->buffer_size)
#define HASH(a,k)       (a)->buffer + hash(a) % (a)->buffer_size
#define SET_CAPACITY(a) (a)->capacity = (unsigned int) ( (a)->buffer_size * MODELVECTOR__LOAD_FACTOR )


/******************************************************************************/


/* FIXME: this is slow... */
static unsigned int
next_prime( unsigned int i )
{
    unsigned int j;

    if ( i <= 3 )
        return 3;

    else if ( !( i % 2 ) )
        i++;

    for (;;)  /* Breaks out when next prime is found. */
    {
        for ( j = 3; j < i; j += 2 )
            if ( !( i % j ) )
                j = i + 1;

        if ( j > i )
            i += 2;
        else
            return i;
   }
}


static ModelVector *
expand( ModelVector *a )
{
    unsigned int buffer_size;
    index *src, *dest, *buffer, *lim;

    buffer_size = next_prime(
        ( unsigned int ) ( a->buffer_size * MODELVECTOR__EXPANSION_FACTOR ) );

    if ( !MODELVECTOR__INITIAL_BUFFER_SIZE || MODELVECTOR__LOAD_FACTOR < 2 )
    {
        if ( buffer_size <= a->buffer_size )
            buffer_size = next_prime( a->buffer_size + 2 );
    }

    buffer = calloc( buffer_size, cell_size );

    if ( buffer )
    {
        if ( a->size )
        {
            lim = a->buffer + a->buffer_size;

            for ( src = a->buffer; src < lim; src++ )
            {
                if ( OCCUPIED( src ) )
                {
                    dest = buffer + hash( src ) % buffer_size;

                    while ( OCCUPIED( dest ) )
                    {
                        dest++;
                        dest = buffer + ( unsigned int ) ( dest - buffer ) % buffer_size;
                    }

                    *dest = *src;
                }
            }
        }

        if ( MODELVECTOR__INITIAL_BUFFER_SIZE || a->buffer_size )
            free( a->buffer );

        a->buffer = buffer;
        a->buffer_size = buffer_size;
        a->capacity = ( unsigned int ) ( buffer_size * MODELVECTOR__LOAD_FACTOR );

        return a;
    }

    else
        return 0;
}


static void
rehash_affected_cells( Hash_Table *a, index *removed )
{
    index *cur = removed, *aux;
    unsigned int i = -1, j;

    do
    {
        i++;
        cur = WRAP( a, ++cur );

    } while ( *cur );

    if ( i )
    {
/* TODO: use a static buffer, or abuse the call stack */
        aux = malloc( i * cell_size );

        cur = removed;
        for ( j = 0; j < i; j++ )
        {
            cur = WRAP( a, ++cur );
            *( aux + j ) = *cur;
            VACATE( cur );
            a->size--;
        }

        for ( j = 0; j < i; j++ )
            modelvector__set_component( a, aux + j );

        free( aux );
    }
}


/******************************************************************************/


ModelVector *
modelvector__new()
{
    ModelVector *a = NEW( ModelVector );

    if ( a )
    {
        if ( MODELVECTOR__INITIAL_BUFFER_SIZE )
        {
            a->buffer_size = MODELVECTOR__INITIAL_BUFFER_SIZE;

            a->buffer = calloc( h->buffer_size, h->cell_size );
            if ( !a->buffer )
            {
                free a;
                return 0;
            }
        }

        else
            a->buffer_size = 0;

        SET_CAPACITY( a );
        a->size = 0;
    }

    return a;
}


ModelVector *
modelvector__copy( ModelVector *a )
{
    ModelVector *b;

    if ( DEBUG__SAFE && !a )
        ABORT;

    b = NEW( Hash_Table );

    if ( b )
    {
        *b = *a;

        if ( MODELVECTOR__INITIAL_BUFFER_SIZE || a->buffer_size )
        {
            b->buffer = malloc( cell_size * a->buffer_size );

            if ( !b->buffer )
            {
                free( b );
                return 0;
            }

            else
                memcpy( b->buffer, a->buffer, cell_size * a->buffer_size );
        }
    }

    return b;
}


void
modelvector__free( ModelVector *a )
{
    if ( MODELVECTOR__INITIAL_BUFFER_SIZE || a->buffer_size )
        free( a->buffer );

    free( a );
}


boolean
modelvector__get_component( ModelVector *a, index i )
{
    index *cur;

    if ( DEBUG__SAFE && ( !a || !i ) )
        ABORT;

    cur = HASH( a, i );

    while ( OCCUPIED( cur ) )
    {
        if ( *cur == i )
            return TRUE;

        cur = WRAP( a, ++cur );
    }

    return FALSE;
}


void
modelvector__set_component( ModelVector *a, index i, boolean b )
{
    index *cur;

    if ( DEBUG__SAFE && ( !a || !i ) )
        ABORT;

    cur = HASH( a, i );

    while ( OCCUPIED( cur ) )
    {
        if ( *cur == i )
        {
            if ( !b )
            {
                VACATE( cur );
                a->size--;
                rehash_affected_cells( a, cur );
            }

            return;
        }

        cur = WRAP( a, ++cur );
    }

    if ( b )
    {
        *cur = i;
        a->size++;
        if ( a->size >= a->capacity )
/* TODO: check return value */
            expand( a );
    }
}


unsigned int
modelvector__manhattan_norm( ModelVector *a )
{
    return a->size;
}


ModelVector *
modelvector__add( ModelVector *a, ModelVector *b )
{
    ModelVector *c, *tmp;

    if ( DEBUG__SAFE && ( !a || !b ) )
        ABORT;

    ACTION add(
    {
...










...
    }

    if ( b->size > a->size )
    {
        tmp = a;
        a = b;
        b = tmp;
    }

    c = modelvector__copy( a );

    if ( c )
        modelvector__walk( b, ( Visitor ) add );

    return c;
}


ModelVector *
modelvector__subtract( ModelVector *a, ModelVector *b )
{

}


ModelVector *
modelvector__multiply( ModelVector *a, ModelVector *b )
{

}


ModelVectorDiff *
modelvector__diff( ModelVector *a, ModelVector *b )
{

}


#endif  /* VECTOR_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
