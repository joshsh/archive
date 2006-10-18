/**

\file  ModelVector.h

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

#ifndef MODELVECTOR_H
#define MODELVECTOR_H

#include <phase2/common.h>


#ifdef index
#   undef index
#endif

typedef union index index;

union index
{
    unsigned int intval;
    void *pointerval;
};


/*
    (dadfriends & !momfriends)
=   dadfriends - momfriends
*/


typedef struct ModelVector ModelVector;

typedef struct ModelVectorDiff ModelVectorDiff;

struct ModelVectorDiff
{
    ModelVector *addend;
    ModelVector *subtrahend;
};


extern ModelVector *
modelvector__new();

extern ModelVector *
modelvector__copy( ModelVector *a );

extern void
modelvector__free( ModelVector *a );


extern boolean
modelvector__get_component( ModelVector *a, index i );

extern ModelVector *
modelvector__set_component( ModelVector *a, index i, boolean b );


/** \return  the Manhattan norm of the vector */
/* Note for negated vectors, you could use a negative value to indicate
   "infinity minus n" */
extern unsigned int
modelvector__manhattan_norm( ModelVector *a );


extern ModelVector *
modelvector__add( ModelVector *a, ModelVector *b );

extern ModelVector *
modelvector__subtract( ModelVector *a, ModelVector *b );

extern ModelVector *
modelvector__multiply( ModelVector *a, ModelVector *b );


extern ModelVectorDiff *
modelvector__diff( ModelVector *a, ModelVector *b );


/* FIXME: rename */
typedef ACTION          ( *VisitorNew )( index i );

/* FIXME: rename */
extern void
modelvector__walk( ModelVector *a, VisitorNew v );

extern boolean
modelvector__equals( ModelVector *a, ModelVector *b );


#endif  /* MODELVECTOR_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
