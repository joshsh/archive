/**

\file  Primitive.h

\brief  Defines a class representing function primitives.

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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H


#include <phase2/Type.h>


/******************************************************************************/


/** A first-class function primitive. */
typedef struct Primitive Primitive;

/** Indicates referential opacity, i.e. the condition that a primitive may have
    a side-effect with respect to a particular argument */
#define REF_OPQ 0

/** Indicates referential transparency, i.e. the condition that a primitive does
    not affect a particular argument */
#define REF_TRP 1

/** These flags indicate special functions which are essential methods for a
    given data type. */
enum Primitive__Flags
{
    PRIM__CONSTRUCTOR   = 0x1,
    PRIM__DECODER       = 0x2,
    PRIM__DESTRUCTOR    = 0x4,
    PRIM__ENCODER       = 0x8
};

/******************************************************************************/


/** Destructor. */
extern void
primitive__free( Primitive *prim );

/** \return  the name of the primitive */
extern char *
primitive__name( Primitive *p );

/** \return  the (fixed) number of parameters consumed by the primitive */
extern unsigned int
primitive__arity( Primitive *p );

/** \return  the return type of the primitive */
extern Type *
primitive__return_type( Primitive *p );

/** \return  the name of the given parameter of the primitive */
extern char *
primitive__parameter_name( Primitive *p, unsigned int i );

/** \return  the data type of the given parameter of the primitive */
extern Type *
primitive__parameter_type( Primitive *p, unsigned int i );

/** \return  whether the primitive is referentially transparent with respect to
    the given parameter */
extern boolean
primitive__parameter_reftrans( Primitive *p, unsigned int i );

/** Serializes the primitive (i.e. its type signature) to a string. */
extern void
primitive__encode( Primitive *prim, char *buffer );

/*p2_object *primitive__apply( Primitive *prim, void **args );*/


/******************************************************************************/


/** \return  an instance of the "primitive" data type */
extern Type *
primitive__create_type( const char *name );


/******************************************************************************/

#endif  /* PRIMITIVE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
