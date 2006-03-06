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

#include "p2_hash_table.h"

#include <string.h>  /* strcmp */


/* By default, the hash table will wait until it is 1/3 full before expanding.
   Note: the sparsity factor does not need to be an integer. */
#define DEFAULT_SPARSITY_FACTOR 3.0

/* By default, expand() approximately floats the size of the buffer.
   Note: the expansion factor does not need to be an integer. */
#define DEFAULT_EXPANSION_FACTOR 2.0


#define buffer_new( size )  ( void** ) malloc( size * sizeof( void* ) )


/******************************************************************************/


/** \return the least prime > 2 and >= i. */
static unsigned int next_prime( unsigned int i )
{
    int j;

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


static p2_procedure__effect rehash( void **key_p, p2_hash_table *h )
{
    p2_hash_table__add( h, *key_p );
    return p2_procedure__effect__continue;
}


static p2_hash_table *expand( p2_hash_table *h )
{
    int i;

    p2_hash_table h_new = *h;
    h_new.size = 0;
    h_new.buffer_size = ( int ) ( h->buffer_size * h->expansion );
    if ( h_new.buffer_size <= h->buffer_size )
        h_new.buffer_size = ( int ) ( h->buffer_size * DEFAULT_EXPANSION_FACTOR );
    h_new.buffer_size = next_prime( h_new.buffer_size );
    h_new.capacity = ( int ) ( ( ( float ) h_new.buffer_size ) / h_new.sparsity);

    if ( !( h_new.buffer = buffer_new( h_new.buffer_size ) ) )
        return 0;

    for ( i = 0; i < h_new.buffer_size; i++ )
        h_new.buffer[i] = 0;

    p2_procedure p = { ( procedure ) rehash, &h };
    p2_hash_table__distribute( h, &p );

    free( h->buffer );
    *h = h_new;

    return h;
}


/******************************************************************************/


p2_hash_table *p2_hash_table__new(
    unsigned int buffer_size,
    float sparsity,
    float expansion,
    hash_f hash,
    comparator compare )
{
    int i;

    p2_hash_table *h;

    if ( ( h = new( p2_hash_table ) ) )
    {
        h->buffer_size = next_prime( buffer_size );

        h->hash = hash;
        h->compare = compare;

        /* Sparsity must be at least 1, otherwise the buffer will not resize
           even when it is completely full. */
        h->sparsity = ( sparsity >= 1 ) ? sparsity : DEFAULT_SPARSITY_FACTOR;

        /* Expansion factor must be greater than 1 for the buffer to actually
           gain in size. */
        h->expansion = ( expansion > 1 ) ? expansion : DEFAULT_EXPANSION_FACTOR;

        /* Buffer is initially empty. */
        h->size = 0;

        /* Capacity is re-calculated whenever the table resizes. */
        h->capacity = ( int ) ( ( ( float ) h->buffer_size ) / h->sparsity );

        if ( !( h->buffer = buffer_new( h->buffer_size ) ) )
        {
            free( h );
            h = 0;
        }

        else
            for ( i = 0; i < h->buffer_size; i++ )
                h->buffer[i] = 0;
    }

    if ( !h )
        PRINTERR( "p2_hash_table__new: allocation failure" );

    return h;
}


p2_hash_table *p2_hash_table__copy( const p2_hash_table *h )
{
    p2_hash_table *h2;

    if ( ( h2 = new( p2_hash_table ) ) )
    {
        *h2 = *h;

        if ( !( h2->buffer = buffer_new( h->buffer_size ) ) )
        {
            free( h2 );
            h2 = 0;
        }

        else
            memcpy( h2->buffer, h->buffer, h->buffer_size * sizeof( void* ) );
    }

    if ( !h2 )
        PRINTERR( "p2_hash_table__copy: allocation failure" );

    return h2;
}


void p2_hash_table__delete( p2_hash_table *h )
{
    free( h->buffer );
    free( h );
}


void *p2_hash_table__add( p2_hash_table *h, void *key )
{
    void **cur, **buffer = h->buffer;
    int buffer_size = h->buffer_size;

    void *key_old;

    #if DEBUG__SAFE
    if ( !key )
    {
        PRINTERR( "p2_hash_table__add: null key" );
        return 0;
    }
    #endif

    cur = buffer + ( h->hash( key ) % buffer_size );

    while ( *cur )
    {
        /* No duplicate entries allowed.  Replace with new key. */
        if ( !h->compare( *cur, key ) )
        {
            /* Table doesn't grow. */
            h->size--;

            break;
        }

        /* Increment and wrap */
        cur++;
        cur = buffer + ( ( unsigned int ) cur % buffer_size );
    }

    key_old = *cur;
    *cur = key;

    h->size++;
    if ( h->size >= h->capacity )
        /* Warning: no checking of return value. */
        expand( h );

    return key_old;
}


void *p2_hash_table__lookup( p2_hash_table *h, const void *key )
{
    void **cur, **buffer = h->buffer;
    int buffer_size = h->buffer_size;

    #if DEBUG__SAFE
    if ( !key )
    {
        PRINTERR( "p2_hash_table__lookup: null key" );
        return 0;
    }
    #endif

    cur = buffer + ( h->hash( key ) % buffer_size );

    while ( *cur && h->compare( *cur, key ) )
    {
        /* Increment and wrap. */
        cur++;
        cur = buffer + ( ( unsigned int ) cur % buffer_size );
    }

    return *cur;
}


void *p2_hash_table__remove(p2_hash_table *h, const void *key)
{
    void **cur, **buffer = h->buffer;
    int buffer_size = h->buffer_size;

    #if DEBUG__SAFE
    if ( !key )
    {
        PRINTERR( "p2_hash_table__remove: null key" );
        return 0;
    }
    #endif

    cur = buffer + ( h->hash( key ) % buffer_size );

    while ( *cur )
    {
        if ( !h->compare( *cur, key ) )
        {
            h->size--;
            break;
        }

        /* Increment and wrap. */
        cur++;
        cur = buffer + ( ( unsigned int ) cur % buffer_size );
    }

    return *cur;
}


void p2_hash_table__distribute( p2_hash_table *h, p2_procedure *p )
{
    void **cur = h->buffer;
    void **sup = h->buffer + h->buffer_size;

    while ( cur < sup )
    {
        if ( *cur )
        {
            switch ( p2_procedure__execute( p, cur ) )
            {
                case p2_procedure__effect__continue:
                    break;
                case p2_procedure__effect__break:
                    return;
                case p2_procedure__effect__remove:
                    *cur++ = 0;
                    *cur-- = 0;
                    break;
                default:
                    ;
            }
        }

        cur += 2;
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
