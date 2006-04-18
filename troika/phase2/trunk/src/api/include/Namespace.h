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
#include <Memory_Manager.h>
#include <util/Dictionary.h>
#include <util/Name.h>


typedef Object Namespace_o;

typedef struct Namespace Namespace;



/** \return  a new namespace */
extern Namespace *
namespace__new();

/** \param ns  the namespace to destroy */
extern void
namespace__delete( Namespace *ns );

extern unsigned int
namespace__size( Namespace *ns );

/** Adds an object to a namespace.
    \param ns  the target namespace
    \param name  the local part of the name to assign
    \param o  the object to add
    \return  the object added, or 0 if it could not be added */
extern Object *
namespace__add( Namespace_o *ns_obj, Name *name, Object *o );

extern Object *
namespace__add_simple( Namespace *ns, const char *name, Object *o );

extern void
namespace__add_all( Namespace *dest, Namespace *src );


extern Array *
namespace__keys( Namespace *ns );


/** Finds an object in a namespace.
    \param ns  the target namespace
    \param name  the local part of the name to look up
    \return  the object found */
extern Object *
namespace__lookup( Namespace_o *ns_obj, Name *name );

/** Finds an object using a simple name.
    \param ns  the target namespace
    \param s  the local part of the name to look up
    \return  the object found, or 0 if not found */
extern Object *
namespace__lookup_simple( Namespace *ns, const char *name );

/** Removes an object from a namespace.
    \param ns  the target namespace
    \param name  the local part of the name to look up
    \return  the object removed, or 0 if not found */
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
namespace__find( Namespace_o *haystack, Object *needle, Memory_Manager *m );

extern Object *
namespace__resolve_simple( Namespace_o *ns_obj, char *name, Memory_Manager *m );

extern Object *
namespace__resolve( Namespace_o *ns_obj, Name *name, Memory_Manager *m );

extern Object *
namespace__undefine( Namespace_o *nso, Name *name, Memory_Manager *m );


#endif /* NAMESPACE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
