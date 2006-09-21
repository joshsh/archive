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

#include <phase2/Bunch.h>
#include "../settings.h"
    #include "../type/Type-impl.h"


typedef struct Block Block;

/** \brief A container for an array of pointers.
    May be completely or partially full. */
struct Block
{
    /** The number of (void *) cells in the buffer. */
    unsigned int size;

    /** The number of cells which contain meaningful data. */
    unsigned int filled;

    /** A simple pointer array. */
    void **buffer;

};

/** \brief A container for large, unordered bags of references.
    Consists of a Array of "blocks" of a preferred size.
    May contain nulls and/or repeat references. */
struct Bunch
{
    /** An expanding array of memory blocks. */
    Array *blocks;

    /** The intended size of a block. */
    unsigned int block_size;

    /** Saves on array lookups. */
    Block *last_block;
};


/******************************************************************************/


static Block *
block__new( unsigned int size )
{
    Block *bl = NEW( Block );

    if (bl)
    {
        bl->filled = 0;
            bl->size = size;

        /*~ take memory faults into account (actual block size might have to be
            smaller than the intended size). */
        if ( !( bl->buffer = malloc( size * sizeof( void* ) ) ) )
            bl = 0;
    }

    return bl;
}


static Block *
block__copy( Block *bl )
{
    Block *bl2 = NEW( Block );

    if (bl2)
    {
        bl2->size = bl->size;
        bl2->filled = bl->filled;

        if ( !( bl2->buffer = malloc( bl2->size * sizeof ( void* ) ) ) )
            bl2 = 0;
        else
            memcpy( bl2->buffer, bl->buffer, bl->filled * sizeof ( void* ) );
    }

    return bl2;
}


static void
block__delete( Block *bl )
{
    free( bl->buffer );
    free( bl );
}


/******************************************************************************/


Bunch *
bunch__new( unsigned int block_size )
{
    Bunch *b = NEW( Bunch );

    if ( b )
    {
        /* Block size must be at least 1. */
        b->block_size = ( block_size > 0 ) ? block_size : 1;

        b->last_block = 0;

        if ( !( b->blocks = array__new( 42, 2.0 ) ) )
        {
            free( b );
            b = 0;
        }
    }

    return b;
}


Bunch *
bunch__copy( Bunch *b )
{
    int i, size = b->block_size;
    Block *bl;

    Bunch *b2 = NEW( Bunch );
    if ( !b2 )
        return 0;

    b2->block_size = size;
    b2->blocks = array__new
        ( array__size( b->blocks ), array__expansion( b->blocks ) );
    if ( !b2->blocks )
    {
        free( b2 );
        return 0;
    }

    for ( i = 0; i < size; i++ )
    {
        bl = block__copy( array__get( b->blocks, i ) );
        if ( !bl )
        {
            bunch__delete( b2 );
            return 0;
        }

        if ( !array__enqueue( b2->blocks, ( void* ) bl ) )
        {
            bunch__delete( b2 );
            return 0;
        }
    }

    b2->last_block = array__get( b2->blocks, array__size( b2->blocks ) - 1 );

    return b2;
}


void
bunch__delete( Bunch *b )
{
    ACTION helper( Block **blp )
    {
        block__delete( *blp );
        return 0;
    }

    /* Free all blocks. */
    array__walk( b->blocks, ( Visitor ) helper );

    /* Delete the blocks array. */
    array__delete( b->blocks );

    /* Free the bunch structure itself. */
    free( b );
}


unsigned int
bunch__size( const Bunch *b )
{
    return ( array__size( b->blocks ) * b->block_size ) - ( b->block_size - b->last_block->filled );
}


void *
bunch__add( Bunch *b, void *p )
{
    Block *bl;

    if ( DEBUG__SAFE && ( !b || !p ) )
        ABORT;

    /* Get or create tail-end block. */
    if ( !b->last_block || b->last_block->filled == b->last_block->size )
    {
        bl = block__new( b->block_size );

        if ( bl && array__enqueue( b->blocks, ( void* ) bl ) )
            b->last_block = bl;
        else
            return 0;
    }

    /* Add the item to the tail-end block. */
    b->last_block->buffer[b->last_block->filled++] = p;

    return p;
}


void *
bunch__remove( Bunch *b )
{
    Block *bl = b->last_block;
    void *p = bl->buffer[--bl->filled];

    /* Remove the tail-end block if empty. */
    if ( !bl->filled )
    {
        block__delete( array__dequeue( b->blocks ) );

        if ( bl == b->last_block )
        {
            b->last_block = array__get( b->blocks, array__size( b->blocks ) - 1 );
            return b;
        }

        else
            b->last_block = array__get( b->blocks, array__size( b->blocks ) - 1 );
    }

    return p;
}


void
bunch__walk( Bunch *b, Visitor f )
{
    unsigned int i, j, n = array__size( b->blocks );
    Block *bl;

    for ( i = 0; i < n; i++ )
    {
        bl = array__get( b->blocks, i );

        for ( j = 0; j < bl->filled; j++ )
        {
            if ( f( &bl->buffer[j] ) == REMOVE )
            {
                /* Replace the item with the last item in the bunch. */
                bl->buffer[j] = b->last_block->buffer[--( b->last_block->filled )];

                /* Remove the tail-end block if empty. */
                if ( !b->last_block->filled )
                {
                    block__delete( array__dequeue( b->blocks ) );

                    if ( bl == b->last_block )
                    {
                        b->last_block = 0;
                        /*b->last_block = array__get(b->blocks, b->blocks->size - 1); */
                        return;
                    }

                    else
                    {
                        n--;
                        b->last_block = array__get( b->blocks, array__size( b->blocks ) - 1 );
                    }
                }
                j--;
            }
        }
    }
}


/******************************************************************************/


Type *
bunch__create_type( const char *name, int flags )
{
    Type *type = type__new( name, flags );

    if ( type )
    {
        type->destroy = ( Destructor ) bunch__delete;
        type->size = ( Size_Of ) bunch__size;
        type->walk = ( Walker ) bunch__walk;
    }

    return type;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
