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

#include <util/Hash_Table.h>


/* By default, the hash table will wait until it is 1/3 full before expanding. */
#define DEFAULT_SPARSITY_FACTOR     3

/* By default, expand() approximately doubles the size of the buffer. */
#define DEFAULT_EXPANSION_FACTOR    2


#define buffer_new( size )  calloc( (size), sizeof( void* ) )


struct Hash_Table
{
    unsigned int size;

    /** The number of occupied cells the buffer can hold before the hash table
        becomes too dense. */
    unsigned int capacity;

    unsigned int sparsity;

    unsigned int expansion;

    /** The hash table buffer array. */
    void **buffer;

    /** The number of cells the buffer array. */
    unsigned int buffer_size;

    /** A hashing function specific to the table's "key" type. */
    hash_f hash;

    /** A comparison function for key values. */
    Comparator compare;
};


/******************************************************************************/


/** \return the least prime > 2 and >= i. */
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


static Hash_Table *
expand( Hash_Table *h )
{
    void **src, **dest, **buffer, **lim;
    int buffer_size;

    buffer_size = next_prime(
        ( unsigned int ) ( h->buffer_size * h->expansion ) );

printf( "h->buffer_size = %i\n", h->buffer_size ); fflush( stdout );
printf( "h->size = %i\n", h->size ); fflush( stdout );
printf( "h->capacity = %i\n", h->capacity ); fflush( stdout );
printf( "buffer_size = %i\n", buffer_size ); fflush( stdout );

    if ( !( buffer = buffer_new( buffer_size ) ) )
        return 0;

    lim = buffer + buffer_size;
    for ( dest = buffer; dest < lim; dest++ )
        *dest = 0;

    lim = h->buffer + h->buffer_size;
printf( "lim - h->buffer = %i\n", ( unsigned int ) ( lim - h->buffer ) );
    for ( src = h->buffer; src < lim; src++ )
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


Hash_Table *
hash_table__new(
    unsigned int buffer_size,
    unsigned int sparsity,
    unsigned int expansion,
    hash_f hash,
    Comparator compare )
{
    unsigned int i;

    Hash_Table *h;

    if ( ( h = new( Hash_Table ) ) )
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
        ERROR( "hash_table__new: allocation failure" );

    return h;
}


Hash_Table *
hash_table__copy( const Hash_Table *h )
{
    Hash_Table *h2;

    if ( ( h2 = new( Hash_Table ) ) )
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
        ERROR( "hash_table__copy: allocation failure" );

    return h2;
}


void
hash_table__delete( Hash_Table *h )
{
    free( h->buffer );
    free( h );
}


unsigned int
hash_table__size( const Hash_Table *h )
{
    return h->size;
}


void *
hash_table__add( Hash_Table *h, void *key )
{
    void **cur, **buffer = h->buffer;
    int buffer_size = h->buffer_size;
int i=0;
    void *key_old;

    #if DEBUG__SAFE
    if ( !key )
    {
        ERROR( "hash_table__add: null key" );
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


void *
hash_table__lookup( Hash_Table *h, const void *key )
{
int i=0;
    void **cur, **buffer = h->buffer;
    int buffer_size = h->buffer_size;

    #if DEBUG__SAFE
    if ( !key )
    {
        ERROR( "hash_table__lookup: null key" );
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


void *
hash_table__remove(Hash_Table *h, const void *key)
{
    void **cur, **buffer = h->buffer;
    int buffer_size = h->buffer_size;
    void *displaced = 0;

    #if DEBUG__SAFE
    if ( !key )
    {
        ERROR( "hash_table__remove: null key" );
        return 0;
    }
    #endif

    cur = buffer + ( h->hash( key ) % buffer_size );

    while ( *cur )
    {
        if ( !h->compare( *cur, key ) )
        {
            displaced = *cur;
            *cur = 0;
            h->size--;
            break;
        }

        /* Increment and wrap. */
        cur++;
        cur = buffer + ( ( unsigned int ) ( cur - buffer ) % buffer_size );
    }

    return displaced;
}


void
hash_table__walk( Hash_Table *h, Dist_f f )
{
    void **cur, **lim;

    #if DEBUG__SAFE
    if ( !h || !f )
    {
        ERROR( "hash_table__walk: null argument" );
        return;
    }
    #endif

    #if DEBUG__HASH_TABLE
    printf( "[] hash_table__walk(%#x, %#x)\n", ( int ) h, ( int ) f );
    #endif

    cur = h->buffer;
    lim = h->buffer + h->buffer_size;

    while ( cur < lim )
    {
        if ( *cur && f( cur ) )
            break;

        cur++;
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
