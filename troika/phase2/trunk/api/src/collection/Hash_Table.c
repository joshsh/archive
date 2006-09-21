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

#include <phase2/collection/Hash_Table.h>
#include "../settings.h"


/* By default, the hash table will wait until the ratio of total cells to
   occupied cells drops to this ratio before expanding. */
#define DEFAULT_LOAD_FACTOR         0.75

/* By default, expand() approximately doubles the size of the buffer. */
#define DEFAULT_EXPANSION_FACTOR    2.0


typedef char cell;

#define OCCUPIED(c)     *(c)
#define VACATE(c)       *(c) = 0
#define WRAP(h,c)       (h)->buffer + (((c) - (h)->buffer) % ((h)->cell_size * (h)->buffer_size));
#define HASH(h,k)       (h)->buffer + ((h)->cell_size * ((h)->hash(k) % (h)->buffer_size ))


struct Hash_Table
{
    size_t          cell_size;

    unsigned int    size;

    /** The number of occupied cells the buffer can hold before the hash table
        becomes too dense. */
    unsigned int    capacity;

    double          load;

    double          expansion;

    /** The hash table buffer array. */
    cell *          buffer;

    /** The number of cells the buffer array. */
    unsigned int    buffer_size;

    /** A hashing function specific to the table's "key" type. */
    Hash_f          hash;

    /** A comparison function for key values. */
    Comparator      compare;
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
    cell *src, *dest, *buffer, *lim;
    unsigned int buffer_size;

    buffer_size = next_prime(
        ( unsigned int ) ( h->buffer_size * h->expansion ) );

    if ( buffer_size <= h->buffer_size )
        buffer_size = next_prime( h->buffer_size + 2 );

    if ( !( buffer = calloc( buffer_size, h->cell_size ) ) )
    {
        ERROR( "Allocation failure." );
        return 0;
    }

    lim = h->buffer + ( h->cell_size * h->buffer_size );

    /* For each cell in the existing table buffer... */
    for ( src = h->buffer; src < lim; src += h->cell_size )
    {
        /* If the cell is occupied, hash it into the new table. */
        if ( OCCUPIED( src ) )
        {
            dest = buffer + ( h->cell_size * ( h->hash( src ) % buffer_size ) );
            while ( *dest )
            {
                dest += h->cell_size;
                dest = buffer + ( ( unsigned int ) ( dest - buffer ) % ( buffer_size * h->cell_size ) );
            }

            memcpy( dest, src, h->cell_size );
        }
    }

    free( h->buffer );
    h->buffer = buffer;
    h->buffer_size = buffer_size;
    h->capacity = ( unsigned int ) ( buffer_size * h->load );

    return h;
}


/******************************************************************************/


Hash_Table *
hash_table__new(
    unsigned int buffer_size,
    double load,
    double expansion,
    size_t cell_size,
    Hash_f hash,
    Comparator compare )
{
/*
    unsigned int i;
*/

    Hash_Table *h;

    if ( DEBUG__SAFE && ( !hash || !compare ) )
        ABORT;

    h = NEW( Hash_Table );

    if ( h )
    {
        /* This needs to be a reasonable number. */
        h->cell_size = cell_size;

        h->buffer_size = next_prime( buffer_size );

        h->hash = hash;
        h->compare = compare;

        /* Sparsity must be at least 1, otherwise the buffer will not resize
           even when it is completely full. */
        h->load = ( load > 0 && load < 1 ) ? load : DEFAULT_LOAD_FACTOR;

        /* Expansion factor must be greater than 1 for the buffer to actually
           gain in size. */
        h->expansion = ( expansion > 1 ) ? expansion : DEFAULT_EXPANSION_FACTOR;

        /* Buffer is initially empty. */
        h->size = 0;

        /* Capacity is re-calculated whenever the table resizes.
           Note: capacity may initially be 0, which just means that the hash
           table will expand as soon as it is added to. */
        h->capacity = ( unsigned int ) ( h->buffer_size * h->load );

        if ( !( h->buffer = calloc( h->buffer_size, h->cell_size ) ) )
        {
            free( h );
            h = 0;
        }
    }

    if ( !h )
        ERROR( "hash_table__new: allocation failure" );

    return h;
}


Hash_Table *
hash_table__copy( const Hash_Table *h )
{
    Hash_Table *h2;

    if ( DEBUG__SAFE && !h )
        ABORT;

    h2 = NEW( Hash_Table );

    if ( h2 )
    {
        /* Copy everything except for the buffer. */
        *h2 = *h;

        /* Copy the buffer, too. */
        if ( !( h2->buffer = malloc( h->buffer_size * h->cell_size ) ) )
        {
            free( h2 );
            h2 = 0;
        }

        else
            memcpy( h2->buffer, h->buffer, h->cell_size * h->buffer_size );
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


/******************************************************************************/


unsigned int
hash_table__size( const Hash_Table *h )
{
    return h->size;
}


double
hash_table__load( const Hash_Table *h )
{
    return h->load;
}


double
hash_table__expansion( const Hash_Table *h )
{
    return h->expansion;
}


/******************************************************************************/


void
hash_table__add( Hash_Table *h, void *key )
{
    cell *cur;

    if ( DEBUG__SAFE && ( !h || !key ) )
        ABORT;

    cur = HASH( h, key );

    while ( OCCUPIED( cur ) )
    {
        /* No duplicate entries allowed.  Replace with new key. */
        if ( !h->compare( cur, key ) )
        {
            /* Table doesn't grow. */
            h->size--;

            break;
        }

        /* Increment and wrap */
        cur += h->cell_size;
        cur = WRAP( h, cur );
    }

    memcpy( cur, key, h->cell_size );

    h->size++;
    if ( h->size >= h->capacity )
    {
        /* Warning: no checking of return value. */
        expand( h );
    }
}


void *
hash_table__lookup( const Hash_Table *h, const void *key )
{
    cell *cur;

    if ( DEBUG__SAFE && ( !h || !key ) )
        ABORT;

    cur = HASH( h, key );

    while ( OCCUPIED( cur ) )
    {
        if ( !h->compare( cur, key ) )
            return cur;

        /* Increment and wrap. */
        cur += h->cell_size;
        cur = WRAP( h, cur );
    }

    return 0;
}


/* When a cell is removed (replaced with a NULL), adjacent cells on the
   higher-address side may become unreachable, and need to be re-inserted into
   the table. */
static void
rehash_dependent_cells( Hash_Table *h, cell *removed )
{
    cell *cur = removed, *aux;
    unsigned int i = -1, j;

    do
    {
        i++;
        cur += h->cell_size;
        cur = WRAP( h, cur );

    } while ( *cur );

    if ( i )
    {
        aux = malloc( i * h->cell_size );

        cur = removed;
        for ( j = 0; j < i; j++ )
        {
            cur += h->cell_size;
            cur = WRAP( h, cur );
            memcpy( aux + ( h->cell_size * j ), cur, h->cell_size );
            VACATE( cur );
            h->size--;
        }

        for ( j = 0; j < i; j++ )
            hash_table__add( h, aux + ( h->cell_size * j ) );

        free( aux );
    }
}


void
hash_table__remove( Hash_Table *h, const void *key )
{
    cell *cur;

    if ( DEBUG__SAFE && ( !h || !key ) )
        ABORT;

    cur = HASH( h, key );

    while ( OCCUPIED( cur ) )
    {
        if ( !h->compare( cur, key ) )
        {
            VACATE( cur );
            h->size--;

            /* Re-hash dependent cells. */
            rehash_dependent_cells( h, cur );

            break;
        }

        /* Increment and wrap. */
        cur += h->cell_size;
        cur = WRAP( h, cur );
    }
}


void
hash_table__walk( Hash_Table *h, Visitor f )
{
    cell *cur, *lim;
    ACTION r;

    if ( DEBUG__SAFE && ( !h || !f ) )
        ABORT;

    cur = h->buffer;
    lim = h->buffer + ( h->cell_size * h->buffer_size );

    while ( cur < lim )
    {
/* FIXME: ? */
        if ( OCCUPIED( cur ) && ( r = f( ( void** ) cur ) ) )
        {
            if ( r == REMOVE )
            {
                VACATE( cur );
                h->size--;
            }

            else if ( r == BREAK )
                break;
        }

        cur += h->cell_size;
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
