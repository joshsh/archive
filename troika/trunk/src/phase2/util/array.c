/*//////////////////////////////////////////////////////////////////////////////

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

*///////////////////////////////////////////////////////////////////////////////

#include "array.h"

#include <stdlib.h>  // malloc
#include <string.h>  // memcpy


// By default, array__expand() floats the size of the array.
#define DEFAULT_EXPANSION_FACTOR    2.0

void array__expand(P2_array *a);



// Constructors and destructor /////////////////////////////////////////////////



P2_array *array__new(int buffer_size, float expansion)
{
    P2_array *a = (P2_array *) malloc(sizeof(P2_array));

    if (expansion <= 1)
        a->expansion = DEFAULT_EXPANSION_FACTOR;
    else
        a->expansion = expansion;

    if (buffer_size < 1)
        a->buffer_size = 1;
    else
        a->buffer_size = buffer_size;

    a->head = 0;
    a->size = 0;
    a->buffer = (void **) malloc(a->buffer_size * sizeof(void *));

    return a;
}



P2_array *array__copy(P2_array *a)
{
    int size;

    P2_array *b = (P2_array *) malloc(sizeof(P2_array));

    b->head = a->head;
    b->size = a->size;
    b->expansion = a->expansion;
    b->buffer_size = a->buffer_size;
    size = b->buffer_size * sizeof(void *);
    b->buffer = malloc(size);

    memcpy(b->buffer, a->buffer, size);

    return b;
}



void array__delete(P2_array *a)
{
    free(a->buffer);
    free(a);
}



// Array resizing //////////////////////////////////////////////////////////////



// Hidden.
void array__expand(P2_array *a)
{
    void **buffer0;
    int i, size0 = (int) (a->expansion * a->buffer_size);

    // If the the array's own exansion factor is too close to 1 to resize the
    // buffer, use DEFAULT_EXPANSION_FACTOR instead.
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



// Random access ///////////////////////////////////////////////////////////////



void *array__get(P2_array *a, int index)
{
    if ((index >= 0) && (index < a->size))
        return a->buffer[(index + a->head) % a->buffer_size];
    else
        return 0;
}



void array__set(P2_array *a, int index, void *p)
{
    if ((index >=0) && (index < a->size))
        a->buffer[(index + a->head) % a->buffer_size] = p;
}



// Stack and queue operations //////////////////////////////////////////////////

/*
   push    : pre-decrement head (pre-resize if is_full)
   enqueue : post-increment tail_index (pre-resize if is_full)
   pop     : post-increment head (if array is non-empty)
   "dequeue" : pre-decrement tail_index (if array is non-empty)
*/



void *array__peek(P2_array *a)
{
    if (a->size)
        return a->buffer[a->head];
    else
        return 0;
}



void array__push(P2_array *a, void *p)
{
    if (a->size >= a->buffer_size)
        array__expand(a);  // Note: replaces a->buffer
    a->head = ((a->head - 1) + a->buffer_size) % a->buffer_size;
    a->buffer[a->head] = p;
    a->size++;
}



void *array__pop(P2_array *a)
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



void array__enqueue(P2_array *a, void *p)
{
    if (a->size >= a->buffer_size)
        array__expand(a);  // Note: replaces a->buffer
    a->buffer[(a->head + a->size) % a->buffer_size] = p;
    a->size++;
}



void *array__dequeue(P2_array *a)
{
    if (a->size)
    {
        a->size--;
        return a->buffer[(a->head + a->size) % a->buffer_size];
    }
    else
        return 0;
}



// Random insertion and removal ////////////////////////////////////////////////



void array__insert_before(P2_array *a, int index, void *p)
{
    int i;

    if ((index >= 0) && (index < a->size))
    {
        if (a->size >= a->buffer_size)
            array__expand(a);  // Note: replaces a->buffer

        for (i = a->size; i > index; i--)
            a->buffer[(a->head + i) % a->buffer_size]
                = a->buffer[(a->head + i - 1) % a->buffer_size];

        a->buffer[(a->head + index) % a->buffer_size] = p;
        a->size++;
    }
}



void array__insert_after(P2_array *a, int index, void *p)
{
    int i;

    if ((index >= 0)&&(index < a->size))
    {
        if (a->size >= a->buffer_size)
            array__expand(a);  // Note: replaces a->buffer

        for (i = a->size; i > (index + 1); i--)
            a->buffer[(a->head + i) % a->buffer_size]
                = a->buffer[(a->head + i - 1) % a->buffer_size];

        a->buffer[(a->head + index + 1) % a->buffer_size] = p;
        a->size++;
    }
}



void array__remove(P2_array *a, int index)
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



void array__simple_remove(P2_array *a, int index)
{
    if ((index >= 0) && (index < a->size))
    {
        a->buffer[(a->head + index) % a->buffer_size]
            = a->buffer[(a->head + a->size - 1) % a->buffer_size];
        a->size--;
    }
}



// Array sorting ///////////////////////////////////////////////////////////////



/* Some (hidden) global variables to save on argument passing */
void **buffer_, **aux_;
int (*compare_) (void *, void *);



/* adapted from a MergeSort example by H.W. Lang */
void array__merge_(int lo, int m, int hi)
{
    int i, j, k;

    i = 0; j = lo;
    // copy first half of target array to auxiliary array
    while (j <= m)
        aux_[i++] = buffer_[j++];

    i = 0; k = lo;

    // copy back next-greatest element at each time
    while (k < j && j <= hi)
    {
        if (compare_(aux_[i],buffer_[j]) <= 0)
            buffer_[k++] = aux_[i++];
        else
            buffer_[k++] = buffer_[j++];
    }

    // copy back remaining elements of first half (if any)
    while (k < j)
        buffer_[k++] = aux_[i++];
}



/* adapted from a MergeSort example by H.W. Lang */
void array__mergesort_(int lo, int hi)
{
    int m;

    if (lo < hi)
    {
        m = (lo + hi) / 2;
        array__mergesort_(lo, m);
        array__mergesort_(m + 1, hi);
        array__merge_(lo, m, hi);
    }
}



void array__mergesort(P2_array *a, int (*compare) (void *, void *))
{
    int i;

    int size = a->size, buffer_size = a->buffer_size, head = a->head;
    void **buffer = a->buffer;

    // Normalize the array a so that the mergesort algorithm doesn't have to
    // deal with index wrapping.
    buffer_ = (void **) malloc(sizeof(void*)*a->size);
    for (i=0; i<size; i++)
        buffer_[i] = buffer[(head+i)%buffer_size];
    a->head = 0;
    a->buffer = buffer_;
    free(buffer);

    // Create auxiliary array for array__merge_
    aux_ = (void **) malloc(sizeof(void*)*a->size);

    compare_ = compare;

    array__mergesort_(0, size - 1);

    // Destroy the auxiliary array.
    free(aux_);
}



// Miscellaneous ///////////////////////////////////////////////////////////////



void array__clear(P2_array *a)
{
    a->size = 0;
    a->head = 0;
}



void array__minimize(P2_array *a)
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



void array__forall(P2_array *a, void (*func) (void *))
{
    int i;

    for (i = 0; i < a->size; i++)
        func(a->buffer[(a->head + i) % a->buffer_size]);
}

