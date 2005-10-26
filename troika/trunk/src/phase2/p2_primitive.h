/**
    \file p2_primitive.h

    \author Joshua Shinavier   \n
            parcour@gmail.com  \n
            +1 509 570-6990    \n */

/*//////////////////////////////////////////////////////////////////////////////

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

*///////////////////////////////////////////////////////////////////////////////

#ifndef P2_PRIMITIVE_H
#define P2_PRIMITIVE_H


#include "p2_flags.h"
#include "p2_type.h"  // p2_type

#ifdef P2FLAGS__MANAGE_PRIMITIVES
    #include "p2_error.h"  // p2_error
#endif


////////////////////////////////////////////////////////////////////////////////

/** A structure containing a C function pointer together with typing information
    and a unique name. */
typedef struct
{
  /** Memory location of the C function. */
  void *value;

  /** A unique name (preferably the same as the C function stub referenced by
      the pointer, e.g. "math_h__cos"). */
  char *name;

  /** Number of input parameters. */
  int parameters;

  /** An array of length {parameters} containing the data type of all input
      parameters in the correct order. */
  p2_type *parameter_types;

  /** \note  Not used. */
  char **parameter_names;

  /** An array of length {parameters} containing at each parameter index a 0
      if the argument at that index may experience a side-effect, or a 1
      otherwise (the primitive is said to be "referentially transparent" with
      respect to that parameter).
      \note  Not yet used. */
  char *transparency;

  /** The primitive's (constant) return type. */
  p2_type return_type;

} p2_primitive;


////////////////////////////////////////////////////////////////////////////////

/** Create a new p2_primitive.
    \note  Array arguments are copied into the new structure (and not simply
    referenced again).
    \warning  Arguments are assumed to be sound. */
p2_primitive *p2_primitive__new(
    void *value,               /**< Memory location of the C function. */
    char *name,                /**< A unique name (preferably the same as the C function stub referenced by the pointer, e.g. "math_h__cos"). */
    int parameters,            /**< Number of input parameters. */
    p2_type *parameter_types,  /**< An array of length {parameters} containing the data type of all input parameters in the correct order. */
    char **parameter_names,    /**< \note  Not used.  May pass a NULL instead. */
    char *transparency,        /**< An array of length {parameters} containing at each parameter index a 0 if the argument at that index may experience a side-effect, or a 1 otherwise (the primitive is said to be "referentially transparent" with respect to that parameter).
                                    \note  Not yet used.  May pass a NULL instead. */
    p2_type return_type);      /**< The primitive's (constant) return type. */


////////////////////////////////////////////////////////////////////////////////

#ifdef P2FLAGS__MANAGE_PRIMITIVES

    p2_error p2_primitive_init();

    p2_error p2_primitive_end();

    p2_primitive *p2_primitive__lookup(char *name);

    /** Serialize a p2_primitive to a string. */
    void p2_primitive__encode(p2_primitive *p, char *buffer);

    /** Deserialize a p2_primitive from a string. */
    p2_primitive *p2_primitive__decode(char *buffer);

#else

   /** Destroy the p2_primitive. */
   void p2_primitive__delete(p2_primitive *prim);

#endif  // P2FLAGS__MANAGE_PRIMITIVES


////////////////////////////////////////////////////////////////////////////////

#endif  // P2_PRIMITIVE_H

