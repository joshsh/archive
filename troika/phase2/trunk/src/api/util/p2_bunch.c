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

#include "p2_bunch.h"


typedef struct block block;

/** \brief A container for an array of pointers.
    May be completely or partially full. */
struct block
{
    /** The number of (void *) cells in the buffer. */
    unsigned int size;

    /** The number of cells which contain meaningful data. */
    unsigned int filled;

    /** A simple pointer array. */
    void **buffer;

};

/** \brief A container for large, unordered bags of references.
    Consists of a p2_array of "blocks" of a preferred size.
    May contain nulls and/or repeat references. */
struct p2_bunch
{
    /** An expanding array of memory blocks. */
    p2_array *blocks;

    /** The intended size of a block. */
    unsigned int block_size;

    /** Saves on array lookups. */
    block *last_block;
};


/******************************************************************************/


static block *block__new(unsigned int size)
{
    block *bl = new( block );

    if (bl)
    {
        bl->filled = 0;
            bl->size = size;

        /*~ take memory faults into account (actual block size might have to be
            smaller than the intended size). */
        if (!(bl->buffer = malloc(size * sizeof(void *))))
            bl = 0;
    }

    return bl;
}


static block *block__copy(block *bl)
{
    block *bl2 = new( block );

    if (bl2)
    {
        bl2->size = bl->size;
        bl2->filled = bl->filled;

        if (!(bl2->buffer = malloc(bl2->size * sizeof(void *))))
            bl2 = 0;
        else
            memcpy(bl2->buffer, bl->buffer, bl->filled * sizeof(void *));
    }

    return bl2;
}


static void *block__delete(block *bl)
{
    free(bl->buffer);
    free(bl);

    return ( void* ) 1;
}


/******************************************************************************/


p2_bunch *p2_bunch__new(unsigned int block_size)
{
    p2_bunch *b = new( p2_bunch );

    #if DEBUG__BUNCH
    printf( "[%#x] p2_bunch__new(%i)\n", ( int ) b, block_size );
    #endif

    if (b)
    {
        /* Block size must be at least 1. */
        b->block_size = (block_size > 0) ? block_size : 1;

        b->last_block = 0;

        if (!(b->blocks = p2_array__new(42, 2.0)))
        {
            free(b);
            b = 0;
        }
    }

    return b;
}


p2_bunch *p2_bunch__copy(p2_bunch *b)
{
    int i, size = b->block_size;
    block *bl;

    p2_bunch *b2 = new( p2_bunch );

    #if DEBUG__BUNCH
    printf( "[%#x] p2_bunch__copy(%#x)\n", ( int ) b2, ( int ) b );
    #endif

    b2->block_size = size;
    b2->blocks = p2_array__new
        (p2_array__size(b->blocks), p2_array__expansion(b->blocks));

    for (i = 0; i < size; i++)
    {
        bl = block__copy((block *) p2_array__get(b->blocks, i));
        p2_array__enqueue(b2->blocks, (void *) bl);
    }

    b2->last_block = (block *) p2_array__get(b2->blocks, p2_array__size(b2->blocks) - 1);

    return b2;
}


static p2_action * block__delete__proc( block *block_p, void *ignored )
{
    block__delete( block_p );
    return 0;
}


void p2_bunch__delete( p2_bunch *b )
{
    p2_procedure p;

    #if DEBUG__BUNCH
    printf( "[] p2_bunch__delete(%#x)\n", ( int ) b );
    #endif

    p.execute = ( procedure ) block__delete__proc;

    /* Free all blocks. */
    p2_array__distribute( b->blocks, &p );

    /* Delete the blocks array. */
    p2_array__delete( b->blocks );

    /* Free the bunch structure itself. */
    free( b );
}


unsigned int p2_bunch__size(p2_bunch *b)
{
    return (p2_array__size(b->blocks) * b->block_size) - (b->block_size - b->last_block->filled);
}


void *p2_bunch__add(p2_bunch *b, void *p)
{
    /* Get or create tail-end block. */
    if (!b->last_block || b->last_block->filled == b->last_block->size)
    {
        b->last_block = block__new(b->block_size);
        p2_array__enqueue(b->blocks, (void *) b->last_block);
    }

    /* Add the item to the tail-end block. */
    b->last_block->buffer[b->last_block->filled++] = p;

    return p;
}


void p2_bunch__add_all(p2_bunch *dest, p2_bunch *src)
{
    unsigned int i, size;
    block *last_block = dest->last_block;

    /* Remove the tail-end block if it is only partially filled. */
    if (last_block)
    {
        if (last_block->filled < last_block->size)
            last_block = (block *) p2_array__dequeue(dest->blocks);
        else
            last_block = 0;
    }

    /* Add all source blocks. */
    size = p2_array__size(src->blocks);
    for (i = 0; i < size; i++)
        p2_array__enqueue(dest->blocks,
            (void *) block__copy((block *) p2_array__get(src->blocks, i)));

    /* Find the new tail-end block. */
    dest->last_block = (block *) p2_array__get(dest->blocks, p2_array__size(dest->blocks) - 1);

    /* Add all items from the previous tail-end block (if any). */
    for ( i = 0; i < last_block->filled; i++ )
        p2_bunch__add( dest, last_block->buffer[i] );
    free(last_block);
}


void *p2_bunch__remove(p2_bunch *b)
{
    block *bl = b->last_block;
    void *p = bl->buffer[--bl->filled];

    /* Remove the tail-end block if empty. */
    if (!bl->filled)
    {
        block__delete((block *) p2_array__dequeue(b->blocks));

        if (bl == b->last_block)
        {
            b->last_block = (block *) p2_array__get(b->blocks, p2_array__size(b->blocks) - 1);
            return b;
        }

        else
            b->last_block = (block *) p2_array__get(b->blocks, p2_array__size(b->blocks) - 1);
    }

    return p;
}


void p2_bunch__distribute( p2_bunch *b, p2_procedure *p )
{
    unsigned int i, j, numblocks = p2_array__size(b->blocks);
    block *bl;
    p2_action *action;

    #if DEBUG__BUNCH
    printf( "[] p2_bunch__distribute(%#x, %#x)\n", ( int ) b, ( int ) p );
    #endif

    for ( i = 0; i < numblocks; i++ )
    {
        bl = ( block* ) p2_array__get( b->blocks, i );

        for ( j = 0; j < bl->filled; j++ )
        {
            if ( ( action =  p2_procedure__execute( p, bl->buffer[j] ) ) )
            {
                switch ( action->type )
                {
                    case p2_action__type__break:

                        return;

                    case p2_action__type__remove:

                        /* Replace the item with the last item in the bunch. */
                        bl->buffer[j] = b->last_block->buffer[--( b->last_block->filled )];

                        /* Remove the tail-end block if empty. */
                        if ( !b->last_block->filled )
                        {
                            block__delete( ( block* ) p2_array__dequeue( b->blocks ) );

                            if ( bl == b->last_block )
                            {
                                b->last_block = 0;
                                /*b->last_block = (block *) p2_array__get(b->blocks, b->blocks->size - 1); */
                                return;
                            }

                            else
                            {
                                numblocks--;
                                b->last_block = ( block* ) p2_array__get( b->blocks, p2_array__size(b->blocks) - 1 );
                            }
                        }
                        j--;

                        break;

                    case p2_action__type__replace:

                        bl->buffer[j] = action->value;
                        break;

                    default:
                        ;
                }
            }
        }
    }
}


/******************************************************************************/


p2_type *p2_bunch__type( const char *name, int flags )
{
    p2_type *type = p2_type__new( name, flags );

    if ( type )
    {
        type->destroy = ( destructor ) p2_bunch__delete;
        type->distribute = ( distributor ) p2_bunch__distribute;
    }

    return type;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
