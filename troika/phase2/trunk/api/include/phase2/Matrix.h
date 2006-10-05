/**

\file  Matrix.h

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

#ifndef MATRIX_H
#define MATRIX_H

#include <Vector.h>


/** A sparse binary matrix over   */
typedef struct Matrix Matrix;

typedef struct MatrixDiff MatrixDiff;

struct MatrixDiff
{
    Matrix *addend;
    Matrix *subtrahend;
};


extern Vector *
matrix__row( Matrix *a, index i );

extern Vector *
matrix__column( Matrix *a, index i );

extern boolean
matrix__component( Matrix *a, index i, index j );

extern Matrix *
matrix__add( Matrix *a, Matrix *b );

extern Matrix *
matrix__subtract( Matrix *a, Matrix *b );

extern Matrix *
matrix__matrix_multiply( Matrix *a, Matrix *b );

extern Matrix *
matrix__vector_multiply( Matrix *a, Vector *v );


extern MatrixDiff *
matrix__diff( Matrix *a, Matrix *b );


#endif  /* MATRIX_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
