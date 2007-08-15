/**

\file  Primitive-impl.h

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

#ifndef PRIMITIVE_IMPL_H
#define PRIMITIVE_IMPL_H


#include <Primitive.h>
#include "settings.h"


typedef struct Parameter Parameter;

struct Parameter
{
    /** A helpful name. */
    char *name;

    /** Data type. */
    Type *type;

    /** Whether the parent primitive may have a side-effect on this parameter. */
    boolean transparent;
};


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
    Parameter *parameters;
};


#endif  /* PRIMITIVE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
