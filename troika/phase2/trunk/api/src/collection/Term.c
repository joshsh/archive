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

#include <phase2/Object.h>
#include "Term-impl.h"
    #include "../type/Type-impl.h"


/*******************************************************************************/


#ifndef TERM__DEFAULT_EXPANSION
/** By default, the size of a term's buffer will be multiplied by this factor
    when the term outgrows it. */
#define TERM__DEFAULT_EXPANSION 2.0
#endif


/* Copy the term to a larger buffer. */
Term *
term__expand( Term *t, unsigned int minimum_buffer_size )
{
    term_cell *new_buffer, *new_head;
    unsigned int size, new_buffer_size;

    /* Ordinarily, the new buffer size will be the old buffer size times the
       Term expansion factor. */
    new_buffer_size = ( unsigned int ) ( t->buffer_size * t->expansion );

    /* If the new buffer size is not large enough, use the given minimum
       buffer size instead. */
    if ( new_buffer_size < minimum_buffer_size )
        new_buffer_size = minimum_buffer_size;

    /* Copy array data to the new buffer. */
    size = t->head->intval;
    new_buffer = malloc( new_buffer_size * sizeof( term_cell ) );
    new_head = new_buffer + new_buffer_size - size;
    memcpy( new_head, t->head, size * sizeof( term_cell ) );
    free( t->buffer );

    /* Update the Term's metadata. */
    t->buffer = new_buffer;
    t->buffer_size = new_buffer_size;
    t->head = new_head;

    return t;
}


/* Constructors and destructor ************************************************/


Term *
term__new( void *p, unsigned int initial_buffer_size )
{
    Term *t;

    /* Terms may contain null atoms. */
/*
    if ( DEBUG__SAFE && !p )
        ABORT;
*/

    t = NEW( Term );

    /* Buffer starts out at this size, but may expand later. */
    if ( initial_buffer_size < 2 )
        initial_buffer_size = 2;
    t->buffer_size = initial_buffer_size;

    /* Create the buffer. */
    t->buffer = malloc( t->buffer_size * sizeof( term_cell ) );

    /* Add the atom. */
    t->head = t->buffer + t->buffer_size - 1;
    t->head->pointerval = p;

    /* Set the head of the Term and store its size there. */
    t->head--;
    t->head->intval = 2;

    /* Expansion factor starts at the default value. */
    t->expansion = TERM__DEFAULT_EXPANSION;

    return t;
}


Term *
term__copy( Term *source )
{
    unsigned int size;
    Term *t;

    if ( DEBUG__SAFE && !source )
        ABORT;

    size = source->head->intval;

    t = NEW( Term );

    /* Buffer starts out at this size, but may expand later. */
    t->buffer_size = source->buffer_size;

    /* Create the buffer. */
    t->buffer = malloc( t->buffer_size * sizeof( term_cell ) );

    /* Set the head of the Term and store its size there. */
    t->head = t->buffer + t->buffer_size - size;
    t->head->intval = size;

    /* Copy data from source buffer to new Term's buffer. */
    memcpy( t->head, source->head, size * sizeof( term_cell ) );

    return t;
}


void
term__delete( Term *t )
{
    if ( DEBUG__SAFE && !t )
        ABORT;

    free( t->buffer );
    free( t );
}


/* Accessors ******************************************************************/


unsigned int
term__length( const Term *t )
{
    unsigned int length = 0;
    term_cell *cur, *lim;

    if ( DEBUG__SAFE && !t )
        ABORT;

    cur = t->head;
    if ( cur->intval == 2 )
        length = 1;
    else
    {
        cur++;
        lim = t->buffer + t->buffer_size;
        while ( cur < lim )
        {
            length++;
            cur += cur->intval;
        }
    }

    return length;
}


Term *
term__subterm_at( Term *t, int i )
{
    Term *subterm;
    term_cell *cur = t->head;
    unsigned int length;

    /* TODO: upper bounds checking */
    if ( DEBUG__SAFE && ( !t || i < 0 ) )
        ABORT;

    /* If the term contains a single element, skip to copy (assumes that index
       is equal to 0).  Otherwise cycle through the target index. */
    if ( cur->intval > 2 )
    {
        cur++;

        while ( i )
        {
            cur += cur->intval;
            i--;
        }
    }

    length = cur->intval;
    subterm = NEW( Term );
    subterm->buffer_size = length;
    subterm->buffer = malloc( length * sizeof( term_cell ) );
    memcpy( subterm->buffer, cur, length * sizeof( term_cell ) );
    subterm->head = subterm->buffer;

    return subterm;
}


void *
term__head( Term *t )
{
    term_cell *cur = t->head;

    if ( DEBUG__SAFE && !t )
        ABORT;

    /* Find the leftmost atom. */
    while ( cur->intval > 2)
        cur++;

    cur++;

    return cur->pointerval;
}


void
term__set_expansion( Term *t, double expansion )
{
    /* Override the default array expansion factor. */
    if (expansion > 1)
        t->expansion = expansion;
}


/* Normalizing functions ******************************************************/


/* Merge functions ************************************************************/


Term *
term__merge( Term *t1, Term *t2 )
{
    /* Find the size of each Term, as well as of the resulting Term. */
    unsigned int t1_size = t1->head->intval,
                 t2_size = t2->head->intval;
    unsigned int newsize = t1_size + t2_size + 1;

    /* Term t2 will be receiving t1's data.  Expand its buffer if necessary. */
    if ( t2->buffer_size < newsize )
        t2 = term__expand( t2, newsize );

    /* Prepend t1 to t2.  Note: pointer t2->head may have changed. */
    t2->head -= t1_size;
    memcpy( t2->head, t1->head, t1_size * sizeof( term_cell ) );

    /* Add a new Term head. */
    t2->head = t2->buffer + t2->buffer_size - newsize;
    t2->head->intval = newsize;

    /* Destroy t1. */
    term__delete( t1 );

    return t2;
}


Term *
term__merge_la(Term *t1, Term *t2)
{
    /* Find the size of each Term, as well as of the resulting Term. */
    unsigned int t1_size = t1->head->intval,
                 t2_size = t2->head->intval;
    unsigned int newsize = t1_size + t2_size;
    if ( t1_size == 2 )
        newsize++;

    /* Term t2 will be receiving t1's data.  Expand its buffer if necessary. */
    if ( t2->buffer_size < newsize )
        t2 = term__expand( t2, newsize );

    /* Prepend t1 to t2.  Note: pointer t2->head may have changed. */
    t2->head -= t1_size;
    memcpy( t2->head, t1->head, t1_size * sizeof( term_cell ) );

    /* Add a new Term head. */
    t2->head = t2->buffer + t2->buffer_size - newsize;
    t2->head->intval = newsize;

    /* Destroy t1. */
    term__delete( t1 );

    return t2;
}


Term *
term__merge_ra( Term *t1, Term *t2 )
{
    /* Find the size of each Term, as well as of the resulting Term. */
    unsigned int t1_size = t1->head->intval,
                 t2_size = t2->head->intval;
    unsigned int newsize = t1_size + t2_size;
    if ( t2_size == 2 )
        newsize++;

    /* Term t2 will be receiving t1's data.  Expand its buffer if necessary. */
    if ( t2->buffer_size < newsize )
        t2 = term__expand( t2, newsize );

    /* Prepend t1 to t2.  Note: pointer t2->head may have changed. */
    t2->head -= t1_size;
    if ( t2_size > 2 )
        t2->head++;
    memcpy( t2->head, t1->head, t1_size * sizeof( term_cell ) );

    /* Add a new Term head. */
    t2->head = t2->buffer + t2->buffer_size - newsize;
    t2->head->intval = newsize;

    /* Destroy t1. */
    term__delete( t1 );

    return t2;
}


Term *
term__cat(Term *t1, Term *t2)
{
    /* Find the size of each Term, as well as of the resulting Term. */
    unsigned int t1_size = t1->head->intval,
                 t2_size = t2->head->intval;

    unsigned int newsize = t1_size + t2_size + 1;
    if ( t1_size != 2 )
        newsize--;
    if ( t2_size != 2 )
        newsize--;

    /* Term t2 will be receiving t1's data.  Expand its buffer if necessary. */
    if ( t2->buffer_size < newsize) 
        t2 = term__expand( t2, newsize );

    /* Prepend t1 to t2.  Note: pointer t2->head may have changed. */
    t2->head -= t1_size;
    if ( t2_size > 2 )
        t2->head++;
    memcpy( t2->head, t1->head, t1_size * sizeof( term_cell ) );

    /* Add a new Term head. */
    t2->head = t2->buffer + t2->buffer_size - newsize;
    t2->head->intval = newsize;

    /* Destroy t1. */
    term__delete( t1 );

    return t2;
}


/* Logical set functions and atom substitution ********************************/


void
term__walk( Term *t, Visitor f )
{
    __label__ finish;

    void walk( term_cell *cur )
    {
        term_cell *lim;

        /* If the sub-term represents a leaf node, execute the procedure. */
        if ( cur->intval == 2 )
        {
            cur++;

            /* Apply the target function.  Break out if necessary. */
            if ( f( &cur->pointerval ) == BREAK )
                goto finish;
        }

        /* If the sub-term contains further sub-terms, recurse through them. */
        else
        {
            lim = cur + cur->intval;
            cur++;
            while ( cur < lim )
            {
                walk( cur );
                cur += cur->intval;
            }
        }
    }

    if ( DEBUG__SAFE && ( !t || !f ) )
        ABORT;

    walk( t->head );

finish:

    /* Avoids GCC's "label at end of compound statement" error. */
    return;
}


/******************************************************************************/


void
term__encode( Term *t, char *buffer )
{
    void encode( term_cell *cur, boolean delimit )
    {
        Object *o;
        term_cell *lim;

        /* If the sub-term represents a leaf node, execute the procedure. */
        if ( cur->intval == 2 )
        {
            cur++;

            o = cur->pointerval;
            object__encode( o, buffer );
        }

        /* If the sub-term contains further sub-terms, recurse through them. */
        else
        {
            if ( delimit )
            {
                sprintf( buffer,  "(" );
                buffer++;
            }

            lim = cur + cur->intval;
            cur++;
            while ( cur < lim )
            {
                encode( cur, TRUE );
                buffer += strlen( buffer );

                cur += cur->intval;

                if ( cur < lim )
                {
                    sprintf( buffer, " " );
                    buffer++;
                }
            }

            if ( delimit )
            {
                sprintf( buffer, ")" );
                buffer++;
            }
        }
    }

    if ( DEBUG__SAFE && ( !t || !buffer ) )
        ABORT;

    sprintf( buffer, "[" );
    buffer++;

    encode( t->head, FALSE );

    sprintf( buffer, "]" );
}


Type *
term__create_type( const char *name, int flags )
{
    Type *type = type__new( name, flags );

    if ( type )
    {
        type->destroy = ( Destructor ) term__delete;
        type->encode = ( Encoder ) term__encode;
        type->size = ( Size_Of ) term__length;
        type->walk = ( Walker ) term__walk;
    }

    return type;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
