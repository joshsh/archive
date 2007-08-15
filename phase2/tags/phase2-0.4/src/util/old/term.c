/*+
  term.h

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
    // Number of void pointers the buffer can store.
    unsigned int buffer_size;
    
    // An array which holds both generic pointers ("items") and unsigned int
    // values indicating the size of each subterm.
    void **buffer;
    
    // The rightmost cell of the term representation in the buffer.  It always
    // contains an unsigned int value representing the size of the term.
    void **head;
    
    // Points to the cell immediately after the last cell in the buffer, for
    // bounds checking.
    void **sup;
};

#define TERM    struct JS_term



// Identifying values of the S and K combinators, with default values.
void *TERM_SK_S = (void *) 1;
void *TERM_SK_K = (void *) 2;

void term__init_sk(void *sk_s, void *sk_k)
{
    TERM_SK_S = sk_s;
    TERM_SK_K = sk_k;
}



/**
 * The physical size of the term in memory (i.e. the number of cells it contains).
 */
unsigned int term__size(TERM *term)
{
    // The head position always contains an unsigned int representing the number
    // of cells in the term.
    return (unsigned int) *(term->head);
}

/**
 * Logical length of the term, i.e. the number of sub-terms it contains.  Not to
 * be confused with term__size (the number of array cells required to represent
 * the term).  Caution: O(n) processing overhead to count the subterms.
 */
int term__length(TERM *term)
{
    ...
}

/**
 * Create a new, empty term.
 */
TERM *term__empty(int initial_buffer_size)
{
    buffer_size = initial_buffer_size;
    
    // Create the buffer.
    buffer = (void **) malloc(initial_buffer_size * sizeof(void *));

    // Set upper bound.
    sup = buffer + initial_buffer_size;

    // Set head position.
    head = buffer;
            
    // Length of the new term is 0.
    *head = (void *) 0; //~
}

/**
 * Create a new term containing a single item.
 */
TERM *term__new(void *p, size_t initial_buffer_size)
{
    // Buffer starts out at this size, but may expand later.
    buffer_size = initial_buffer_size;
    
    // Create the buffer.
    buffer = (void **) malloc(initial_buffer_size * sizeof(void *));
    
    // Set upper bound.
    sup = buffer + initial_buffer_size;
    
    // Set head position, add the item, and increment head position.
    head = buffer;    
    *head = p;
    head++;
    
    // Length of the new term is 1.
    *head = (void *) 1;
}


a1b12 a1b1c13 K1 3

a[1]b[1][4] a[1]b[1]c[1][6] K[1] [14]

{a}[2]{b}[2][4] {a}[2]{b}[2]{c}[2][7] {K}[2] [15]

//Kxy --> x
TERM *term__K_reduce(TERM *term)
{   
    void **x, **y;
    unsigned int x_size, y_size;
    
    // Skip the 'K' to reach the head of the 'x' sub-term.
    x = term->head - 3;
    x_size = (unsigned int) *x;
    
    // Skip the 'x' and the body of the 'y' to reach the leftmost cell in 'y'
    // (target of the copy) and 'x' (source of the copy).
    x -= x_size;
    y = x;
    x++;
    y_size = (unsigned int) *y;
    y -= y_size;
    y++;
        
    // Copy the 'x' subterm to the target position.
    memcpy(y, x, x_size * sizeof(void *));
  
    // Reset the term head and decrement its value by the size of the two
    // sub-terms ('K' and 'y') which were removed.
    x += x_size;
    *x = *(term->head) - (void *) y_size - (void *) 2; 
    term->head = x;
}

{z}[2] {y}[2] {x}[2] {S}[2] [9]
{z}[2]{y}[2][5] {z}[2]{x}[2][5] [11] 

//Sxyz --> xz(yz)
TERM *term__S_reduce(TERM *term)
{   
    void **x, **y, **z;
    unsigned int x_size, y_size, z_size, newsize;
    
    // Locate the head of 'x', 'y' and 'z'.
    x = term->head - 3;
    x_size = *x;
    y = x - x_size;
    y_size = *y;
    z = y - y_size;
    z_size = *z;
    
    // Predict the size of the resulting array.  If necessary, copy the term to a
    // larger buffer and reduce that instead.  The call stack will grow as long
    // as the term grows.
    newsize = (unsigned int) *(term->head) - 2 + z_size;
    if (newsize > term->buffer_size)
        // Note: premature exit.
        return term__S_reduce(term__expand(term));
    
    // Instead of the head of a sub-term, we now want its lowest cell location.
    x = x - x_size + 1;
    y = y - y_size + 1;
    z = z - z_size + 1;
    
    // Copy 'x' to an auxiliary array.
    
    // Copy a duplicate 'z' to the position heretofore occupied by 'x'.
    
    // Copy 'x' back into the original term.
}










TERM term__merge(TERM *t1, TERM *t2)
{
    int s

    memcpy(++t2.head, 
}




      sequence__merge :     ((A1 ... Am) (B1 ... Bn))    simple merge

      sequence__merge_la :  (A1 ... Am (B1 ... Bn))      left-associative merge

      sequence__merge_ra :  ((A1 ... Am) B1 ... Bn)      right-associative merge

      sequence__cat :       (A1 ... Am B1 ... Bn)        concatenation

      
/*- end of file -*/