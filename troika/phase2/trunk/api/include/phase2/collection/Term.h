/**

\file  Term.h

\brief  Provides a class to represent nested sequences.

Terms are represented by arrays of 4-byte cells, each of which contains either
a reference to an "atom" or an integer i indicating the head of a sub-term with
a length of i cells.  For instance, \n
                                                  \n
    "a"  <-->  2 a                                \n
                                                  \n
    "a b"  <-->  5 2 a 2 b                        \n
                                                  \n
    "a (b c)"  <-->  8 2 a 5 2 b 2 c              \n

The term array occupies the higher-address portion of an expanding buffer and
grows towards the lower-address end.

\note  This library assumes that sizeof ( void* ) == sizeof ( unsigned int ).

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

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

#ifndef TERM_H
#define TERM_H


#include <Type.h>


/** A nested sequence of objects.  A term can be thought of as a parenthesized
    expression. */
typedef struct Term Term;


/** Defines a new expansion factor for terms. When the term outgrows its buffer,
    the new buffer will be this much larger. */
extern void
term__set_expansion( Term *t, double expansion );


/** \note Needed by term-reduce.c. */
extern Term *
term__expand( Term *t, unsigned int minimum_buffer_size );


/* Constructors and destructor ************************************************/


/** Creates a new Term containing a single atom.
    \note  The term does not own its atoms, which will suffer no ill effects
    when the term is deleted. */
extern Term *
term__new( void *p, unsigned int initial_buffer_size );

/** Copy constructor. */
extern Term *
term__copy( Term *t );

/** Destructor. */
extern void
term__delete( Term *t );


/* Accessors ******************************************************************/


/** \return  the logical length of the term, i.e. the number of sub-terms it
    contains.  Not to be confused with the number of cells required to represent
    the term (that's in Term->head) or its physical size in memory
    (*(Term->head) * sizeof(void *)).
    \warning  O(n) time overhead to count n sub-terms. */
extern unsigned int
term__length( const Term *t );

/** \return  a new Term representing the subterm at a given index
    \warning  For the sake of efficiency, there is no error checking.  Make sure
    your index is in range by first calling term__length. */
extern Term *
term__subterm_at( Term *t, int i );


/** \return  the leftmost item in the term */
extern void *
term__head(Term *t);


/* Normalizing functions ******************************************************/


/* Merge functions ************************************************************/


/** ((A1 ... Am) (B1 ... Bn))   simple merge */
extern Term *
term__merge( Term *t1, Term *t2 );

/** (A1 ... Am (B1 ... Bn))     left-associative merge */
extern Term *
term__merge_la( Term *t1, Term *t2 );

/** ((A1 ... Am) B1 ... Bn)     right-associative merge */
extern Term *
term__merge_ra( Term *t1, Term *t2 );

/** (A1 ... Am B1 ... Bn)       concatenation */
extern Term *
term__cat( Term *t1, Term *t2 );


/* Distributor ****************************************************************/


/** Applies a visitor function to each object in the term. */
extern void
term__walk( Term *t, Visitor f );


/******************************************************************************/


/** Serializes the term to a string as a parenthesized expression. */
extern void
term__encode( Term *t, char *buffer );

/** \return  an instance of the "term" data type */
extern Type *
term__create_type( const char *name, int flags );


#endif  /* TERM_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
