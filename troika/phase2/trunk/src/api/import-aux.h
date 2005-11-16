/**

\file  import-aux.h

\brief  Primitive-building functions to be included by the generated file
import.c.

\author  Joshua Shinavier   \n
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

#ifndef IMPORT_AUX_H
#define IMPORT_AUX_H

#include "p2_primitive.h"  // p2_function_ptr, p2_primitive


/** \return  a new (but incomplete) p2_primitive.  You must fill in the
    primitive's parameters using prim__set_parameter. */
p2_primitive *prim__new( p2_function_ptr func,
                         char *name,
                         char *return_type_id,
                         int parameters );

/** \return  the original primitive with its new parameter */
p2_primitive *prim__set_parameter(
    p2_primitive *prim,
    char *param_name,
    char *param_type_id,
    char param_transparency,
    int index );


#endif  // IMPORT_AUX_H

