/**

\file  Primitive-import.h

\brief  Provides the means to build function primitives.

Used by the generated file p2-import.c.

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

#ifndef PRIMITIVE_IMPORT_H
#define PRIMITIVE_IMPORT_H

#include <phase2/Environment.h>


/** \return  An incomplete function primitive */
extern Primitive *
primitive__new(
    Environment *env,
    const char *return_type,
    const char *name,
    void *( *cstub )( void** ),
    int arity );


/** \return  The function primitive p after adding a parameter */
extern Primitive *
primitive__add_param(
    Environment *env,
    Primitive *p,
    char *type,
    char *name,
    boolean transparent );


/** \return  The same primitive after adding it to the programming environment */
extern Primitive *
primitive__register(
    Environment *env,
    Primitive *p,
    int flags,
    void ( *src_f ) ( void ) );


#endif  /* PRIMITIVE_IMPORT_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
