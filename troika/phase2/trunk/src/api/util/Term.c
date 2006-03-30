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

#include <util/Term.h>
#include <Object.h>


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
    void **new_buffer, **new_head;
    unsigned int size, new_buffer_size;

    /* Ordinarily, the new buffer size will be the old buffer size times the
       Term expansion factor. */
    new_buffer_size = (unsigned int) (t->buffer_size * t->expansion);

    /* If the new buffer size is not large enough, use the given minimum
       buffer size instead. */
    if (new_buffer_size < minimum_buffer_size)
        new_buffer_size = minimum_buffer_size;

    /* Copy array data to the new buffer. */
    size = (unsigned int) *(t->head);
    new_buffer = malloc(new_buffer_size * sizeof(void *));
    new_head = new_buffer + new_buffer_size - size;
    memcpy(new_head, t->head, size * sizeof(void *));
    free(t->buffer);

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

    #if DEBUG__SAFE
    if ( !p )
    {
        ERROR( "term__new: null argument" );
        return 0;
    }
    #endif

    t = malloc(sizeof(Term));

    /* Buffer starts out at this size, but may expand later. */
    if (initial_buffer_size < 2)
        initial_buffer_size = 2;
    t->buffer_size = initial_buffer_size;

    /* Create the buffer. */
    t->buffer = malloc(t->buffer_size * sizeof(void *));

    /* Add the atom. */
    t->head = t->buffer + t->buffer_size - 1;
    *(t->head) = p;

    /* Set the head of the Term and store its size there. */
    t->head--;
    *(t->head) = (void *) 2;

    /* Expansion factor starts at the default value. */
    t->expansion = TERM__DEFAULT_EXPANSION;

    #if DEBUG__TERM
    printf( "[%#x] term__new(%#x, %i)\n", ( int ) t, ( int ) p, initial_buffer_size );
    #endif

    return t;
}


Term *
term__copy( Term *source )
{
    unsigned int size;
    Term *t;

    #if DEBUG__SAFE
    if ( !source )
    {
        ERROR( "term__copy: null argument" );
        return 0;
    }
    #endif

    size = ( unsigned int ) *( source->head );

    t = new( Term );

    /* Buffer starts out at this size, but may expand later. */
    t->buffer_size = source->buffer_size;

    /* Create the buffer. */
    t->buffer = malloc( t->buffer_size * sizeof( void* ) );

    /* Set the head of the Term and store its size there. */
    t->head = t->buffer + t->buffer_size - size;
    *( t->head ) = ( void* ) size;

    /* Copy data from source buffer to new Term's buffer. */
    memcpy( t->head, source->head, size * sizeof( void* ) );

    #if DEBUG__TERM
    printf( "[%#x] term__copy(%#x)\n", ( int ) t, ( int ) source );
    #endif

    return t;
}


void
term__delete( Term *t )
{
    #if DEBUG__SAFE
    if ( !t )
    {
        ERROR( "term__delete: null term" );
        return;
    }
    #endif

    #if DEBUG__TERM
    printf( "[] term__delete(%#x)\n", ( int ) t );
    #endif

    free( t->buffer );
    free( t );
}


/* Accessors ******************************************************************/


unsigned int
term__length( const Term *t )
{
    unsigned int length = 0;
    void **cur, **lim;

    cur = t->head;
    if ( *cur == ( void* ) 2 )
        length = 1;
    else
    {
        cur++;
        lim = t->buffer + t->buffer_size;
        while ( cur < lim )
        {
            length++;
            cur += ( unsigned int ) *cur;
        }
    }

    return length;
}


Term *
term__subterm_at(Term *t, int i)
{
    Term *subterm;
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
    subterm = new( Term );
    subterm->buffer_size = length;
    subterm->buffer = malloc(length * sizeof(void *));
    memcpy(subterm->buffer, cur, length * sizeof(void *));
    subterm->head = subterm->buffer;

    return subterm;
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
term__merge(Term *t1, Term *t2)
{
    #if DEBUG__TERM
    Term *t1_0 = t1, *t2_0 = t2;
    #endif

    /* Find the size of each Term, as well as of the resulting Term. */
    unsigned int t1_size = (unsigned int) *(t1->head),
                 t2_size = (unsigned int) *(t2->head);
    unsigned int newsize = t1_size + t2_size + 1;

    /* Term t2 will be receiving t1's data.  Expand its buffer if necessary. */
    if (t2->buffer_size < newsize)
        t2 = term__expand(t2, newsize);

    /* Prepend t1 to t2.  Note: pointer t2->head may have changed. */
    t2->head -= t1_size;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));

    /* Add a new Term head. */
    t2->head = t2->buffer + t2->buffer_size - newsize;
    *(t2->head) = (void *) newsize;

    /* Destroy t1. */
    term__delete( t1 );

    #if DEBUG__TERM
    printf( "[%#x] term__merge(%#x, %#x)\n",
        ( int ) t2, ( int ) t1_0, ( int ) t2_0 );
    #endif

    return t2;
}


Term *
term__merge_la(Term *t1, Term *t2)
{
    #if DEBUG__TERM
    Term *t1_0 = t1, *t2_0 = t2;
    #endif

    /* Find the size of each Term, as well as of the resulting Term. */
    unsigned int t1_size = (unsigned int) *(t1->head),
                 t2_size = (unsigned int) *(t2->head);
    unsigned int newsize = t1_size + t2_size;
    if (t1_size == 2)
        newsize++;

    /* Term t2 will be receiving t1's data.  Expand its buffer if necessary. */
    if (t2->buffer_size < newsize)
        t2 = term__expand(t2, newsize);

    /* Prepend t1 to t2.  Note: pointer t2->head may have changed. */
    t2->head -= t1_size;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));

    /* Add a new Term head. */
    t2->head = t2->buffer + t2->buffer_size - newsize;
    *(t2->head) = (void *) newsize;

    /* Destroy t1. */
    term__delete( t1 );

    #if DEBUG__TERM
    printf( "[%#x] term__merge_la(%#x, %#x)\n",
        ( int ) t2, ( int ) t1_0, ( int ) t2_0 );
    #endif

    return t2;
}


Term *
term__merge_ra(Term *t1, Term *t2)
{
    #if DEBUG__TERM
    Term *t1_0 = t1, *t2_0 = t2;
    #endif

    /* Find the size of each Term, as well as of the resulting Term. */
    unsigned int t1_size = (unsigned int) *(t1->head),
                 t2_size = (unsigned int) *(t2->head);
    unsigned int newsize = t1_size + t2_size;
    if (t2_size == 2)
        newsize++;

    /* Term t2 will be receiving t1's data.  Expand its buffer if necessary. */
    if (t2->buffer_size < newsize)
        t2 = term__expand(t2, newsize);

    /* Prepend t1 to t2.  Note: pointer t2->head may have changed. */
    t2->head -= t1_size;
    if (t2_size > 2)
        t2->head++;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));

    /* Add a new Term head. */
    t2->head = t2->buffer + t2->buffer_size - newsize;
    *(t2->head) = (void *) newsize;

    /* Destroy t1. */
    term__delete( t1 );

    #if DEBUG__TERM
    printf( "[%#x] term__merge_ra(%#x, %#x)\n",
        ( int ) t2, ( int ) t1_0, ( int ) t2_0 );
    #endif

    return t2;
}


Term *
term__cat(Term *t1, Term *t2)
{
    #if DEBUG__TERM
    Term *t1_0 = t1, *t2_0 = t2;
    #endif

    /* Find the size of each Term, as well as of the resulting Term. */
    unsigned int t1_size = (unsigned int) *(t1->head),
                 t2_size = (unsigned int) *(t2->head);

    unsigned int newsize = t1_size + t2_size + 1;
    if (t1_size != 2)
        newsize--;
    if (t2_size != 2)
        newsize--;

    /* Term t2 will be receiving t1's data.  Expand its buffer if necessary. */
    if (t2->buffer_size < newsize)
        t2 = term__expand(t2, newsize);

    /* Prepend t1 to t2.  Note: pointer t2->head may have changed. */
    t2->head -= t1_size;
    if (t2_size > 2)
        t2->head++;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));

    /* Add a new Term head. */
    t2->head = t2->buffer + t2->buffer_size - newsize;
    *(t2->head) = (void *) newsize;

    /* Destroy t1. */
    term__delete( t1 );

    #if DEBUG__TERM
    printf( "[%#x] term__cat(%#x, %#x)\n",
        ( int ) t2, ( int ) t1_0, ( int ) t2_0 );
    #endif

    return t2;
}


/* Logical set functions and atom substitution ********************************/


void
term__walk( Term *t, Dist_f f )
{
    __label__ finish;

    void walk( void **cur )
    {
        void **lim;

        /* If the sub-term represents a leaf node, execute the procedure. */
        if ( ( unsigned int ) *cur == 2 )
        {
            cur++;

            /* Apply the target function.  Break out if necessary. */
            if ( f( cur ) == walker__break )
                goto finish;
        }

        /* If the sub-term contains further sub-terms, recurse through them. */
        else
        {
            lim = cur + ( unsigned int ) *cur;
            cur++;
            while ( cur < lim )
            {
                walk( cur );
                cur += ( unsigned int ) *cur;
            }
        }
    }

    #if DEBUG__SAFE
    if ( !t || !f )
    {
        ERROR( "term__walk: null argument" );
        return;
    }
    #endif

    #if DEBUG__TERM
    printf( "[] term__walk(%#x, %#x)\n", ( int ) t, ( int ) f );
    #endif

    walk( t->head );

finish:

    /* Avoids GCC's "label at end of compound statement" error. */
    return;
}


/******************************************************************************/


void
term__encode( Term *t, char *buffer )
{
    void encode( void **cur, boolean delimit )
    {
        Object *o;
        void **lim;

        /* If the sub-term represents a leaf node, execute the procedure. */
        if ( ( unsigned int ) *cur == 2 )
        {
            cur++;

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
                encode( cur, TRUE );
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
/*
void **cur = t->head, **lim = t->buffer + t->buffer_size;
while ( cur < lim ) {
printf( " %i", ( int ) *cur ); cur++; }
printf( "\n" );
*/
    #if DEBUG__SAFE
    if ( !t || !buffer )
    {
        ERROR( "term__encode: null argument" );
        return;
    }
    #endif

    encode( t->head, FALSE );
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
