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


/* TODO: qualify these with a sizeof() criterion. */
#define OCCUPIED(c)     (c)->intval
#define NONZERO(i)      (i).intval
#define EQUALS(i,j)     (i).intval == (j).intval
#define VACATE(c)       (c)->intval = 0
/* Trivial hash should do the trick for pointers... */
#define hash(p)         (p)->intval

#define WRAP(a,c)       (a)->buffer + (((c) - (a)->buffer) % (a)->buffer_size)
#define HASH(a,i)       (a)->buffer + (i).intval % (a)->buffer_size
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

    buffer = calloc( buffer_size, sizeof (index) );

    if ( !buffer )
        goto failure;

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

failure:

    return 0;
}


static void
rehash_affected_cells( ModelVector *a, index *removed )
{
    index *cur = removed, *aux;
    unsigned int i = -1, j;

    do
    {
        i++;
        cur++;
        cur = WRAP( a, cur );

    } while ( OCCUPIED( cur ) );

    if ( i )
    {
/* TODO: use a static buffer, or abuse the call stack */
        aux = malloc( i * sizeof (index) );

        cur = removed;
        for ( j = 0; j < i; j++ )
        {
            cur++;
            cur = WRAP( a, cur );
            *( aux + j ) = *cur;
            VACATE( cur );
            a->size--;
        }

        for ( j = 0; j < i; j++ )
            modelvector__set_component( a, *( aux + j ), TRUE );

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

            a->buffer = calloc( a->buffer_size, sizeof (index) );
            if ( !a->buffer )
            {
                free( a );
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
        abort();

    b = NEW( ModelVector );

    if ( b )
    {
        *b = *a;

        if ( MODELVECTOR__INITIAL_BUFFER_SIZE || a->buffer_size )
        {
            b->buffer = malloc( sizeof (index) * a->buffer_size );

            if ( !b->buffer )
            {
                free( b );
                return 0;
            }

            else
                memcpy( b->buffer, a->buffer, sizeof (index) * a->buffer_size );
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

    if ( DEBUG__SAFE && ( !a || !NONZERO( i ) ) )
        abort();

    cur = HASH( a, i );

    while ( OCCUPIED( cur ) )
    {
        if ( EQUALS( *cur, i ) )
            return TRUE;

        cur++;
        cur = WRAP( a, cur );
    }

    return FALSE;
}


ModelVector *
modelvector__set_component( ModelVector *a, index i, boolean b )
{
    index *cur;

    if ( DEBUG__SAFE && ( !a || !NONZERO( i ) ) )
        abort();

    cur = HASH( a, i );

    while ( OCCUPIED( cur ) )
    {
        if ( EQUALS( *cur, i ) )
        {
            if ( !b )
            {
                VACATE( cur );
                a->size--;
                rehash_affected_cells( a, cur );
            }

            goto success;
        }

        cur++;
        cur = WRAP( a, cur );
    }

    if ( b )
    {
        *cur = i;
        a->size++;
        if ( a->size >= a->capacity
          && !expand( a ) )
            goto failure;
    }

success:

    return a;

failure:

    return 0;
}


unsigned int
modelvector__manhattan_norm( ModelVector *a )
{
    if ( DEBUG__SAFE && !a )
        abort();

    return a->size;
}


ModelVector *
modelvector__add( ModelVector *a, ModelVector *b )
{
    ModelVector *c, *tmp;

    ACTION add( index i )
    {
        modelvector__set_component( c, i, TRUE );
        return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !a || !b ) )
        abort();

    /* Copy the larger vector and iterate through the small one. */
    if ( b->size > a->size )
    {
        tmp = a;
        a = b;
        b = tmp;
    }

    c = modelvector__copy( a );

    if ( c )
        modelvector__walk( b, ( VisitorNew ) add );

    return c;
}


ModelVector *
modelvector__subtract( ModelVector *a, ModelVector *b )
{
    ModelVector *c;

    ACTION subtract( index i )
    {
        modelvector__set_component( c, i, FALSE );
        return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !a || !b ) )
        abort();

    c = modelvector__copy( a );

    if ( c )
        modelvector__walk( b, subtract );

    return c;
}


ModelVector *
modelvector__multiply( ModelVector *a, ModelVector *b )
{
    ModelVector *c, *tmp;

    ACTION multiply( index i )
    {
        if ( !modelvector__get_component( b, i ) )
            modelvector__set_component( c, i, FALSE );

        return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !a || !b ) )
        abort();

    /* Copy the smaller vector and iterate through it. */
    if ( b->size < a->size )
    {
        tmp = a;
        a = b;
        b = tmp;
    }

    c = modelvector__copy( a );

    if ( c )
        modelvector__walk( a, multiply );

    return c;
}


ModelVectorDiff *
modelvector__diff( ModelVector *a, ModelVector *b )
{
    __label__ failure;

    ModelVector *cmp, *result;

    ACTION diffop( index i )
    {
        if ( !modelvector__get_component( cmp, i )
          && !modelvector__set_component( result, i, TRUE ) )
            goto failure;

        return CONTINUE;
    }

    ModelVectorDiff *diff = 0;
    ModelVector *addend = 0, *subtrahend = 0;

    diff = NEW( ModelVectorDiff );
    if ( !diff )
        goto failure;

    addend = modelvector__new();
    if ( !addend )
        goto failure;

    subtrahend = modelvector__new();
    if ( !subtrahend )
        goto failure;

    cmp = b;
    result = subtrahend;
    modelvector__walk( a, ( VisitorNew ) diffop );

    cmp = a;
    result = addend;
    modelvector__walk( b, diffop );

    diff->addend = addend;
    diff->subtrahend = subtrahend;
    return diff;

failure:

    if ( addend )
        free( addend );
    if ( subtrahend )
        free( subtrahend );
    if ( diff )
        free( diff );
    return 0;
}


void
modelvector__walk( ModelVector *a, VisitorNew v )
{
    index *cur, *lim;

    if ( DEBUG__SAFE && ( !a || !v ) )
        abort();

    cur = a->buffer;
    lim = a->buffer + a->buffer_size;

    while ( cur < lim )
    {
        if ( OCCUPIED( cur ) && BREAK == v( *cur ) )
            break;

        cur++;
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
