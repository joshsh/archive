/**

\file  Cube.h

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

#ifndef CUBE_H
#define CUBE_H

#include <Matrix.h>


/** A sparse three-dimensional binary tensor of size N by N by N. */
typedef struct Cube Cube;

typedef struct CubeDiff CubeDiff;

struct CubeDiff
{
    Cube *addend;
    Cube *subtrahend;
};


extern Cube *
cube__create();

extern Matrix *
cube__row_slice( Cube *a, index i );

extern Matrix *
cube__column_slice( Cube *a, index i );

extern Matrix *
cube__tube_slice( Cube *a, index i );

extern Vector *
cube__row_fiber( Cube *a, index i, index j );

extern Vector *
cube__column_fiber( Cube *a, index i, index j );

extern Vector *
cube__tube_fiber( Cube *a, index i, index j );

extern boolean
cube__component( Cube *a, index i, index j, index k );

extern Cube *
cube__add( Cube *a, Cube *b );

extern Cube *
cube__subtract( Cube *a, Cube *b );

extern Cube *
cube__cube_multiply( Cube *a, Cube *b );

extern Cube *
cube__matrix_multiply( Cube *a, Matrix *m );

extern Cube *
cube__vector_multiply( Cube *a, Vector *v );


extern CubeDiff *
cube__diff( Cube *a, Cube *b );


#endif  /* CUBE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
