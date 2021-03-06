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


/** A data structure which is equivalent to a namespace containing uniquely
    named data types, function primitives, and data sets. */
typedef struct Environment Environment;

/* ~ */
typedef Object Type_o;
typedef Object Primitive_o;
typedef Object Combinator_o;


/** \return  a new environment */
extern Environment *
environment__new();

/** Destroys an environment. */
extern void
environment__delete( Environment *env );


extern Memory_Manager *
environment__manager( const Environment *env );

extern Namespace_o *
environment__data( const Environment *env );

extern Namespace_o *
environment__root( const Environment *env );


extern Object *
environment__register_type( Environment *env, Type *type );

extern Type_o *
environment__resolve_type( Environment *env, const char *name );


extern Object *
environment__register_primitive
    ( Environment *env, Primitive *prim, int flags, Generic_f src_f );

extern Primitive_o *
environment__resolve_primitive( Environment *env, const char *name );

/* Defined in p2-stubs.c */
extern void *
environment__import_primitives( Environment *env );


extern Combinator_o *
environment__resolve_combinator( Environment *env, const char *name );


#endif  /* ENVIRONMENT_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
