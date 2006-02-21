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

#include "p2_set.h"

#include <string.h>  /* memcpy */


/* By default, the hash table will wait until it is 1/3 full before expanding.
   Note: the sparsity factor does not need to be an integer. */
#define DEFAULT_SPARSITY_FACTOR 3

/* By default, expand() approximately doubles the size of the buffer.
   Note: the expansion factor does not need to be an integer. */
#define DEFAULT_EXPANSION_FACTOR 2


/** \return the least prime > 2 and >= i. */
static int next_prime(int i)
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


static void expand(p2_set *h)
{
    int i, size_old, size0 = (int) (h->buffer_size * h->expansion);
    void **p, **buffer_old;

    if (size0 > h->buffer_size)
    {
        size_old = h->buffer_size;
        buffer_old = h->buffer;
        h->buffer_size = next_prime(size0);
        h->buffer = (void **) malloc(sizeof(void *) * h->buffer_size);
        h->capacity = (int) (((double) h->buffer_size) / h->sparsity);

        for (i = 0; i < h->buffer_size; i++)
            h->buffer[i] = NULL;

        for (i = 0; i < size_old; i++)
        {
            p = buffer_old + i;
            if (*p != NULL)
            {
                h->size--; /* Cancel out the incrementation for this re-hashing add() */
                p2_set__add(h, *p);
            }
        }

        free(buffer_old);
    }
}


p2_set *p2_set__new(int buffer_size, float sparsity, float expansion)
{
    int i;

    p2_set *S = (p2_set *) malloc(sizeof(p2_set));

    if (S)
    {
        S->buffer_size = next_prime(buffer_size);

        /* Sparsity must be at least 1, otherwise the buffer will not resize
           even when it is completely full. */
        if (sparsity < 1)
            S->sparsity = DEFAULT_SPARSITY_FACTOR;
        else
            S->sparsity = sparsity;

        /* Expansion factor must be greater than 1 for the buffer to actually
           gain in size. */
        if (expansion <= 1)
            S->expansion = DEFAULT_EXPANSION_FACTOR;
        else
            S->expansion = expansion;

        /* Buffer is initially empty. */
        S->size = 0;

        /* Capacity is re-calculated whenever the buffer resizes. */
        S->capacity = (int) (((float) S->buffer_size) / S->sparsity);

        if (!(S->buffer = (void **) malloc(sizeof(void*) * S->buffer_size)))
            S = 0;
        else
            for (i = 0; i < S->buffer_size; i++)
                S->buffer[i] = 0;
    }

    return S;
}


p2_set *p2_set__copy(p2_set *S)
{
    p2_set *S2 = (p2_set *) malloc(sizeof(p2_set));

    if (S2)
    {
        memcpy(S2, S, sizeof(p2_set));
        if (!(S2->buffer = (void **) malloc(sizeof(void*) * S2->buffer_size)))
            S2 = 0;
        else
            memcpy((void *) S2->buffer, (void *) S->buffer, sizeof(void*) * S2->buffer_size);
    }

    return S2;
}


void p2_set__delete(p2_set *S)
{
  free(S->buffer);
  free(S);
}


void *p2_set__lookup(p2_set *S, void *key)
{
    void **p;

    if (!key)
        return 0;

    /* Apply the hashing function. */
    p = (int) key < 0 ?
        S->buffer + ((- (int) key) % S->buffer_size)
      : S->buffer + ((  (int) key) % S->buffer_size);

    while (*p && *p != key)
    {
        /* Increment and wrap. */
        p = S->buffer + ((p + 1 - S->buffer) % S->buffer_size);
    }

    return *p;
}


p2_set *p2_set__add(p2_set *S, void *key)
{
    void **p;

    if (!key)
        return 0;

    /* Apply the hashing function. */
    p = (int) key < 0 ?
        S->buffer + ((- (int) key) % S->buffer_size)
      : S->buffer + ((  (int) key) % S->buffer_size);

    while (*p)
    {
        if (*p == key)
        {
            S->size--;
            break;  /* No duplicate entries allowed.  Replace with new target value. */
        }
        /* Increment and wrap. */
        p = S->buffer + ((p + 1 - S->buffer) % S->buffer_size);
    }

    *p = key;

    if (++S->size >= S->capacity)
        expand(S);

    return S;
}


p2_set *p2_set__remove(p2_set *S, void *key)
{
    void **p;

    if (!key)
        return 0;

    /* Apply the hashing function. */
    p = (int) key < 0 ?
        S->buffer + ((- (int) key) % S->buffer_size)
      : S->buffer + ((  (int) key) % S->buffer_size);

    while (*p)
    {
        if (*p == key)
        {
            *p = 0;
            S->size--;
            break;
        }

        /* Increment and wrap. */
        p = S->buffer + ((p + 1 - S->buffer) % S->buffer_size);
    }

    return S;
}


p2_set *p2_set__union(p2_set *S, p2_set *T)
{
    void **p = S->buffer;
    void **psup = S->buffer + S->buffer_size;

    do
        if (*p)
            p2_set__add(T, *p);
    while (++p < psup);

    p2_set__delete(S);
    return T;
}


p2_set *p2_set__intersection(p2_set *S, p2_set *T)
{
    void **p = S->buffer;
    void **psup = S->buffer + S->buffer_size;

    do
        if (*p && !p2_set__lookup(T, *p))
        {
           *p = 0;
           S->size--;
        }
    while (++p < psup);

    p2_set__delete(T);
    return S;
}


void *p2_set__for_all(p2_set *S, void *(*f)(void *))
{
    void **p = S->buffer;
    void **psup = S->buffer + S->buffer_size;

    do
       if (*p && !f(*p))
            return 0;
    while (++p < psup);

    return (void *) 1;
}


void *p2_set__exists(p2_set *S, void *(*f)(void *))
{
    void **p = S->buffer;
    void **psup = S->buffer + S->buffer_size;

    do
        if (*p && f(*p))
            return *p;
    while (++p < psup);

    return 0;
}


p2_set *p2_set__subset(p2_set *S, void *(*f)(void *))
{
    void **p = S->buffer;
    void **psup = S->buffer + S->buffer_size;

    do
        if (*p && !f(*p))
        {
            *p = 0;
            S->size--;
        }
    while (++p < psup);

    return S;
}


void *p2_set__free(void *s)
{
    free(s);
    return (void *) 1;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
