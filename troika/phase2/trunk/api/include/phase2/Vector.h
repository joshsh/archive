/**

\file  Vector.h

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

#ifndef VECTOR_H
#define VECTOR_H

#include <common.h>


#ifdef index
#   undef index
#endif

typedef union index index;

union index
{
    unsigned int intval;
    void *pointerval;
};


typedef struct Vector Vector;

typedef struct VectorDiff VectorDiff;

struct VectorDiff
{
    Vector *addend;
    Vector *subtrahend;
};


extern boolean
vector__component( Vector *a, index i );

extern Vector *
vector__add( Vector *a, Vector *b );

extern Vector *
vector__subtract( Vector *a, Vector *b );

extern Vector *
vector__multiply( Vector *a, Vector *b );


extern VectorDiff *
vector__diff( Vector *a, Vector *b );


#endif  /* VECTOR_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
