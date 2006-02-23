/**

\file  p2_primitive.h

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

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

#ifndef P2_PRIMITIVE_H
#define P2_PRIMITIVE_H


#include "p2_type.h"


/******************************************************************************/

/** A structure containing a C function pointer together with type information
    and a unique name. */
typedef struct _p2_primitive
{
    /** Memory location of the C function stub. */
    void *( *cstub )( void ** );

    /** A unique name (preferably the same as the C function stub referenced by
        the pointer, e.g. "strdup"). */
    char *name;

    /** Number of parameters. */
    int n_params;

    /** An array of length {n_params} containing the data type of each
        parameter. */
    p2_type **parameter_types;

    /** \note  Not yet used. */
    char **parameter_names;

    /** An array of length {n_params} containing at each parameter index a 0
        if the argument at that index may experience a side-effect, or a 1
        otherwise (the primitive is said to be "referentially transparent" with
        respect to that parameter).
        \note  Not yet used. */
    char *transparency;

    /** The primitive's return type. */
    p2_type *return_type;

} p2_primitive;


/******************************************************************************/

/** Constructor.
    \note  The primitive assumes ownership of the arguments. */
p2_primitive *p2_primitive__new
(
    void *( *cstub )( void ** ),
    char *name,
    int n_params,
    p2_type **parameter_types,
    char **parameter_names,
    char *transparency,
    p2_type *return_type
);

/** Destructor. */
void p2_primitive__delete( p2_primitive *prim );


/*p2_object *p2_primitive__apply( p2_primitive *prim, void **args );*/


/******************************************************************************/

#endif  /* P2_PRIMITIVE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
