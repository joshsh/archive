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

#include <string.h>  /* memcpy */


/* By default, expand() doubles the size of the array. */
#define DEFAULT_EXPANSION_FACTOR    2.0

#define elmt( a, i )  a->buffer[ ( a->head + i ) % a->buffer_size ]
#define inbounds( a, i )  ( i >= 0 ) && ( i < a->size )
#define wrap( a, i )  ( a->head + i ) % a->buffer_size


#define buffer_new( size )  (void **) malloc( size * sizeof( void* ) )

static void **buffer_copy( p2_array *a )
{
    void **buffer;
    int size = a->buffer_size * sizeof( void* );

    if ( ( buffer = malloc( size ) ) )
        memcpy( buffer, a->buffer, size );

    return buffer;
}


/* Constructors and destructor ************************************************/


p2_array *p2_array__new( int buffer_size, float expansion )
{
    p2_array *a;

    if ( !( a = new( p2_array ) ) )
        return 0;

    /* Buffer size must be positive. */
    a->buffer_size = ( buffer_size > 0 )
        ? buffer_size : 1;

    if ( !( a->buffer = buffer_new( a->buffer_size ) ) )
    {
        free( a );
        return 0;
    }

    /* Expansion factor must be greater than 1 for the buffer to actually
       gain in size. */
    a->expansion = ( expansion > 1 )
        ? expansion : DEFAULT_EXPANSION_FACTOR;

    a->head = a->size = 0;

    return a;
}


p2_array *p2_array__copy( p2_array *a )
{
    p2_array *b;

    if ( !( b = new( p2_array ) ) )
        return 0;

    *b = *a;

    if ( !( b->buffer = buffer_copy( a ) ) )
    {
        free( b );
        return 0;
    }

    return b;
}


void p2_array__delete( p2_array *a )
{
printf( "---array a 1---\n" ); fflush( stdout );
    free( a->buffer );
printf( "---array a 2---\n" ); fflush( stdout );
    free( a );
printf( "---array a 3---\n" ); fflush( stdout );
}


/* Array resizing *************************************************************/


static p2_array *sizeup( p2_array *a )
{
    void **buffer_new;
    int i, buffer_size_new;

    if ( a->size < a->buffer_size )
        return a;

    buffer_size_new = ( int ) ( a->expansion * a->buffer_size );

    /* If the the array's own exansion factor is too close to 1 to resize the
       buffer, use DEFAULT_EXPANSION_FACTOR instead. */
    if ( buffer_size_new <= a->buffer_size )
        buffer_size_new = DEFAULT_EXPANSION_FACTOR * a->buffer_size;

    if ( !( buffer_new = buffer_new( buffer_size_new ) ) )
    {
        PRINTERR( "p2_array__minimize: allocation failure" );
        return 0;
    }

    for ( i = 0; i < a->size; i++ )
        buffer_new[i] = elmt( a, i );

    free( a->buffer );

    a->buffer = buffer_new;
    a->head = 0;
    a->buffer_size = buffer_size_new;

    return a;
}


/* Random access **************************************************************/


void *p2_array__get( p2_array *a, int index )
{
    if ( inbounds( a, index ) )
        return elmt( a, index );

    else
    {
        PRINTERR( "p2_array__get: array index out of bounds" );
        return 0;
    }
}


void *p2_array__set( p2_array *a, int index, void *p )
{
    void **addr, *displaced;

    if ( inbounds( a, index ) )
    {
        addr = &elmt( a, index );
        displaced = *addr;
        *addr = p;
        return displaced;
    }

    else
    {
        PRINTERR( "p2_array__set: array index out of bounds" );
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


void *p2_array__peek( p2_array *a )
{
    if ( a->size )
        return a->buffer[a->head];

    else
    {
        PRINTERR( "p2_array__peek: empty array" );
        return 0;
    }
}


void *p2_array__push( p2_array *a, void *p )
{
    if ( !sizeup( a ) )
        return 0;

    a->head = wrap( a, a->buffer_size - 1 );
    a->buffer[a->head] = p;
    a->size++;

    return p;
}


void *p2_array__pop( p2_array *a )
{
    void *p;

    if ( a->size )
    {
        p = a->buffer[a->head];
        a->head = wrap( a, 1 );
        a->size--;
        return p;
    }

    else
    {
        PRINTERR( "p2_array__pop: can't remove from empty array" );
        return 0;
    }
}


void *p2_array__enqueue( p2_array *a, void *p )
{
    if ( !sizeup( a ) )
        return 0;

    elmt( a, a->size ) = p;
    a->size++;

    return p;
}


void *p2_array__dequeue( p2_array *a )
{
    if ( a->size )
    {
        a->size--;
        return elmt( a, a->size );
    }

    else
    {
        PRINTERR( "p2_array__dequeue: can't remove from empty array" );
        return 0;
    }
}


/* Random insertion and removal ***********************************************/


void *p2_array__insert_before( p2_array *a, int index, void *p )
{
    int i;

    if ( inbounds( a, index ) )
    {
        if ( !sizeup( a ) )
            return 0;

        for ( i = a->size; i > index; i-- )
            elmt( a, i ) = elmt( a, i - 1 );

        elmt( a, index ) = p;
        a->size++;

        return p;
    }

    else
    {
        PRINTERR( "p2_array__insert_before: array index out of bounds" );
        return 0;
    }
}


void *p2_array__insert_after( p2_array *a, int index, void *p )
{
    int i;

    if ( inbounds( a, index ) )
    {
        if ( !sizeup( a ) )
            return 0;

        for ( i = a->size; i > (index + 1); i-- )
            elmt( a, i ) = elmt( a, i - 1 );

        elmt( a, index + 1 ) = p;
        a->size++;

        return p;
    }

    else
    {
        PRINTERR( "p2_array__insert_before: array index out of bounds" );
        return 0;
    }
}


void *p2_array__remove( p2_array *a, int index )
{
    int i;
    void *displaced;

    if ( inbounds( a, index ) )
    {
        displaced = elmt( a, index );

        for ( i = index; i < ( a->size - 1 ); i++ )
            elmt( a, i ) = elmt( a, i + 1 );

        a->size--;

        return displaced;
    }

    else
    {
        PRINTERR( "p2_array__insert_before: array index out of bounds" );
        return 0;
    }
}


void *p2_array__simple_remove( p2_array *a, int index )
{
    void **addr, *displaced;

    if ( inbounds( a, index ) )
    {
        addr = &elmt( a, index );
        displaced = *addr;
        a->size--;
        *addr = elmt( a, a->size );

        return displaced;
    }

    else
    {
        PRINTERR( "p2_array__insert_before: array index out of bounds" );
        return 0;
    }
}


/* Array sorting **************************************************************/


typedef struct _mergesort_state
{
    int lo, m, hi;
    void **buffer, **aux;
    comparator compare;

} mergesort_state;


/* Adapted from a MergeSort example by H.W. Lang */
static void mergesort( int lo, int hi, mergesort_state *state )
{
    int m;
    int i, j, k;
    void **buffer, **aux;
    comparator compare;

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


static p2_array *normalize( p2_array *a )
{
    int i, size = a->size, buffer_size = a->buffer_size, head = a->head;

    void **buffer = a->buffer;
    void **buffer_new = buffer_new( a->size );

    if ( !buffer_new )
        return 0;

    for ( i = 0; i < size; i++ )
        buffer_new[i] = buffer[ ( i + head ) % buffer_size ];

    free( a->buffer );
    a->buffer = buffer_new;
    return a;
}


void p2_array__sort( p2_array *a, comparator compare )
{
    mergesort_state state;
    state.compare = compare;

    /* Normalize the array a so that the mergesort algorithm doesn't have to
       deal with index wrapping. */
    if ( !normalize( a ) || !( state.aux = buffer_new( a->size ) ) )
    {
        PRINTERR( "p2_array__sort: allocation failure" );
        return;
    }

    state.buffer = a->buffer;

    mergesort( 0, a->size - 1, &state );

    /* Destroy the auxiliary array. */
    free( state.aux );
}


/* Logical set functions and item substitution ********************************/


void p2_array__distribute( p2_array *a, p2_procedure *p )
{
    int i;
    p2_action *action;

    for ( i = 0; i < a->size; i++ )
    {
        if ( ( action = p2_procedure__execute( p, elmt( a, i ) ) ) )
        {
            switch ( action->type )
            {
                case p2_action__type__break:

                    return;

                case p2_action__type__replace:

                    elmt( a, i ) = action->value;
                    break;

                default:
                    ;
            }
        }
    }
}


/* Miscellaneous **************************************************************/


p2_array *p2_array__clear( p2_array *a )
{
    a->size = 0;
    a->head = 0;
    return a;
}


p2_array *p2_array__minimize( p2_array *a )
{
    int i;
    void **buffer_new;
    int buffer_size_new;

    if ( a->size >= a->buffer_size )
        return 0;

    buffer_size_new = ( a->size )
        ? a->size : 1;

    if ( !( buffer_new = buffer_new( a->size ) ) )
    {
        PRINTERR( "p2_array__minimize: allocation failure" );
        return 0;
    }

    for ( i = 0; i < a->size; i++ )
        buffer_new[i] = elmt( a, i );

    free( a->buffer );
    a->buffer = buffer_new;
    a->buffer_size = buffer_size_new;
    a->head = 0;

    return a;
}


/******************************************************************************/


p2_type *p2_array__type( const char *name )
{
    p2_type *type = p2_type__new( name, 0 );

    if ( type )
    {
        type->destroy = ( destructor ) p2_array__delete;
        type->distribute = ( distributor ) p2_array__distribute;
    }

    return type;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
