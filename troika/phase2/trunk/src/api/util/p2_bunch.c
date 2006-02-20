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

#include <stdlib.h>  /* malloc */
#include <string.h>  /* memcpy */


/******************************************************************************/


static block *block__new(unsigned int size)
{
    block *bl = (block *) malloc(sizeof(block));

    if (bl)
    {
        bl->filled = 0;
            bl->size = size;

        /*~ take memory faults into account (actual block size might have to be
            smaller than the intended size). */
        if (!(bl->buffer = (void **) malloc(size * sizeof(void *))))
            bl = 0;
    }

    return bl;
}


static block *block__copy(block *bl)
{
    block *bl2 = (block *) malloc(sizeof(block));

    if (bl2)
    {
        bl2->size = bl->size;
        bl2->filled = bl->filled;

        if (!(bl2->buffer = (void **) malloc(bl2->size * sizeof(void *))))
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
    p2_bunch *b = (p2_bunch *) malloc(sizeof(p2_bunch));

    if (b)
    {
        /* Block size must be at least 1. */
        b->block_size = (block_size > 0) ? block_size : 1;

        b->last_block = 0;

        if (!(b->blocks = p2_array__new(42, 2.0)))
            b = 0;
    }

    return b;
}


p2_bunch *p2_bunch__copy(p2_bunch *b)
{
    int i, size = b->block_size;
    block *bl;

    p2_bunch *b2 = (p2_bunch *) malloc(sizeof(p2_bunch));

    b2->block_size = size;
    b2->blocks = p2_array__new(b->blocks->size, b->blocks->expansion);

    for (i = 0; i < size; i++)
    {
        bl = block__copy((block *) p2_array__get(b->blocks, i));
        p2_array__enqueue(b2->blocks, (void *) bl);
    }

    b2->last_block = (block *) p2_array__get(b2->blocks, b2->blocks->size - 1);

    return b2;
}


void p2_bunch__delete(p2_bunch *b)
{
    /* Free all blocks. */
    p2_array__for_all(b->blocks, (void *(*) (void *)) block__delete);

    /* Delete the blocks array. */
    p2_array__delete(b->blocks);

    /* Free the bunch structure itself. */
    free(b);
}


unsigned int p2_bunch__size(p2_bunch *b)
{
    return (b->blocks->size * b->block_size) - (b->block_size - b->last_block->filled);
}


void p2_bunch__add(p2_bunch *b, void *p)
{
    /* Get or create tail-end block. */
    if (!b->last_block || b->last_block->filled == b->last_block->size)
    {
        b->last_block = block__new(b->block_size);
        p2_array__enqueue(b->blocks, (void *) b->last_block);
    }

    /* Add the item to the tail-end block. */
    b->last_block->buffer[b->last_block->filled++] = p;
}


void p2_bunch__add_all(p2_bunch *dest, p2_bunch *src)
{
    int i, size;
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
    size = src->blocks->size;
    for (i = 0; i < size; i++)
        p2_array__enqueue(dest->blocks,
            (void *) block__copy((block *) p2_array__get(src->blocks, i)));

    /* Find the new tail-end block. */
    dest->last_block = (block *) p2_array__get(dest->blocks, dest->blocks->size - 1);

    /* Add all items from the previous tail-end block (if any). */
    for (i = 0; i < last_block->filled; i++)
        p2_bunch__add(dest, last_block->buffer[i]);
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
            b->last_block = (block *) p2_array__get(b->blocks, b->blocks->size - 1);
            return b;
        }

        else
            b->last_block = (block *) p2_array__get(b->blocks, b->blocks->size - 1);
    }
}


void *p2_bunch__for_all(p2_bunch *b, void *(*criterion) (void *))
{
    int i, j, numblocks = b->blocks->size;
    block *bl;

    for (i = 0; i < numblocks; i++)
    {
        bl = (block *) p2_array__get(b->blocks, i);
        for (j = 0; j < bl->filled; j++)
            if (!criterion(bl->buffer[j]))
                return 0;
    }

    return (void *) 1;
}


void *p2_bunch__exists(p2_bunch *b, void *(*criterion) (void *))
{
    int i, j, numblocks = b->blocks->size;
    block *bl;
    void *p;

    for (i = 0; i < numblocks; i++)
    {
        bl = (block *) p2_array__get(b->blocks, i);
        for (j = 0; j < bl->filled; j++)
        {
            p = bl->buffer[j];
            if (criterion(p))
                return p;
        }
    }

    return 0;
}


p2_bunch *p2_bunch__exclude_if(p2_bunch *b, void *(*criterion) (void *))
{
    int i, j, numblocks = b->blocks->size;
    block *bl;
    void *p;

    for (i = 0; i < numblocks; i++)
    {
        bl = (block *) p2_array__get(b->blocks, i);
        for (j = 0; j < bl->filled; j++)
        {
            p = bl->buffer[j];
            while (criterion(p))
            {
                /* Replace the item with the last item in the bunch. */
                p = bl->buffer[j] = b->last_block->buffer[--(b->last_block->filled)];

                /* Remove the tail-end block if empty. */
                if (!b->last_block->filled)
                {
                    block__delete((block *) p2_array__dequeue(b->blocks));

                    if (bl == b->last_block)
                    {
                        b->last_block = 0;
                        /*b->last_block = (block *) p2_array__get(b->blocks, b->blocks->size - 1); */
                        return b;
                    }

                    else
                    {
                        numblocks--;
                        b->last_block = (block *) p2_array__get(b->blocks, b->blocks->size - 1);
                    }
                }
            }
        }
    }

    return b;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
