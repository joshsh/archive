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

#include "p2_array.h"

#include <stdlib.h>  /* malloc */
#include <string.h>  /* memcpy */


/* By default, expand() doubles the size of the array. */
#define DEFAULT_EXPANSION_FACTOR    2.0

static void expand(p2_array *a);


/* Constructors and destructor ************************************************/


p2_array *p2_array__new(int buffer_size, float expansion)
{
    p2_array *a = (p2_array *) malloc(sizeof(p2_array));

    if (a)
    {
        /* Expansion factor must be greater than 1 for the buffer to actually
           gain in size. */
        if (expansion <= 1)
            a->expansion = DEFAULT_EXPANSION_FACTOR;
        else
            a->expansion = expansion;

        /* Buffer size must be positive. */
        if (buffer_size < 1)
            a->buffer_size = 1;
        else
            a->buffer_size = buffer_size;

        a->head = 0;
        a->size = 0;

        if (!(a->buffer = (void **) malloc(a->buffer_size * sizeof(void *))))
            a = 0;
    }

    return a;
}


p2_array *p2_array__copy(p2_array *a)
{
    int size;

    p2_array *b = (p2_array *) malloc(sizeof(p2_array));
    if (b)
    {
        b->head = a->head;
        b->size = a->size;
        b->expansion = a->expansion;
        b->buffer_size = a->buffer_size;
        size = b->buffer_size * sizeof(void *);
        if (!(b->buffer = malloc(size)))
            b = 0;
        else
            memcpy(b->buffer, a->buffer, size);
    }

    return b;
}


void p2_array__delete(p2_array *a)
{
    free(a->buffer);
    free(a);
}


/* Array resizing *************************************************************/


static void expand(p2_array *a)
{
    void **buffer0;
    int i, size0 = (int) (a->expansion * a->buffer_size);

    /* If the the array's own exansion factor is too close to 1 to resize the
       buffer, use DEFAULT_EXPANSION_FACTOR instead. */
    if (size0 <= a->buffer_size)
        size0 = DEFAULT_EXPANSION_FACTOR * a->buffer_size;

    buffer0 = (void **) malloc(size0 * sizeof(void *));
    for (i = 0; i < a->size; i++)
        buffer0[i] = a->buffer[(a->head + i) % a->buffer_size];
    free(a->buffer);
    a->buffer = buffer0;
    a->head = 0;
    a->buffer_size = size0;
}


/* Random access **************************************************************/


void *p2_array__get(p2_array *a, int index)
{
    if ((index >= 0) && (index < a->size))
        return a->buffer[(index + a->head) % a->buffer_size];
    else
        return 0;
}


void p2_array__set(p2_array *a, int index, void *p)
{
    if ((index >=0) && (index < a->size))
        a->buffer[(index + a->head) % a->buffer_size] = p;
}


/* Stack and queue operations *************************************************/

/*
   push    : pre-decrement head (pre-resize if is_full)
   enqueue : post-increment tail_index (pre-resize if is_full)
   pop     : post-increment head (if array is non-empty)
   "dequeue" : pre-decrement tail_index (if array is non-empty)
*/


void *p2_array__peek(p2_array *a)
{
    if (a->size)
        return a->buffer[a->head];
    else
        return 0;
}


void *p2_array__push(p2_array *a, void *p)
{
    if (a->size >= a->buffer_size)
        expand(a);  /* Note: replaces a->buffer */
    a->head = ((a->head - 1) + a->buffer_size) % a->buffer_size;
    a->buffer[a->head] = p;
    a->size++;

    return p;
}


void *p2_array__pop(p2_array *a)
{
    void *p;

    if (a->size)
    {
        p = a->buffer[a->head];
        a->head = (a->head + 1) % a->buffer_size;
        a->size--;
        return p;
    }
    else
        return 0;
}


void *p2_array__enqueue(p2_array *a, void *p)
{
    if (a->size >= a->buffer_size)
        expand(a);  /* Note: replaces a->buffer */
    a->buffer[(a->head + a->size) % a->buffer_size] = p;
    a->size++;

    return p;
}


void *p2_array__dequeue(p2_array *a)
{
    if (a->size)
    {
        a->size--;
        return a->buffer[(a->head + a->size) % a->buffer_size];
    }
    else
        return 0;
}


/* Random insertion and removal ***********************************************/


void p2_array__insert_before(p2_array *a, int index, void *p)
{
    int i;

    if ((index >= 0) && (index < a->size))
    {
        if (a->size >= a->buffer_size)
            expand(a);  /* Note: replaces a->buffer */

        for (i = a->size; i > index; i--)
            a->buffer[(a->head + i) % a->buffer_size]
                = a->buffer[(a->head + i - 1) % a->buffer_size];

        a->buffer[(a->head + index) % a->buffer_size] = p;
        a->size++;
    }
}


void p2_array__insert_after(p2_array *a, int index, void *p)
{
    int i;

    if ((index >= 0)&&(index < a->size))
    {
        if (a->size >= a->buffer_size)
            expand(a);  /* Note: replaces a->buffer */

        for (i = a->size; i > (index + 1); i--)
            a->buffer[(a->head + i) % a->buffer_size]
                = a->buffer[(a->head + i - 1) % a->buffer_size];

        a->buffer[(a->head + index + 1) % a->buffer_size] = p;
        a->size++;
    }
}


void p2_array__remove(p2_array *a, int index)
{
    int i;

    if ((index >= 0)&&(index < a->size))
    {
        for (i=index; i<(a->size-1); i++)
            a->buffer[(a->head+i)%a->buffer_size]
                = a->buffer[(a->head+i+1)%a->buffer_size];

        a->size--;
    }
}


void p2_array__simple_remove(p2_array *a, int index)
{
    if ((index >= 0) && (index < a->size))
    {
        a->buffer[(a->head + index) % a->buffer_size]
            = a->buffer[(a->head + a->size - 1) % a->buffer_size];
        a->size--;
    }
}


/* Array sorting **************************************************************/


/* Some (hidden) global variables to save on argument passing */
void **buffer_, **aux_;
int (*compare_) (void *, void *);


/* adapted from a MergeSort example by H.W. Lang */
static void merge(int lo, int m, int hi)
{
    int i, j, k;

    i = 0; j = lo;
    /* Copy first half of target array to auxiliary array. */
    while (j <= m)
        aux_[i++] = buffer_[j++];

    i = 0; k = lo;

    /* Copy back next-greatest element at each time. */
    while (k < j && j <= hi)
    {
        if (compare_(aux_[i],buffer_[j]) <= 0)
            buffer_[k++] = aux_[i++];
        else
            buffer_[k++] = buffer_[j++];
    }

    /* Copy back remaining elements of first half (if any). */
    while (k < j)
        buffer_[k++] = aux_[i++];
}


/* Adapted from a MergeSort example by H.W. Lang */
static void mergesort(int lo, int hi)
{
    int m;

    if (lo < hi)
    {
        m = (lo + hi) / 2;
        mergesort(lo, m);
        mergesort(m + 1, hi);
        merge(lo, m, hi);
    }
}


void p2_array__mergesort(p2_array *a, int (*compare) (void *, void *))
{
    int i;

    int size = a->size, buffer_size = a->buffer_size, head = a->head;
    void **buffer = a->buffer;

    /* Normalize the array a so that the mergesort algorithm doesn't have to
       deal with index wrapping. */
    buffer_ = (void **) malloc(sizeof(void*)*a->size);
    for (i=0; i<size; i++)
        buffer_[i] = buffer[(head+i)%buffer_size];
    a->head = 0;
    a->buffer = buffer_;
    free(buffer);

    /* Create auxiliary array for merge. */
    aux_ = (void **) malloc(sizeof(void*)*a->size);

    compare_ = compare;

    mergesort(0, size - 1);

    /* Destroy the auxiliary array. */
    free(aux_);
}


/* Logical set functions and item substitution ********************************/


void *p2_array__for_all(p2_array *a, void *(*criterion) (void *))
{
    int i;

    for (i = 0; i < a->size; i++)
        if (!criterion(a->buffer[(a->head + i) % a->buffer_size]))
            return 0;

    return (void*) 1;
}


void *p2_array__exists(p2_array *a, void *(*criterion)(void *))
{
    int i;

    for (i = 0; i < a->size; i++)
        if (criterion(a->buffer[(a->head + i) % a->buffer_size]))
            return (void*) 1;

    return 0;
}


p2_array *p2_array__substitute_all(p2_array *a, void *(*substitution)(void *))
{
    int i, index;

    for (i = 0; i < a->size; i++)
    {
        index = (a->head + i) % a->buffer_size;
        a->buffer[index] = substitution(a->buffer[index]);
    }

    return a;
}


/* Miscellaneous **************************************************************/


void p2_array__clear(p2_array *a)
{
    a->size = 0;
    a->head = 0;
}


void p2_array__minimize(p2_array *a)
{
    int i;
    void **buffer0;

    if (a->size < a->buffer_size)
    {
        if (!a->size)
        {
            buffer0 = (void **) malloc(sizeof(void *));
            a->buffer_size = 1;
        }
        else
        {
            buffer0 = (void **) malloc(a->size * sizeof(void *));
            for (i = 0; i < a->size; i++)
                buffer0[i] = a->buffer[(a->head + i) % a->buffer_size];
            a->buffer_size = a->size;
        }

        free(a->buffer);
        a->buffer = buffer0;
        a->head = 0;
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
