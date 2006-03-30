/**

\file  Namespace.h

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

#ifndef NAMESPACE_H
#define NAMESPACE_H


#include <Object.h>
#include <util/Dictionary.h>
#include <util/Name.h>


typedef Object Namespace_o;

typedef struct Namespace Namespace;

/** A namespace is a hash table with associated functions for adding, removing,
    and looking up named objects. */
struct Namespace
{
    /** A dictionary of objects in the namespace. */
    Dictionary *children;
};


/** \return  a new namespace */
extern Namespace *
namespace__new();

/** \param ns  the namespace to destroy */
extern void
namespace__delete( Namespace *ns );

/** Adds an object to a namespace.
    \param ns  the target namespace
    \param name  the local part of the name to assign
    \param o  the object to add
    \return  the object displaced by the new */
extern Object *
namespace__add( Namespace_o *ns_obj, Name *name, Object *o );

extern Object *
namespace__add_simple( Namespace *ns, const char *name, Object *o );

/** Finds an object in a namespace.
    \param ns  the target namespace
    \param name  the local part of the name to look up
    \return  the object found */
extern Object *
namespace__lookup( Namespace_o *ns_obj, Name *name );

/** Finds an object using a simple name.
    \param ns  the target namespace
    \param s  the local part of the name to look up
    \return  the object found */
extern Object *
namespace__lookup_simple( Namespace *ns, const char *name );

/** Removes an object from a namespace.
    \param ns  the target namespace
    \param name  the local part of the name to look up
    \return  the object removed */
extern Object *
namespace__remove( Namespace_o *ns_obj, Name *name );

extern Object *
namespace__remove_simple( Namespace *ns, char *name );


extern void
namespace__show_children( const Namespace_o *ns_obj );

extern void
namespace__walk( Namespace *ns, Dist_f f );


/******************************************************************************/

extern Type *
namespace__create_type( const char *name, int flags );


/******************************************************************************/

extern Name *
namespace__find( const Namespace_o *ns_obj, const Object *o );


#endif /* NAMESPACE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */