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

#include <stdlib.h>  /* malloc */
#include <string.h>  /* strcmp */


#define SIZEOF_CHAR 8

/* By default, the hash table will wait until it is 1/3 full before expanding.
   Note: the sparsity factor does not need to be an integer. */
#define DEFAULT_SPARSITY_FACTOR 3.0

/* By default, expand() approximately floats the size of the buffer.
   Note: the expansion factor does not need to be an integer. */
#define DEFAULT_EXPANSION_FACTOR 2.0

/* The number of void references contained in a single entry. */
#define ENTRY_SIZE 2


/******************************************************************************/


/* Works well enough for addresses so long as there's a one-to-one correspondence
   between addresses and key values. */
unsigned int hash_address(const void *key)
{
    return (unsigned int) key;
}


int compare_addresses(const void *key1, const void *key2)
{
    return (key1 != key2);
}


/** \note  From the hashpjw example by P. J. Weinberger in Aho + Sethi + Ullman. */
unsigned int hash_string(const char *key)
{
    char const *p;
    unsigned int h = 0, g;

    for ( p = key; *p != '\0'; p++ )
    {
        h = ( h << 4 ) + *p;
        if ( ( g = h & 0xf0000000 ) )
        {
            h = h ^ ( g >> 24 );
            h = h ^ g;
        }
    }

    return h;
/*
    int len = strlen((char *) key);
    if (len < 3)
        return ((unsigned int) *((void **) key)) << (3-len) * SIZEOF_CHAR >> (3-len) * SIZEOF_CHAR;
    else
        return (unsigned int) *((void **) key);
*/
}


int compare_strings(const char *key1, const char *key2)
{
    return strcmp((char *) key1, (char *) key2);
}


/******************************************************************************/


/** \return the least prime > 2 and >= i. */
static unsigned int next_prime(unsigned int i)
{
    int j;

    if (i <= 3)
        return 3;
    else if (!(i % 2))
        i++;

    while (1)  /* Breaks out when next prime is found. */
    {
        for (j = 3; j < i; j += 2)
            if (!(i % j))
                j = i + 1;

        if (j > i)
            i += 2;
        else
            return i;
   }
}


/* ! Multithreading hazard. */
p2_hash_table *rehash_dest;


static void *rehash(void *key, void *target)
{
    p2_hash_table__add(rehash_dest, key, target);
    return (void*) 1;
}


static void rehash_all(p2_hash_table *src, p2_hash_table *dest)
{
    rehash_dest = dest;
    p2_hash_table__for_all(src, (void *(*)(void*, void *)) rehash);
}


static void expand(p2_hash_table *h)
{
    int i, size_old, size0 = (int) (h->buffer_size * h->expansion);
    void **p, **q, **buffer_old;

    if (size0 > h->buffer_size)
    {
        size_old = h->buffer_size;
        buffer_old = h->buffer;
        h->buffer_size = next_prime(size0);
        h->buffer = (void **) malloc(sizeof(void *) * (ENTRY_SIZE * h->buffer_size ));
        h->capacity = (int) (((float) h->buffer_size) / h->sparsity);

        for (i = 0; i < ENTRY_SIZE * h->buffer_size; i++)
            h->buffer[i] = 0;

        for (i = 0; i < size_old; i++)
        {
            p = buffer_old + (i * ENTRY_SIZE);
            if (*p)
            {
                q = p + 1;

                /* Cancel out the incrementation for this re-hashing add(). */
                h->size--;

                p2_hash_table__add(h, *p, *q);
            }
        }

        free(buffer_old);
    }
}


/******************************************************************************/


p2_hash_table *p2_hash_table__new(
    unsigned int buffer_size,
    float sparsity,
    float expansion,
    unsigned int (*hashing_function) (const void *),
    int (*compare_to) (const void *, const void *))
{
    int i;

    p2_hash_table *h = (p2_hash_table *) malloc(sizeof(p2_hash_table));

    if (h)
    {
        h->buffer_size = next_prime(buffer_size);

        if (!hashing_function)
            h->hashing_function = hash_address;
        else
            h->hashing_function = hashing_function;

        if (!compare_to)
            h->compare_to = compare_addresses;
        else
            h->compare_to = compare_to;

        /* Sparsity must be at least 1, otherwise the buffer will not resize
           even when it is completely full. */
        if (sparsity < 1)
            h->sparsity = DEFAULT_SPARSITY_FACTOR;
        else
            h->sparsity = sparsity;

        /* Expansion factor must be greater than 1 for the buffer to actually
           gain in size. */
        if (expansion <= 1)
            h->expansion = DEFAULT_EXPANSION_FACTOR;
        else
            h->expansion = expansion;

        /* Buffer is initially empty. */
        h->size = 0;

        /* Capacity is re-calculated whenever the table resizes. */
        h->capacity = (int) (((float) h->buffer_size) / h->sparsity);

        if (!(h->buffer = (void **) malloc(sizeof(void*)
                          * (ENTRY_SIZE * h->buffer_size))))
            h = 0;
        else
            for (i = 0; i < (h->buffer_size * ENTRY_SIZE); i++)
                h->buffer[i] = 0;
    }

    return h;
}


p2_hash_table *p2_hash_table__copy(p2_hash_table *h)
{
    p2_hash_table *h2 = (p2_hash_table *) malloc(sizeof(p2_hash_table));

    if (h2)
    {
        memcpy(h2, h, sizeof(p2_hash_table));
        if (!(h2->buffer = (void **) malloc(sizeof(void*)
                          * (ENTRY_SIZE * h2->buffer_size))))
            h2 = 0;
        else
            rehash_all(h, h2);
    }

    return h2;
}


void p2_hash_table__delete(p2_hash_table *h)
{
    free(h->buffer);
    free(h);
}


p2_hashing_pair p2_hash_table__add(p2_hash_table *h, void *key, void *target)
{
    p2_hashing_pair displaced_pair;
    unsigned int actual_size;
    void **p;

    #if DEBUG__SAFE
    if (!key || !target)
    {
        DEBUG__SAFE__PRINT("null hashing key and/or target");
        displaced_pair.key = displaced_pair.target = 0;
        return displaced_pair;
    }
    #endif

    p = h->buffer + (ENTRY_SIZE*(h->hashing_function(key) % h->buffer_size));

    actual_size = ENTRY_SIZE * h->buffer_size;

    while (*p != 0)
    {
        /* No duplicate entries allowed.  Replace with new key / target pair. */
        if (!h->compare_to(*p, key))
        {
            /* Table doesn't grow. */
            h->size--;

            break;
        }

        /* Increment and wrap */
        p = h->buffer + ((p + ENTRY_SIZE - h->buffer) % actual_size);
    }

    displaced_pair.key = *p;
    *p = key;
    p++;
    displaced_pair.target = *p;
    *p = target;

    h->size++;
    if (h->size >= h->capacity)
        expand(h);

    return displaced_pair;
}


void *p2_hash_table__lookup(p2_hash_table *h, const void *key)
{
    unsigned int actual_size;
    void **p;

    if (!key)
        return 0;

    p = h->buffer + (ENTRY_SIZE * (h->hashing_function(key) % h->buffer_size));

    actual_size = ENTRY_SIZE * h->buffer_size;

    while (1)
    {
        if ((*p == 0)||(!h->compare_to(*p, key)))
            break;

        /* Increment and wrap. */
        p = h->buffer + ((p + ENTRY_SIZE - h->buffer) % actual_size);
    }

    p++;
    return *p;  /* Note: relies on the target value of empty h->buffer being NULL. */
}


p2_hashing_pair p2_hash_table__remove(p2_hash_table *h, void *key)
{
    p2_hashing_pair displaced_pair;
    unsigned int actual_size;
    void **p;

    #if DEBUG__SAFE
        if (!key)
        {
            PRINTERR( "p2_hash_table__remove: null key" );
            displaced_pair.key = displaced_pair.target = 0;
            return displaced_pair;
        }
    #endif

    p = h->buffer + (ENTRY_SIZE * (h->hashing_function(key) % h->buffer_size));

    actual_size = ENTRY_SIZE * h->buffer_size;

    while (*p != 0)
    {
        if (!h->compare_to(*p, key))
        {
            displaced_pair.key = *p;
            *p = 0;
            p++;
            displaced_pair.target = *p;
            *p = 0;
            h->size--;
            break;
        }

        /* Increment and wrap. */
        p = h->buffer + ((p + ENTRY_SIZE - h->buffer)%actual_size);
    }

    return displaced_pair;
}


void *p2_hash_table__for_all(p2_hash_table *h, void *(*func)(void *, void *))
{
    void **cur = h->buffer;
    void **sup = h->buffer + (ENTRY_SIZE * h->buffer_size);

    while (cur < sup)
    {
        if (*cur && !func(*cur, *(cur + 1)))
            return 0;
        cur += 2;
    }

    return (void*) 1;
}


void *p2_hash_table__for_all_keys(p2_hash_table *h, void *(*func)(void *))
{
    void **cur = h->buffer;
    void **sup = h->buffer + (ENTRY_SIZE * h->buffer_size);

    while (cur < sup)
    {
        if (*cur && !func(*cur))
            return 0;
        cur += 2;
    }

    return (void*) 1;
}


void *p2_hash_table__for_all_targets(p2_hash_table *h, void *(*func)(void *))
{
    void **cur = h->buffer;
    void **sup = h->buffer + (ENTRY_SIZE * h->buffer_size);

    while (cur < sup)
    {
        if (*cur && !func(*(cur + 1)))
            return 0;
        cur += 2;
    }

    return (void*) 1;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
