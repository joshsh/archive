/**
    \file  Environment.h

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

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


#include <Memory_Manager.h>
#include <Namespace.h>
#include <Primitive.h>


typedef struct Environment Environment;

/** A data structure which is equivalent to a namespace containing uniquely
    named data types, function primitives, and data sets. */
struct Environment
{
    Memory_Manager *manager;

    Namespace_o *root;

    Namespace_o *combinators;
    Namespace_o *data;
    Namespace_o *primitives;
    Namespace_o *types;

    Type *combinator_t, *ns_t, *prim_t, *type_t;

    /* Parser types. */
    Type *bag_t, *char_t, *float_t, *int_t, *string_t, *term_t;
};


/** \return  a new environment */
extern Environment *
environment__new();


/** Destroys an environment. */
extern void
environment__delete( Environment *env );


extern Object *
environment__register_primitive
    ( Environment *env, Primitive *prim, int flags, Generic_f src_f );


extern Object *
environment__register_type( Environment *env, Type *type );


extern Type *
environment__resolve_type( Environment *env, const char *name );


/* Defined in p2-stubs.c */
extern void *
environment__import_primitives( Environment *env );


#endif  /* ENVIRONMENT_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
