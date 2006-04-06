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

#include <util/Array.h>
#include <Object.h>


struct Array
{
    /** A relative pointer to the first item in the array.  Stack operations
        cause this to change. */
    int head;

    /** The number of occupied cells in the array. */
    int size;

    /** The number of cells the buffer array. */
    int buffer_size;

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
#define INBOUNDS( a, i )  ( (i) >= 0 ) && ( (i) < (a)->size )
#define WRAP( a, i )  ( (a)->head + (i) ) % (a)->buffer_size

#define BUFFER_NEW( size )  malloc( (size) * sizeof( void* ) )


static void **
buffer_copy( Array *a )
{
    void **buffer;
    int size = a->buffer_size * sizeof( void* );

    if ( ( buffer = malloc( size ) ) )
        memcpy( buffer, a->buffer, size );

    return buffer;
}


/* Constructors and destructor ************************************************/


Array *
array__new( int buffer_size, double expansion )
{
    Array *a;

    if ( !( a = new( Array ) ) )
        return 0;

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

    #if DEBUG__ARRAY
    printf( "[%#x] array__new(%i, %g)\n", ( int ) a, buffer_size, expansion );
    #endif

    return a;
}


Array *
array__copy( Array *a )
{
    Array *b;

    if ( !( b = new( Array ) ) )
        return 0;

    *b = *a;

    if ( !( b->buffer = buffer_copy( a ) ) )
    {
        free( b );
        return 0;
    }

    #if DEBUG__ARRAY
    printf( "[%#x] array__copy(%#x)\n", ( int ) b, ( int ) a );
    #endif

    return b;
}


void
array__delete( Array *a )
{
    #if DEBUG__ARRAY
    printf( "[] array__delete(%#x)\n", ( int ) a );
    #endif

    free( a->buffer );
    free( a );
}


/******************************************************************************/


unsigned int
array__size( const Array *a )
{
    return a->size;
}


double
array__expansion( Array *a )
{
    return a->expansion;
}


/* Array resizing *************************************************************/


static Array *
sizeup( Array *a )
{
    void **BUFFER_NEW;
    int i, buffer_size_new;

    if ( a->size < a->buffer_size )
        return a;

    buffer_size_new = ( int ) ( a->expansion * a->buffer_size );

    /* If the the array's own exansion factor is too close to 1 to resize the
       buffer, make room for just one more cell. */
    if ( buffer_size_new <= a->buffer_size )
        buffer_size_new++;

    if ( !( BUFFER_NEW = BUFFER_NEW( buffer_size_new ) ) )
    {
        ERROR( "array__minimize: allocation failure" );
        return 0;
    }

    for ( i = 0; i < a->size; i++ )
        BUFFER_NEW[i] = ELMT( a, i );

    free( a->buffer );

    a->buffer = BUFFER_NEW;
    a->head = 0;
    a->buffer_size = buffer_size_new;

    return a;
}


/* Random access **************************************************************/


void *
array__get( Array *a, int i )
{
    if ( INBOUNDS( a, i ) )
        return ELMT( a, i );

    else
    {
        ERROR( "array__get: array index out of bounds" );
        return 0;
    }
}


void *
array__set( Array *a, int i, void *p )
{
    void **addr, *displaced;

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
    if ( !sizeup( a ) )
        return 0;

    ELMT( a, a->size ) = p;
    a->size++;

    return p;
}


void *
array__dequeue( Array *a )
{
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
array__insert_before( Array *a, int i, void *p )
{
    int j;

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
array__insert_after( Array *a, int i, void *p )
{
    int j;

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
array__remove( Array *a, int i )
{
    int j;
    void *displaced;

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
array__simple_remove( Array *a, int i )
{
    void **addr, *displaced;

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
    int lo, m, hi;
    void **buffer, **aux;
    Comparator compare;
};


/* Adapted from a MergeSort example by H.W. Lang */
static void
mergesort( int lo, int hi, Mergesort_Ctx *state )
{
    int m;
    int i, j, k;
    void **buffer, **aux;
    Comparator compare;

    if (lo < hi)
    {

        m = (lo + hi) / 2;
        mergesort(lo, m, state);
        mergesort(m + 1, hi, state);

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


static Array *
normalize( Array *a )
{
    int i, size = a->size, buffer_size = a->buffer_size, head = a->head;

    void **buffer = a->buffer;
    void **BUFFER_NEW = BUFFER_NEW( size );

    if ( !BUFFER_NEW )
        return 0;

    for ( i = 0; i < size; i++ )
        BUFFER_NEW[i] = buffer[ ( i + head ) % buffer_size ];

    free( a->buffer );
    a->buffer = BUFFER_NEW;
    return a;
}


/* Note: compare to qsort in stdlib.h */
void
array__sort( Array *a, Comparator compare )
{
    Mergesort_Ctx state;
    state.compare = compare;

    /* Normalize the array a so that the mergesort algorithm doesn't have to
       deal with index WRAPping. */
    if ( !normalize( a ) || !( state.aux = BUFFER_NEW( a->size ) ) )
    {
        ERROR( "array__sort: allocation failure" );
        return;
    }

    state.buffer = a->buffer;

    mergesort( 0, a->size - 1, &state );

    /* Destroy the auxiliary array. */
    free( state.aux );
}


/* Logical set functions and item substitution ********************************/


void
array__walk( Array *a, Dist_f f )
{
    int i, lim;

    #if DEBUG__SAFE
    if ( !a || !f )
    {
        ERROR( "array__walk: null argument" );
        return;
    }
    #endif

    #if DEBUG__ARRAY
    printf( "[] array__walk(%#x, %#x)\n", ( int ) a, ( int ) f );
    #endif

    lim = a->size;

    for ( i = 0; i < lim; i++ )
    {
        if ( f( &ELMT( a, i ) ) )
            break;
    }
}


/* Miscellaneous **************************************************************/


Array *
array__clear( Array *a )
{
    a->size = 0;
    a->head = 0;
    return a;
}


Array *
array__minimize( Array *a )
{
    int i;
    void **BUFFER_NEW;
    int buffer_size_new;

    if ( a->size >= a->buffer_size )
        return 0;

    buffer_size_new = ( a->size )
        ? a->size : 1;

    if ( !( BUFFER_NEW = BUFFER_NEW( a->size ) ) )
    {
        ERROR( "array__minimize: allocation failure" );
        return 0;
    }

    for ( i = 0; i < a->size; i++ )
        BUFFER_NEW[i] = ELMT( a, i );

    free( a->buffer );
    a->buffer = BUFFER_NEW;
    a->buffer_size = buffer_size_new;
    a->head = 0;

    return a;
}


/******************************************************************************/


static void
encode
    ( void **cur, char *buffer, int delimit )
{
    Object *o;
    void **lim;

    /* If the sub-term represents a leaf node, execute the procedure. */
    if ( ( unsigned int ) *cur == 2 )
    {
        cur++;

/*
        if ( delimit )
        {
            sprintf( buffer, " " );
            buffer++;
        }
*/

        o = ( Object* ) *cur;
        o->type->encode( o->value, buffer );
    }

    /* If the sub-term contains further sub-terms, recurse through them. */
    else
    {
        if ( delimit )
        {
            sprintf( buffer,  "( " );
            buffer += 2;
        }

        lim = cur + ( unsigned int ) *cur;
        cur++;
        while ( cur < lim )
        {
            encode( cur, buffer, 1 );
            buffer += strlen( buffer );

            cur += ( unsigned int ) *cur;

            if ( cur < lim )
            {
                sprintf( buffer, " " );
                buffer++;
            }
        }

        if ( delimit )
        {
            sprintf( buffer, " )" );
            buffer += 2;
        }
    }
}


static void
array__encode( Array *a, char *buffer )
{
    int i;
    Object *o;

    #if DEBUG__SAFE
    if ( !a || !buffer )
    {
        ERROR( "array__encode: null argument" );
        return;
    }
    #endif

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

            o->type->encode( o->value, buffer );
            buffer += strlen( buffer );
        }
    }

    sprintf( buffer, "}" );
}


Type *
array__create_type( const char *name, int flags )
{
    Type *type = type__new( name, flags );

    if ( type )
    {
        type->destroy = ( Destructor ) array__delete;
        type->encode = ( Encoder ) array__encode;
        type->size = ( Size_Of ) array__size;
        type->walk = ( Walker ) array__walk;
    }

    return type;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
