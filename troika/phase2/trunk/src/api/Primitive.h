/**

\file  Primitive.h

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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H


#include "p2_type.h"


/******************************************************************************/


enum primitive__flags
{
    PRIM__CONSTRUCTOR   = 0x1,
    PRIM__DECODER       = 0x2,
    PRIM__DESTRUCTOR    = 0x4,
    PRIM__ENCODER       = 0x8
};


typedef struct _p2_parameter
{
    /** A helpful name. */
    char *name;

    /** Data type. */
    Type *type;

    /** Whether the parent primitive may have a side-effect on this parameter. */
    int transparent;

} p2_parameter;


typedef struct Primitive Primitive;

/** A structure containing a C function pointer together with type information
    and a unique name. */
struct Primitive
{
    /** The primitive's return type. */
    Type *return_type;

    /** A unique name (preferably the same as the C function stub referenced by
        the pointer, e.g. "strdup"). */
    char *name;

    /** Memory location of the C function stub. */
    void *( *cstub )( void ** );

    /** The number of formal parameters. */
    unsigned int arity;

    /** The formal parameters. */
    p2_parameter *parameters;
};


/******************************************************************************/


/** Destructor. */
extern void primitive__delete( Primitive *prim );

extern void primitive__encode( Primitive *prim, char *buffer );

/*p2_object *primitive__apply( Primitive *prim, void **args );*/


/******************************************************************************/

#endif  /* PRIMITIVE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
