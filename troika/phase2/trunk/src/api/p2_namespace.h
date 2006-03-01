/**

\file  p2_namespace.h

\brief  A dictionary which may contain other dictionaries.

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

#ifndef P2_NAMESPACE_H
#define P2_NAMESPACE_H


#include "util/p2_hash_table.h"
#include "p2_object.h"
#include "p2_name.h"


typedef p2_object p2_namespace__object;

/** A namespace is a hash table with associated functions for adding, removing,
    and looking up named objects. */
typedef struct _p2_namespace
{
    /** A dictionary of objects in the namespace. */
    p2_hash_table *children;

    /** If nonzero, the namespace can only be read from, not written to. */
    int constant;

} p2_namespace;


/** \return  a new namespace */
p2_namespace *p2_namespace__new();

/** \param ns  the namespace to destroy */
void p2_namespace__delete( p2_namespace *ns );

/** Adds an object to a namespace.
    \param ns  the target namespace
    \param name  the local part of the name to assign
    \param o  the object to add
    \return  the object displaced by the new */
p2_object *p2_namespace__add( p2_namespace__object *ns_obj, p2_name *name, p2_object *o );

/** Finds an object in a namespace.
    \param ns  the target namespace
    \param name  the local part of the name to look up
    \return  the object found */
p2_object *p2_namespace__lookup( p2_namespace__object *ns_obj, p2_name *name );

/** Removes an object from a namespace.
    \param ns  the target namespace
    \param name  the local part of the name to look up
    \return  the object removed */
p2_object *p2_namespace__remove( p2_namespace__object *ns_obj, p2_name *name );


void *p2_namespace__for_all( p2_namespace *ns, void *(*func)(void *) );


#endif /* P2_NAMESPACE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
