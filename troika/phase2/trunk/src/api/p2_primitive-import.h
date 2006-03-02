/**

\file  p2_primitive-import.h

\brief  Primitive-building functions to be referenced by the generated file
p2-import.c.

\todo  After calling p2__import_primitives, the type registry must be checked
for incomplete types.

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

#ifndef P2_PRIMITIVE_IMPORT_H
#define P2_PRIMITIVE_IMPORT_H

#include "p2_environment.h"


p2_primitive *p2_primitive__new(
    p2_environment *env,
    const char *return_type,
    const char *name,
    void *( *cstub )( void** ),
    int arity );


p2_primitive *p2_primitive__add_param(
    p2_environment *env,
    p2_primitive *p,
    char *type,
    char *name,
    int transparent );


p2_primitive *p2_primitive__register(
    p2_environment *env,
    p2_primitive *p,
    int flags,
    void ( *src_f ) ( void ) );


#endif  /* P2_PRIMITIVE_IMPORT_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
