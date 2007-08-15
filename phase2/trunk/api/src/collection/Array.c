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

#include <phase2/Array.h>
#include <phase2/Object.h>
#include "../settings.h"
    #include "../type/Type-impl.h"


struct Array
{
    /** A relative pointer to the first item in the array.  Stack operations
        cause this to change. */
    unsigned int head;

    /** The number of occupied cells in the array. */
    unsigned int size;

    /** The number of cells the buffer array. */
    unsigned int buffer_size;

    /** The array expands by this factor whenever it outgrows its buffer.
        Memory copying into the new buffer is expensive, so beware of setting
        the expansion factor too low. */
    double expansion;

    /** The buffer array. */
    void **buffer;
};


/* By default, expand() doubles the size of the array. */
#define DEFAULT_EXPANSION_FACTOR    2.0

#define ELMT( a, i )  (a)->buffer[ ( (a)->head + (i) ) % (a)->buffer_size ]
#define INBOUNDS( a, i )  ( (i) < (a)->size )
#define WRAP( a, i )  ( (a)->head + (i) ) % (a)->buffer_size

#define BUFFER_NEW( size )  malloc( (size) * sizeof( void* ) )


static void **
buffer_copy( Array *a )
{
    void **buffer;
    size_t size = a->buffer_size * sizeof( void* );

    if ( ( buffer = malloc( size ) ) )
        memcpy( buffer, a->buffer, size );

    return buffer /*@null@*/;
}


/* Constructors and destructor ************************************************/


Array *
array__new( unsigned int buffer_size, double expansion )
{
    Array *a = NEW( Array );

    if ( a )
    {
        /* Buffer size must be positive. */
        a->buffer_size = ( buffer_size > 0 )
            ? buffer_size : 1;

        if ( !( a->buffer = BUFFER_NEW( a->buffer_size ) ) )
        {
            free( a );
            return 0;
        }

        /* Expansion factor must be greater than 1 for the buffer to actually
        gain in size. */
        a->expansion = ( expansion > 1 )
            ? expansion : DEFAULT_EXPANSION_FACTOR;

        a->head = a->size = 0;
    }

    return a;
}


Array *
array__copy( Array *a )
{
    Array *b;

    if ( DEBUG__SAFE && !a )
        abort();

    if ( !( b = NEW( Array ) ) )
        return 0;

    *b = *a;

    if ( !( b->buffer = buffer_copy( a ) ) )
    {
        free( b );
        return 0;
    }

    return b;
}


void
array__free( Array *a )
{
    if ( DEBUG__SAFE && !a )
        abort();

    free( a->buffer );
    free( a );
}


/******************************************************************************/


unsigned int
array__size( const Array *a )
{
    if ( DEBUG__SAFE && !a )
        abort();

    return a->size;
}


double
array__expansion( Array *a )
{
    if ( DEBUG__SAFE && !a )
        abort();

    return a->expansion;
}


/* Array resizing *************************************************************/


static Array *
sizeup( Array *a )
{
    void **buffer_new;
    unsigned int i, buffer_size_new;

    if ( DEBUG__SAFE && !a )
        abort();

    if ( a->size < a->buffer_size )
        return a;

    buffer_size_new = ( unsigned int ) ( a->expansion * a->buffer_size );

    /* If the the array's own exansion factor is too close to 1 to resize the
       buffer, make room for just one more cell. */
    if ( buffer_size_new <= a->buffer_size )
        buffer_size_new++;

    if ( !( buffer_new = BUFFER_NEW( buffer_size_new ) ) )
    {
        ERROR( "allocation failure" );
        return 0;
    }

    for ( i = 0; i < a->size; i++ )
        buffer_new[i] = ELMT( a, i );

    free( a->buffer );

    a->buffer = buffer_new;
    a->head = 0;
    a->buffer_size = buffer_size_new;

    return a;
}


/* Random access **************************************************************/


void *
array__get( Array *a, unsigned int i )
{
    if ( DEBUG__SAFE && !a )
        abort();

    if ( INBOUNDS( a, i ) )
        return ELMT( a, i );

    else
    {
        ERROR( "array__get: array index out of bounds" );
        return 0;
    }
}


void *
array__set( Array *a, unsigned int i, void *p )
{
    void **addr, *displaced;

    if ( DEBUG__SAFE && !a )
        abort();

    if ( INBOUNDS( a, i ) )
    {
        addr = &ELMT( a, i );
        displaced = *addr;
        *addr = p;
        return displaced;
    }

    else
    {
        ERROR( "array__set: array index out of bounds" );
        return 0;
    }
}


/* Stack and queue operations *************************************************/

/*
   push    : pre-decrement head (pre-resize if is_full)
   enqueue : post-increment tail_index (pre-resize if is_full)
   pop     : post-increment head (if array is non-empty)
   "dequeue" : pre-decrement tail_index (if array is non-empty)
*/


void *
array__peek( Array *a )
{
    if ( DEBUG__SAFE && !a )
        abort();

    if ( a->size )
        return a->buffer[a->head];

    else
    {
        ERROR( "array__peek: empty array" );
        return 0;
    }
}


void *
array__push( Array *a, void *p )
{
    if ( DEBUG__SAFE && !a )
        abort();

    if ( !sizeup( a ) )
        return 0;

    a->head = WRAP( a, a->buffer_size - 1 );
    a->buffer[a->head] = p;
    a->size++;

    return p;
}


void *
array__pop( Array *a )
{
    void *p;

    if ( DEBUG__SAFE && !a )
        abort();

    if ( a->size )
    {
        p = a->buffer[a->head];
        a->head = WRAP( a, 1 );
        a->size--;
        return p;
    }

    else
    {
        ERROR( "array__pop: can't remove from empty array" );
        return 0;
    }
}


void *
array__enqueue( Array *a, void *p )
{
    if ( DEBUG__SAFE && !a )
        abort();

    if ( !sizeup( a ) )
        return 0;

    ELMT( a, a->size ) = p;
    a->size++;

    return p;
}


void *
array__dequeue( Array *a )
{
    if ( DEBUG__SAFE && !a )
        abort();

    if ( a->size )
    {
        a->size--;
        return ELMT( a, a->size );
    }

    else
    {
        ERROR( "array__dequeue: can't remove from empty array" );
        return 0;
    }
}


/* Random insertion and removal ***********************************************/


void *
array__insert_before( Array *a, unsigned int i, void *p )
{
    unsigned int j;

    if ( DEBUG__SAFE && !a )
        abort();

    if ( INBOUNDS( a, i ) )
    {
        if ( !sizeup( a ) )
            return 0;

        for ( j = a->size; j > i; j-- )
            ELMT( a, j ) = ELMT( a, j - 1 );

        ELMT( a, i ) = p;
        a->size++;

        return p;
    }

    else
    {
        ERROR( "array__insert_before: array index out of bounds" );
        return 0;
    }
}


void *
array__insert_after( Array *a, unsigned int i, void *p )
{
    unsigned int j;

    if ( DEBUG__SAFE && !a )
        abort();

    if ( INBOUNDS( a, i ) )
    {
        if ( !sizeup( a ) )
            return 0;

        for ( j = a->size; j > ( i + 1 ); j-- )
            ELMT( a, j ) = ELMT( a, j - 1 );

        ELMT( a, i + 1 ) = p;
        a->size++;

        return p;
    }

    else
    {
        ERROR( "array__insert_before: array index out of bounds" );
        return 0;
    }
}


void *
array__remove( Array *a, unsigned int i )
{
    unsigned int j;
    void *displaced;

    if ( DEBUG__SAFE && !a )
        abort();

    if ( INBOUNDS( a, i ) )
    {
        displaced = ELMT( a, i );

        for ( j = i; j < ( a->size - 1 ); j++ )
            ELMT( a, j ) = ELMT( a, j + 1 );

        a->size--;

        return displaced;
    }

    else
    {
        ERROR( "array__insert_before: array index out of bounds" );
        return 0;
    }
}


void *
array__simple_remove( Array *a, unsigned int i )
{
    void **addr, *displaced;

    if ( DEBUG__SAFE && !a )
        abort();

    if ( INBOUNDS( a, i ) )
    {
        addr = &ELMT( a, i );
        displaced = *addr;
        a->size--;
        *addr = ELMT( a, a->size );

        return displaced;
    }

    else
    {
        ERROR( "array__insert_before: array index out of bounds" );
        return 0;
    }
}


/* Array sorting **************************************************************/


typedef struct Mergesort_Ctx Mergesort_Ctx;

struct Mergesort_Ctx
{
    void **buffer, **aux;
    Comparator compare;
};


/* Adapted from a MergeSort example by H.W. Lang
   Renamed from "mergesort" to avoid conflict with an identically named function
     in FreeBSD's stdlib.h */
static void
merge_sort( unsigned int lo, unsigned int hi, Mergesort_Ctx *state )
{
    unsigned int m;
    unsigned int i, j, k;
    void **buffer, **aux;
    Comparator compare;

    if (lo < hi)
    {

        m = (lo + hi) / 2;
        merge_sort(lo, m, state);
        merge_sort(m + 1, hi, state);

        /* Merge. */
        {
            buffer = state->buffer;
            aux = state->aux;
            compare = state->compare;

            i = 0; j = lo;

            /* Copy first half of target array to auxiliary array. */
            while ( j <= m )
                aux[i++] = buffer[j++];

            i = 0; k = lo;

            /* Copy back next-greatest element at each time. */
            while ( k < j && j <= hi )
            {
                if ( compare( aux[i], buffer[j] ) <= 0 )
                    buffer[k++] = aux[i++];
                else
                    buffer[k++] = buffer[j++];
            }

            /* Copy back remaining elements of first half (if any). */
            while ( k < j )
                buffer[k++] = aux[i++];
        }
    }
}


/* FIXME: You could avoid using malloc by using a simple shift algorithm */
static Array *
normalize( Array *a )
{
    unsigned int i, size = a->size, buffer_size = a->buffer_size, head = a->head;

    void **buffer = a->buffer;
    void **buffer_new = BUFFER_NEW( size );

    if ( !buffer_new )
        return 0;

    for ( i = 0; i < size; i++ )
        buffer_new[i] = buffer[ ( i + head ) % buffer_size ];

    free( a->buffer );
    a->buffer = buffer_new;
    return a;
}


/* NOTE: compare to qsort in stdlib.h */
void
array__sort( Array *a, Comparator compare )
{
    Mergesort_Ctx state;

    if ( DEBUG__SAFE && ( !a || !compare ) )
        abort();

    state.compare = compare;

    /* Don't try to sort a trivial array. */
    if ( a->size > 1 )
    {
        /* Normalize the array a so that the merge_sort algorithm doesn't have to
        deal with index WRAPping. */
        if ( !normalize( a ) || !( state.aux = BUFFER_NEW( a->size ) ) )
        {
            ERROR( "array__sort: allocation failure" );
            return;
        }

        state.buffer = a->buffer;

        merge_sort( 0, a->size - 1, &state );

        /* Destroy the auxiliary array. */
        free( state.aux );
    }
}


/* Logical set functions and item substitution ********************************/


void
array__walk( Array *a, Visitor f )
{
    unsigned int i, lim;

    if ( DEBUG__SAFE && ( !a || !f ) )
        abort();

    lim = a->size;

    for ( i = 0; i < lim; i++ )
    {
        if ( f( &ELMT( a, i ) ) == BREAK )
            break;
    }
}


/* Miscellaneous **************************************************************/


Array *
array__clear( Array *a )
{
    if ( DEBUG__SAFE && !a )
        abort();

    a->size = 0;
    a->head = 0;
    return a;
}


Array *
array__minimize( Array *a )
{
    unsigned int i, buffer_size_new;
    void **buffer_new;

    if ( DEBUG__SAFE && !a )
        abort();

    if ( a->size >= a->buffer_size )
        return 0;

    buffer_size_new = ( a->size )
        ? a->size : 1;

    if ( !( buffer_new = BUFFER_NEW( a->size ) ) )
    {
        ERROR( "array__minimize: allocation failure" );
        return 0;
    }

    for ( i = 0; i < a->size; i++ )
        buffer_new[i] = ELMT( a, i );

    free( a->buffer );
    a->buffer = buffer_new;
    a->buffer_size = buffer_size_new;
    a->head = 0;

    return a;
}


/******************************************************************************/


static void
array__encode( Array *a, char *buffer )
{
    unsigned int i;
    Object *o;

    if ( DEBUG__SAFE && ( !a || !buffer ) )
        abort();

    sprintf( buffer, "{" );
    buffer++;

    if ( a->size )
    {
        for ( i = 0; i < a->size; i++ )
        {
            o = ( Object* ) ELMT( a, i );

            if ( i )
            {
                sprintf( buffer, ", " );
                buffer += 2;
            }

            object__encode( o, buffer );
            buffer += strlen( buffer );
        }
    }

    sprintf( buffer, "}" );
}


Type *
array__create_type( const char *name, int flags )
{
    Type *type;

    if ( DEBUG__SAFE && ( !name ) )
        abort();

    type = type__new( name, flags );

    if ( type )
    {
        type->destroy = ( Destructor ) array__free;
        type->encode = ( Encoder ) array__encode;
        type->size = ( Size_Of ) array__size;
        type->walk = ( Walker ) array__walk;
    }

    return type;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
