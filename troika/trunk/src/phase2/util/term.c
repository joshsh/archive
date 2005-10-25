/*+
  term.c

  An empty P2_term has no data; it is represented by the NULL.

  Note: this program assumes that sizeof(void *) == sizeof(unsigned int).

*//*/////////////////////////////////////////////////////////////////////////////

Phase2 version 0.4, Copyright (C) 2005 Joshua Shinavier.

This program is free software; you can redistribute it and/or modify it under
the P2_terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Joshua Shinavier
parcour@gmail.com
+1 509 747-6051

*///////////////////////////////////////////////////////////////////////////////

#include "term.h"

#include <stdlib.h>  // malloc
#include <string.h>  // memcpy



/////////////////////////////////////////////////////////////////////////////////

// Global array expansion factor, with a default value.
float P2_term__expansion_factor = TERM__DEFAULT_EXPANSION;

// Copy the term to a larger buffer.  Hidden.
P2_term *P2_term__expand(P2_term *term, unsigned int minimum_buffer_size)
{
    void **new_buffer, **new_head;
    unsigned int size, new_buffer_size;

    if (term)
    {
        // Ordinarily, the new buffer size will be the old buffer size times the
        // P2_term expansion factor.
        new_buffer_size = (unsigned int)
            (term->buffer_size * P2_term__expansion_factor);

        // If the new buffer size is not large enough, use the given minimum
        // buffer size instead.
        if (new_buffer_size < minimum_buffer_size)
            new_buffer_size = minimum_buffer_size;

        // Copy array data to the new buffer.
        size = (unsigned int) *(term->head);
        new_buffer = (void **) malloc(new_buffer_size * sizeof(void *));
        new_head = new_buffer + new_buffer_size - size;
        memcpy(new_head, term->head, size * sizeof(void *));  
        free(term->buffer);

        // Update the P2_term's metadata.
        term->buffer = new_buffer;
        term->buffer_size = new_buffer_size;
        term->head = new_head;
    }

    return term;
}



/** Constructors and destructor *///////////////////////////////////////////////



/**
 * Create a new P2_term containing a single atom.
 */
P2_term *P2_term__new(void *p, unsigned int initial_buffer_size)
{
    P2_term *term = (P2_term *) malloc(sizeof(P2_term));

    // Buffer starts out at this size, but may expand later.
    term->buffer_size = initial_buffer_size;
    if (term->buffer_size < 2)
        term->buffer_size = 2;

    // Create the buffer.
    term->buffer = (void **) malloc(term->buffer_size * sizeof(void *));

    // Add the atom.
    term->head = term->buffer + term->buffer_size - 1;
    *(term->head) = p;

    // Set the head of the P2_term and store its size there.
    term->head--;
    *(term->head) = (void *) 2;

    return term;
}



/**
 * Copy constructor.
 */
P2_term *P2_term__copy(P2_term *source)
{
    unsigned int size;
    P2_term *term;

    if (!source)
        term = 0;
    else
    {
        size = (unsigned int) *(source->head);

        term = (P2_term *) malloc(sizeof(P2_term));

        // Buffer starts out at this size, but may expand later.
        term->buffer_size = source->buffer_size;

        // Create the buffer.
        term->buffer = (void **) malloc(term->buffer_size * sizeof(void *));

        // Set the head of the P2_term and store its size there.
        term->head = term->buffer + term->buffer_size - size;    
        *(term->head) = (void *) size;

        // Copy data from source buffer to new P2_term's buffer.
        memcpy(term->head, source->head, size * sizeof(void *));
    }

    return term;
}



void P2_term__delete(P2_term *term)
{
    if (term)
    {
        free(term->buffer);
        free(term);
    }
}



/** Accessors */////////////////////////////////////////////////////////////////



/**
 * Logical length of the P2_term, i.e. the number of sub-P2_terms it contains.  Not to
 * be confused with the number of cells required to represent the P2_term (that's in
 * P2_term->head) or its physical size in memory (*(P2_term->head) * sizeof(void *)).
 * Caution: O(n) time overhead to count n sub-P2_terms.
 */
unsigned int P2_term__length(P2_term *term)
{
    unsigned int length = 0;
    void **cur;
    void **sup;

    if (!term)
        length = 0;
    else
    {
        cur = term->head;
        if (*cur == (void *) 2)
            length = 1;
        else
        {
            cur++;
            sup = term->buffer + term->buffer_size;
            while (cur < sup)
            {
                length++;
                cur += (unsigned int) *cur;
            }
        }
    }

    return length;
}



void P2_term__set_expansion_factor(float expansion_factor)
{
    // Override the default array expansion factor.
    if (expansion_factor > 1)
        P2_term__expansion_factor = expansion_factor;
}



/** Normalizing functions *//////////////////////////////////////////////////////



/** Merge functions *////////////////////////////////////////////////////////////



/**
 * ((A1 ... Am) (B1 ... Bn))    simple merge
 */
P2_term *P2_term__merge(P2_term *t1, P2_term *t2)
{
    // Find the size of each P2_term, as well as of the resulting P2_term.
    unsigned int t1_size = (unsigned int) *(t1->head),
                 t2_size = (unsigned int) *(t2->head);
    unsigned int newsize = t1_size + t2_size + 1;

    // P2_term t2 will be receiving t1's data.  Expand its buffer if necessary.
    if (t2->buffer_size < newsize)
        t2 = P2_term__expand(t2, newsize);

    // Prepend t1 to t2.  Note: t2->head may have changed.
    t2->head -= t1_size;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));

    // Add a new P2_term head.
    t2->head--;
    *(t2->head) = (void *) newsize;

    return t2;
}



/**
 * (A1 ... Am (B1 ... Bn))      left-associative merge
 */
P2_term *P2_term__merge_la(P2_term *t1, P2_term *t2)
{
    // Find the size of each P2_term, as well as of the resulting P2_term.
    unsigned int t1_size = (unsigned int) *(t1->head),
                 t2_size = (unsigned int) *(t2->head);
    unsigned int newsize = t1_size + t2_size + 1;
    if (t1_size == 2)
        newsize++;

    // P2_term t2 will be receiving t1's data.  Expand its buffer if necessary.
    if (t2->buffer_size < newsize)
        t2 = P2_term__expand(t2, newsize);

    // Prepend t1 to t2.  Note: t2->head may have changed.
    t2->head -= t1_size;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));

    // Add a new P2_term head.
    if (t1_size == 2)
        t2->head--;
    *(t2->head) = (void *) newsize;

    return t2;
}



/**
 * ((A1 ... Am) B1 ... Bn)      right-associative merge
 */
P2_term *P2_term__merge_ra(P2_term *t1, P2_term *t2)
{
    // Find the size of each P2_term, as well as of the resulting P2_term.
    unsigned int t1_size = (unsigned int) *(t1->head),
                 t2_size = (unsigned int) *(t2->head);
    unsigned int newsize = t1_size + t2_size + 1;
    if (t2_size == 2)
        newsize++;

    // P2_term t2 will be receiving t1's data.  Expand its buffer if necessary.
    if (t2->buffer_size < newsize)
        t2 = P2_term__expand(t2, newsize);

    // Prepend t1 to t2.  Note: t2->head may have changed.
    t2->head -= t1_size;
    if (t2_size > 2)
        t2->head++;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));

    // Add a new P2_term head.
    t2->head--;
    *(t2->head) = (void *) newsize;

    return t2;
}



/**
 * (A1 ... Am B1 ... Bn)        concatenation
 */
P2_term *P2_term__cat(P2_term *t1, P2_term *t2)
{
    // Find the size of each P2_term, as well as of the resulting P2_term.
    unsigned int t1_size = (unsigned int) *(t1->head),
                 t2_size = (unsigned int) *(t2->head);
    unsigned int newsize = t1_size + t2_size + 1;
    if (t1_size != 2)
        newsize--;
    if (t2_size != 2)
        newsize--;

    // P2_term t2 will be receiving t1's data.  Expand its buffer if necessary.
    if (t2->buffer_size < newsize)
        t2 = P2_term__expand(t2, newsize);

    // Prepend t1 to t2.  Note: t2->head may have changed.
    t2->head -= t1_size;
    if (t2_size > 2)
        t2->head++;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));

    // Add a new P2_term head.
    if (t1_size == 2)
        t2->head--;
    *(t2->head) = (void *) newsize;

    return t2;
}



/** Logical set functions and atom substitution *////////////////////////////////



// A (hidden) global variable to avoid repetitive passing of the same function pointer to
// the subterm functions.
void *(*P2_term__f)(void *);



// Hidden.
void *P2_term__for_all_(void **head)
{
    void **sup;

    // If the sub-term represents a leaf node, apply the criterion function.
    // Return 'true' if the result is non-zero.
    if ((unsigned int) *(head) == 2)
    {
        head++;
        return (void *) (P2_term__f(*head) != (void *) 0);
    }

    // If the sub-term contains further sub-terms, iterate through them.
    // Return 'true' if and only if every sub-term returns 'true'.
    else
    {
        sup = head + (unsigned int) *head;
        head++;
        while (head < sup)
        {
            if (!P2_term__for_all_(head))
                return (void *) 0;
            head += (unsigned int) *head;
        }
        return (void *) 1;
    }
}



// Hidden.
void *P2_term__exists_(void **head)
{
    void **sup;
    void *result;

    // If the sub-term represents a leaf node, apply the criterion function.
    // Return the atom itself if the result is non-zero.
    if ((unsigned int) *(head) == 2)
    {
        head++;
        if (P2_term__f(*head) != (void *) 0)
            return *head;
        else
            return (void *) 0;
    }

    // If the sub-term contains further sub-terms, iterate through them.
    // Return the first atom encountered which meets the criterion.  If none are
    // encountered, return 'false'.
    else
    {
        sup = head + (unsigned int) *head;
        head++;
        while (head < sup)
        {
            result = P2_term__exists_(head);
            if (result)
                return result;
            head += (unsigned int) *head;
        }
        return (void *) 0;
    }
}



// Hidden.
void P2_term__substitute_all_(void **head)
{
    void **sup;

    // If the sub-term represents a leaf node, apply the substitution function.
    if ((unsigned int) *(head) == 2)
    {
        head++;
        *head = P2_term__f(*head);
    }

    // If the sub-term contains further sub-terms, iterate through them.
    else
    {
        sup = head + (unsigned int) *head;
        head++;
        while (head < sup)
        {
            P2_term__substitute_all_(head);
            head += (unsigned int) *head;
        }
    }
}



void *P2_term__for_all(P2_term *term, void *(*criterion)(void *))
{
    if (!term)
        return (void *) 0;
    else
    {
        P2_term__f = criterion;
        return P2_term__for_all_(term->head);
    }
}



// Caution: output may not be meaningful if the P2_term contains NULL atoms.
void *P2_term__exists(P2_term *term, void *(*criterion)(void *))
{
    if (!term)
        return (void *) 0;
    else
    {
        P2_term__f = criterion;
        return P2_term__exists_(term->head);
    }
}



/**
  Note: unlike the equivalent sequence__replace_atoms, there is no return value
  to indicate substitution failure.
*/
P2_term *P2_term__substitute_all(P2_term *term, void *(*substitution)(void *))
{
    if (term)
    {
        P2_term__f = substitution;
        P2_term__substitute_all_(term->head);
    }

    return term;
}



/*- end of file -*/
