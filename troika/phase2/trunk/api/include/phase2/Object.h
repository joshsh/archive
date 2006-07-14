/**

\file  Object.h

\brief  Provides a class which handles method dispatch and graph traversal.

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

#ifndef OBJECT_H
#define OBJECT_H


#include <Type.h>


/** An explicitly typed value. */
typedef struct Object Object;


/******************************************************************************/


/** Constructor.
    \param type  the new object's data type
    \param value  the new object's data.  The object assumes ownership of this
    data, and will deallocate it on deletion as per its data type.
    \param flags  object properties
    \warning  don't call this function directly if you are using a memory
    manager */
extern Object *
object__new( Type *type, void *value, int flags );

/** Destructor. */
extern void
object__delete( Object *o );


/* Accessors ******************************************************************/


/** \return  whether the object has been flagged as immutable.  This indicates
    a sensitive built-in object which resists the effects of publicly-visible
    functions */
extern boolean
object__immutable( const Object *o );

/** \return  the data type of the object */
extern Type *
object__type( const Object *o );

/** \return  the value of the object.  This will be an instance of the
    object's associated data type */
extern void *
object__value( const Object *o );


/******************************************************************************/


/** Serializes an object to a string. */
extern void
object__encode( const Object *o, char *buffer );


/******************************************************************************/

/** \return  an instance of the "object" type */
extern Type *
object__create_type( const char *name );


/* Graph traversal ************************************************************/

/** A (depth-first) recursive distributor.
    \param o  root of the traversal
    \param f  distributor function to apply to reachable objects
    \param follow_triples  whether to follow links to "associated" objects
    \param redirect  whether to "short out" indirection nodes as they are
    encountered */
extern void
object__trace( Object *o, Visitor f, boolean follow_triples );

/** A breadth-first recursive distributor.
    \param o  root of the traversal
    \param f  distributor function to apply to reachable objects
    \param follow_triples  whether to follow links to "associated" objects
    \param redirect  whether to "short out" indirection nodes as they are
    encountered */
extern void
object__trace_bfs( Object *o, Visitor f, boolean follow_triples );


/******************************************************************************/


#endif  /* OBJECT_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
