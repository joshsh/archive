/**

\file  Object.h

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

#ifndef OBJECT_H
#define OBJECT_H


#include <Type.h>


enum Object__Flags
{
    OBJECT__IMMUTABLE           = 0x1,
    OBJECT__VISITED             = 0x2,
    /*OBJECT__NATIVE*/
    OBJECT__OWNED               = 0x4
};


typedef struct Object Object;


/******************************************************************************/

/** Constructor.
    \param type  the new object's data type
    \param value  the new object's data.  The object assumes ownership of this
    data, and will deallocate it on deletion as per its data type. */
extern Object *
object__new( Type *type, void *value, int flags );

/** Destructor. */
extern void
object__delete( Object *o );


/* Accessors ******************************************************************/

extern boolean
object__immutable( const Object *o );

extern Type *
object__type( const Object *o );

extern void *
object__value( const Object *o );


/******************************************************************************/

extern Type *
object__create_type( const char *name );


/* Graph traversal ************************************************************/

/** A (depth-first) recursive distributor. */
extern void
object__trace( Object *o, Dist_f f, boolean follow_triples );

/** A breadth-first recursive distributor. */
extern void
object__trace_bfs( Object *o, Dist_f f, boolean follow_triples );


/******************************************************************************/


#endif  /* OBJECT_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
