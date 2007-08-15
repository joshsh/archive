/*+
  term.h

  An empty term has no data; it is represented by the NULL.
  
  last edited: 7/30/05

*//*/////////////////////////////////////////////////////////////////////////////

Phase2 version 0.4, Copyright (C) 2005 Joshua Shinavier.

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

Joshua Shinavier
parcour@gmail.com
+1 509 747-6051
  
*///////////////////////////////////////////////////////////////////////////////

// Note: this program assumes that sizeof(void *) == sizeof(unsigned int).



struct JS_term
{
    // Number of cells the buffer can hold.
    unsigned int buffer_size;
    
    // An array of 4-byte cells containing both generic pointers ("items") and
    // unsigned int values (each one indicates the size of the sub-term in which
    // it is the first cell).
    void **buffer;
    
    // Position of the first cell of the term representation in the buffer.  It always
    // contains an unsigned int value indicating the overall size of the term.
    void **head;
};

#define TERM    struct JS_term


/////////////////////////////////////////////////////////////////////////////////

// Identifying values of the S and K combinators, with default values.
void *TERM_SK_S = (void *) 1;
void *TERM_SK_K = (void *) 2;

void term__init_sk(void *sk_s, void *sk_k)
{
    TERM_SK_S = sk_s;
    TERM_SK_K = sk_k;
}

/////////////////////////////////////////////////////////////////////////////////

void **auxiliary_array = NULL;
int auxiliary_array_size = 0;
void **get_auxiliary_array(int minimum_size)
{
    if (auxiliary_array_size < minimum_size)
    {
        free(auxiliary_array);
        auxiliary_array = (void **) malloc(minimum_size * sizeof(void *));
        auxiliary_array_size = minimum_size;
    }
    
    return auxiliary_array;
}

void term__end()
{
    if (auxiliary_array)
        free(auxiliary_array);
}

/////////////////////////////////////////////////////////////////////////////////



/**
 * Logical length of the term, i.e. the number of sub-terms it contains.  Not to
 * be confused with the number of cells required to represent the term (that's in
 * term->head) or its physical size in memory (*(term->head) * sizeof(void *)).
 * Caution: O(n) time overhead to count n sub-terms.
 */
unsigned int term__length(TERM *term)
{
    unsigned int length = 0;
    void **cur;
    void **sup;
    
    if (term == NULL)
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



/**
 * Create a new term containing a single item.
 */
TERM *term__new(void *p, unsigned int initial_buffer_size)
{
    TERM *term = (term *) malloc(sizeof(term));
    
    // Buffer starts out at this size, but may expand later.
    term->buffer_size = initial_buffer_size;
    if (term->buffer_size < 2)
        term->buffer_size = 2;
    
    // Create the buffer.
    term->buffer = (void **) malloc(term->buffer_size * sizeof(void *));
    
    // Add the item.
    term->head = term->buffer + term->buffer_size - 1;
    *(term->head) = p;
    
    // Set the head of the term and store its size there.
    term->head--;    
    *(term->head) = (void *) 2;
    
    return term;
}

TERM *term__expand(TERM *term)
{

}



//Kxy --> x
// [term size] [1]{K} [x_size]{x} [y_size]{y} ...
//     --> [term size - y_size - 2] [x_size]{x} ...
TERM *term__K_reduce(TERM *term)
{   
    void **x, **y;
    unsigned int x_size, y_size;
    
    // Skip the 'K' to reach the head of the 'x' sub-term.
    x = term->head + 3;
    x_size = (unsigned int) *x;
    
    // Skip the 'x' and to reach the head of the 'y' sub-term.
    y = x + x_size;
    y_size = (unsigned int) *y;
        
    // Copy the 'x' subterm to the target position.
    term->head = y + (y_size - x_size);
    memcpy(term->head, x, x_size * sizeof(void *));
  
    // Reset the term head.
    term->head--;
    *(term->head) = (void *) (term->buffer + term->buffer_size - term->head);

    return term;
}



//Sxyz --> xz(yz)
// [term size] [1]{S} [x_size]{x} [y_size]{y} [z_size]{z} ...
//     --> [term size + z_size] [x_size]{x} [z_size]{z} [y_size + z_size] [y_size]{y} [z_size]{z} ...
TERM *term__S_reduce(TERM *term)
{   
    void **x, **y, **z;
    unsigned int x_size, y_size, z_size, newsize;
    
    // Locate the head of 'x', 'y' and 'z'.
    x = term->head + 3;
    x_size = *x;
    y = x + x_size;
    y_size = *y;
    z = y + y_size;
    z_size = *z;
    
    // Predict the size of the resulting array.  If necessary, copy the term to a
    // larger buffer and reduce that instead.
    newsize = (unsigned int) *(term->head) - 2 + z_size;
    if (newsize > term->buffer_size)
    {
        term = term__expand(term, newsize);

        // Re-locate the head of 'x', 'y' and 'z'.
        x = term->head + 3;
        x_size = *x;
        y = x + x_size;
        y_size = *y;
        z = y + y_size;
        z_size = *z;
    }
    
    // Copy 'x' to an auxiliary array.
    void **aux = get_auxiliary_array(x_size);
    memcpy(aux, x, x_size * sizeof(void *));
    
    // Prepend a term head for the new sub-term 'yz'.  The data of 'y' and 'z'
    // remains where it is.
    term->head = y - 1;
    *(term->head) = y_size + z_size + 1;
    
    // Prepend a duplicate 'z'.
    term->head -= z_size;
    memcpy(term->head, z, z_size * sizeof(void *));
    
    // Prepend an 'x'.
    term->head -= x_size;
    memcpy(term->head, aux, x_size * sizeof(void *));
    
    // Prepend a term head.
    term->head--;
    *(term->head) = (void *) (term->buffer + term->buffer_size - term->head);
        
    return term;
}


/**
 * ((A1 ... Am) (B1 ... Bn))    simple merge
 */
TERM *term__merge(TERM *t1, TERM *t2)
{
    // Find the size of each term, as well as of the resulting term.
    unsigned int t1_size = *(t1->head), t2_size = *(t2->head);
    int newsize = t1_size + t2_size + 1;
    
    // Term t2 will be receiving t1's data.  Expand its buffer if necessary.
    if (t2->buffer_size < newsize)
        t2 = term__expand(t2, newsize);

    // Prepend t1 to t2.  Note: t2->head may have changed.
    t2->head -= t1_size;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));
    
    // Add a new term head.
    t2->head--;
    *(t2->head) = (void *) newsize;
    
    return t2;
}



/**
 * (A1 ... Am (B1 ... Bn))      left-associative merge
 */
TERM *term__merge_la(TERM *t1, TERM *t2)
{
    // Find the size of each term, as well as of the resulting term.
    unsigned int t1_size = *(t1->head), t2_size = *(t2->head);
    int newsize = t1_size + t2_size;
    
    // Term t2 will be receiving t1's data.  Expand its buffer if necessary.
    if (t2->buffer_size < newsize)
        term__expand(t2, newsize);

    // Prepend t1 to t2.  Note: t2->head may have changed.
    t2->head -= t1_size;
    memcpy(t2->head, t1->head, t1_size * sizeof(void *));
    
    // Add a new term head.
    t2->head--;
    *(t2->head) = (void *) newsize;
    
    return t2;
}


      sequence__merge :     ((A1 ... Am) (B1 ... Bn))    simple merge

      sequence__merge_la :  (A1 ... Am (B1 ... Bn))      left-associative merge

      sequence__merge_ra :  ((A1 ... Am) B1 ... Bn)      right-associative merge

      sequence__cat :       (A1 ... Am B1 ... Bn)        concatenation

      
/*- end of file -*/
