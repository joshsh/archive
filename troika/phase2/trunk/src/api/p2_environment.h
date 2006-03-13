/**
    \file  p2_environment.h

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

#ifndef P2_ENVIRONMENT_H
#define P2_ENVIRONMENT_H


#include <p2_memory_manager.h>
#include <p2_namespace.h>
#include <p2_primitive.h>


/** A data structure which is equivalent to a namespace containing uniquely
    named data types, function primitives, and data sets. */
typedef struct _p2_environment
{
    p2_memory_manager *manager;

    p2_namespace_o *root;

    p2_namespace_o *data;
    p2_namespace_o *primitives;
    p2_namespace_o *types;

    p2_type *ns_t, *prim_t, *type_t;

} p2_environment;


/** \return  a new environment */
p2_environment *p2_environment__new();


/** Destroys an environment. */
void p2_environment__delete( p2_environment *env );


p2_object *p2_environment__register_primitive(
    p2_environment *env,
    p2_primitive *prim,
    int flags,
    void ( *src_f ) ( void ) );


p2_object *p2_environment__register_type(
    p2_environment *env,
    p2_type *type,
    int flags );


p2_type *p2_environment__resolve_type(
    p2_environment *env,
    const char *name );


/* Externally linked function defined in p2-stubs.c */
extern void *p2_environment__import_primitives( p2_environment *env );


#endif  /* P2_ENVIRONMENT_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
