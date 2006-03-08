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


/* By default, the hash table will wait until it is 1/3 full before expanding. */
#define DEFAULT_SPARSITY_FACTOR     3

/* By default, expand() approximately doubles the size of the buffer. */
#define DEFAULT_EXPANSION_FACTOR    2


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


static p2_hash_table *expand( p2_hash_table *h )
{
    void **src, **dest, **buffer, **sup;
printf( "h->buffer_size = %i\n", h->buffer_size ); fflush( stdout );
printf( "h->size = %i\n", h->size ); fflush( stdout );
printf( "h->capacity = %i\n", h->capacity ); fflush( stdout );

    int buffer_size = next_prime(
        ( unsigned int ) ( h->buffer_size * h->expansion ) );
printf( "buffer_size = %i\n", buffer_size ); fflush( stdout );

    if ( !( buffer = buffer_new( buffer_size ) ) )
        return 0;

    sup = buffer + buffer_size;
    for ( dest = buffer; dest < sup; dest++ )
        *dest = 0;

    sup = h->buffer + h->buffer_size;
printf( "sup - h->buffer = %i\n", ( unsigned int ) ( sup - h->buffer ) );
    for ( src = h->buffer; src < sup; src++ )
    {
        if ( *src )
        {
            dest = buffer + ( h->hash( *src ) % buffer_size );
            while ( *dest )
            {
                dest++;
                dest = buffer + ( ( unsigned int ) ( dest - buffer ) % buffer_size );
            }

            *dest = *src;
        }
    }

    free( h->buffer );
    h->buffer = buffer;
    h->buffer_size = buffer_size;
    h->capacity = buffer_size / h->sparsity;
printf( "h->capacity (new) = %i\n", h->capacity ); fflush( stdout );

    return h;
}


/******************************************************************************/


p2_hash_table *p2_hash_table__new(
    unsigned int buffer_size,
    unsigned int sparsity,
    unsigned int expansion,
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

        /* Capacity is re-calculated whenever the table resizes.
           Note: capacity may initially be 0, which just means that the hash
           table will expand as soon as it is added to. */
        h->capacity = h->buffer_size / h->sparsity;

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
int i=0;
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
if (++i > 10)
{
  printf( "cur = %i\n", ( int ) cur );
  printf( "buffer = %i\n", ( int ) buffer );
  printf( "buffer_size = %i\n", buffer_size );
  printf( "h->size = %i, h->capacity = %i\n", h->size, h->capacity );
  exit(0);
}
        /* No duplicate entries allowed.  Replace with new key. */
        if ( !h->compare( *cur, key ) )
        {
            /* Table doesn't grow. */
            h->size--;

            break;
        }

        /* Increment and wrap */
        cur++;
        cur = buffer + ( ( unsigned int ) ( cur - buffer ) % buffer_size );
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
int i=0;
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
if (++i > 10)
{
  printf( "cur = %i\n", ( int ) cur );
  printf( "buffer = %i\n", ( int ) buffer );
  printf( "buffer_size = %i\n", buffer_size );
  printf( "h->size = %i, h->capacity = %i\n", h->size, h->capacity );
  exit(0);
}
        /* Increment and wrap. */
        cur++;
        cur = buffer + ( ( unsigned int ) ( cur - buffer ) % buffer_size );
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
        cur = buffer + ( ( unsigned int ) ( cur - buffer ) % buffer_size );
    }

    return *cur;
}


void p2_hash_table__distribute( p2_hash_table *h, p2_procedure *p )
{
    void **cur = h->buffer;
    void **sup = h->buffer + h->buffer_size;
    p2_action *action;

    while ( cur < sup )
    {
        if ( *cur )
        {
            if ( ( action = p2_procedure__execute( p, *cur ) ) )
            {
                switch ( action->type )
                {
                    case p2_action__type__break:

                        return;

                    case p2_action__type__remove:

                        *cur = 0;
                        h->size--;
                        break;

                    case p2_action__type__replace:

                        *cur = action->value;
                        break;

                    default:

                        ;
                }
            }
        }

        cur++;
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
