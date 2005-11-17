/**

\file  import-aux.h

\brief  Primitive-building functions to be included by the generated file
p2-import.c.

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

#include "p2_primitive.h"


/** Begins a p2_primitive declaration.
    \return  null for failure, non-null for success.  You must now fill in the
    primitive's parameters using p2_primdef__parameter. */
void *p2_primdef__head(
    void *( *cstub )( void** ),
    char *name,
    char *return_type_id,
    int parameters );

/** Defines a p2_primitive input parameter.
    \return  null for failure, non-null for success */
void *p2_primdef__parameter(
    char *param_name,
    char *param_type_id,
    char param_transparency );

/** Ends a p2_primitive declaration, registering the p2_primitive.
    \return  null for failure, non-null for success */
void *p2_primdef__tail( );


#endif  // IMPORT_AUX_H

