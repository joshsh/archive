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

#include "p2_term.h"

#include <stdlib.h>  /* free */
#include <string.h>  /* memcpy */


/*******************************************************************************/


#ifndef TERM__DEFAULT_EXPANSION
/** By default, the size of a term's buffer will be multiplied by this factor
    when the term outgrows it. */
#define TERM__DEFAULT_EXPANSION 2
#endif


/* Copy the term to a larger buffer. */
p2_term *p2_term__expand( p2_term *t, unsigned int minimum_buffer_size )
{
    void **new_buffer, **new_head;
    unsigned int size, new_buffer_size;

    /* Ordinarily, the new buffer size will be the old buffer size times the
       p2_term expansion factor. */
    new_buffer_size = (unsigned int)
        (t->buffer_size * t->expansion);

    /* If the new buffer size is not large enough, use the given minimum
       buffer size instead. */
    if (new_buffer_size < minimum_buffer_size)
        new_buffer_size = minimum_buffer_size;

    /* Copy array data to the new buffer. */
    size = (unsigned int) *(t->head);
    new_buffer = (void **) malloc(new_buffer_size * sizeof(void *));
    new_head = new_buffer + new_buffer_size - size;
    memcpy(new_head, t->head, size * sizeof(void *));
    free(t->buffer);

    /* Update the p2_term's metadata. */
    t->buffer = new_buffer;
    t->buffer_size = new_buffer_size;
    t->head = new_head;

    return t;
}


/* Constructors and destructor ************************************************/


p2_term *p2_term__new(void *p, unsigned int initial_buffer_size)
{
    p2_term *t = (p2_term *) malloc(sizeof(p2_term));

    /* Buffer starts out at this size, but may expand later. */
    if (initial_buffer_size < 2)
        initial_buffer_size = 2;
    t->buffer_size = initial_buffer_size;

    /* Create the buffer. */
    t->buffer = (void **) malloc(t->buffer_size * sizeof(void *));

    /* Add the atom. */
    t->head = t->buffer + t->buffer_size - 1;
    *(t->head) = p;

    /* Set the head of the p2_term and store its size there. */
    t->head--;
    *(t->head) = (void *) 2;

    /* Expansion factor starts at the default value. */
    t->expansion = TERM__DEFAULT_EXPANSION;

    #if DEBUG__TERM
    printf( "[%X] p2_term__new(%X, %i)\n", ( int ) t, ( int ) p, initial_buffer_size );
    #endif

    return t;
}


p2_term *p2_term__copy(p2_term *source)
{
    unsigned int size;
    p2_term *t;

    if (!source)
        t = 0;
    else
    {
        size = (unsigned int) *(source->head);

        t = (p2_term *) malloc(sizeof(p2_term));

        /* Buffer starts out at this size, but may expand later. */
        t->buffer_size = source->buffer_size;

        /* Create the buffer. */
        t->buffer = (void **) malloc(t->buffer_size * sizeof(void *));

        /* Set the head of the p2_term and store its size there. */
        t->head = t->buffer + t->buffer_size - size;
        *(t->head) = (void *) size;

        /* Copy data from source buffer to new p2_term's buffer. */
        memcpy(t->head, source->head, size * sizeof(void *));
    }

    #if DEBUG__TERM
    printf( "[%X] p2_term__copy(%X)\n", ( int ) t, ( int ) source );
    #endif

    return t;
}


void p2_term__delete(p2_term *t)
{
    #if DEBUG__TERM
    printf( "[] p2_term__delete(%X)\n", ( int ) t );
    #endif

    free(t->buffer);
    free(t);
}


/* Accessors ******************************************************************/


unsigned int p2_term__length(p2_term *t)
{
    unsigned int length = 0;
    void **cur, **sup;

    cur = t->head;
    if (*cur == (void *) 2)
        length = 1;
    else
    {
        cur++;
        sup = t->buffer + t->buffer_size;
        while (cur < sup)
        {
            length++;
            cur += (unsigned int) *cur;
        }
    }

    return length;
}


p2_term *p2_term__subterm_at(p2_term *t, int i)
{
    p2_term *subterm;
    void **cur = t->head;
    unsigned int length;

    /* If the term contains a single element, skip to copy (assumes that index
       is equal to 0).  Otherwise cycle through the target index. */
    if (*cur > (void *) 2)
    {
        cur++;

        while (i)
        {
            cur += (unsigned int) *cur;
            i--;
        }
    }

    length = (unsigned int) *cur;
    subterm = (p2_term *) malloc(sizeof(p2_term));
    subterm->buffer_size = length;
    subterm->buffer = (void **) malloc(length * sizeof(void *));
    subterm->head = subterm->buffer;
    memcpy(subterm->head, cur, length * sizeof(void *));

    return subterm;
}


void p2_term__set_expansion( p2_term *t, unsigned int expansion )
{
    /* Override the default array expansion factor. */
    if (expansion > 1)
        t->expansion = expansion;
}


/* Normalizing functions ******************************************************/


/* Merge functions ************************************************************/


p2_term *p2_term__merge(p2_term *t1, p2_term *t2)
{
    /* Find the size of each p2_term, as well as of the resulting p2_term. */
    unsigned int t1_size = (unsigned int) *(t1->head),
                 t2_size = (unsigned int) *(t2->head);
    unsigned int newsize = t1_size + t2_size + 1;

    /* p2_term t2 will be receiving t1's data.  Expand its buffer if necessary. */
    if (t2->buffer_size < newsize)
        t2 = p2_term__expand(t2, newsize);

    /* Prepend t1 to t2.  Note: pointer t2->head may have changed. */
    t2->head -= t1_size;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));

    /* Add a new p2_term head. */
    t2->head = t2->buffer + t2->buffer_size - newsize;
    *(t2->head) = (void *) newsize;

    /* Destroy t1. */
    p2_term__delete( t1 );

    return t2;
}


p2_term *p2_term__merge_la(p2_term *t1, p2_term *t2)
{
    /* Find the size of each p2_term, as well as of the resulting p2_term. */
    unsigned int t1_size = (unsigned int) *(t1->head),
                 t2_size = (unsigned int) *(t2->head);
    unsigned int newsize = t1_size + t2_size;
    if (t1_size == 2)
        newsize++;

    /* p2_term t2 will be receiving t1's data.  Expand its buffer if necessary. */
    if (t2->buffer_size < newsize)
        t2 = p2_term__expand(t2, newsize);

    /* Prepend t1 to t2.  Note: pointer t2->head may have changed. */
    t2->head -= t1_size;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));

    /* Add a new p2_term head. */
    t2->head = t2->buffer + t2->buffer_size - newsize;
    *(t2->head) = (void *) newsize;

    /* Destroy t1. */
    p2_term__delete( t1 );

    return t2;
}


p2_term *p2_term__merge_ra(p2_term *t1, p2_term *t2)
{
    /* Find the size of each p2_term, as well as of the resulting p2_term. */
    unsigned int t1_size = (unsigned int) *(t1->head),
                 t2_size = (unsigned int) *(t2->head);
    unsigned int newsize = t1_size + t2_size;
    if (t2_size == 2)
        newsize++;

    /* p2_term t2 will be receiving t1's data.  Expand its buffer if necessary. */
    if (t2->buffer_size < newsize)
        t2 = p2_term__expand(t2, newsize);

    /* Prepend t1 to t2.  Note: pointer t2->head may have changed. */
    t2->head -= t1_size;
    if (t2_size > 2)
        t2->head++;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));

    /* Add a new p2_term head. */
    t2->head = t2->buffer + t2->buffer_size - newsize;
    *(t2->head) = (void *) newsize;

    /* Destroy t1. */
    p2_term__delete( t1 );

    return t2;
}


p2_term *p2_term__cat(p2_term *t1, p2_term *t2)
{
    /* Find the size of each p2_term, as well as of the resulting p2_term. */
    unsigned int t1_size = (unsigned int) *(t1->head),
                 t2_size = (unsigned int) *(t2->head);

    unsigned int newsize = t1_size + t2_size + 1;
    if (t1_size != 2)
        newsize--;
    if (t2_size != 2)
        newsize--;

    /* p2_term t2 will be receiving t1's data.  Expand its buffer if necessary. */
    if (t2->buffer_size < newsize)
        t2 = p2_term__expand(t2, newsize);

    /* Prepend t1 to t2.  Note: pointer t2->head may have changed. */
    t2->head -= t1_size;
    if (t2_size > 2)
        t2->head++;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));

    /* Add a new p2_term head. */
    t2->head = t2->buffer + t2->buffer_size - newsize;
    *(t2->head) = (void *) newsize;

    /* Destroy t1. */
    p2_term__delete( t1 );

    return t2;
}


/* Logical set functions and atom substitution ********************************/


void p2_term__distribute( p2_term *t, p2_procedure *p )
{
    void **sup, **head = ( void** ) t;
    p2_action *action;

    /* If the sub-term represents a leaf node, execute the procedure. */
    if ( ( unsigned int ) *head == 2 )
    {
        head++;
        if ( ( action = p2_procedure__execute( p, *head ) ) )
        {
            switch ( action->type )
            {
                case p2_action__type__replace:

                    *head = action->value;
                    break;

                default:

                    ;
            }
        }
    }

    /* If the sub-term contains further sub-terms, recurse through them. */
    else
    {
        sup = head + ( unsigned int ) *head;
        head++;
        while ( head < sup )
        {
            p2_term__distribute( ( p2_term* ) head, p );
            head += ( unsigned int ) *head;
        }
    }
}


/******************************************************************************/


p2_type *p2_term__type( const char *name )
{
    p2_type *type = p2_type__new( name, 0 );

    if ( type )
    {
        type->destroy = ( destructor ) p2_term__delete;
        type->distribute = ( distributor ) p2_term__distribute;
    }

    return type;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
