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

#ifndef TERM_IMPL_H
#define TERM_IMPL_H


#include <collection/Term.h>
#include "../settings.h"


/** \brief A data structure to represent and manipulate nested sequences.
    The implementation is not recursive.

    \note An empty Term has no data; it is represented by the NULL.
 */
struct Term
{
    /** Number of cells the buffer can hold. */
    unsigned int buffer_size;

    /** An array of 4-byte cells containing both generic pointers ("atoms") and
        unsigned int values (each one indicates the size of the sub-term in
        which it is the first cell). */
    term_cell *buffer;

    /** Position of the first cell of the Term representation in the buffer.
        It always contains an unsigned int value indicating the overall size of
        the Term. */
    term_cell *head;

    /** The buffer expands by this factor whenever it becomes full. */
    double expansion;
};


#endif  /* TERM_IMPL_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
