/**

\file  Type.h

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

#ifndef TYPE_H
#define TYPE_H


#include <defs.h>


enum Type__Flags
{
    TYPE__IS_OBJ_COLL         = 0x1,
    TYPE__OWNS_DESCENDANTS    = 0x2
};


typedef struct Type Type;

/** */
struct Type
{
    /** */
    char *name;

    int flags;

    Type *type_arg;

    /** Copy constructor. */
    Copy_Cons   clone;

    Comparator  compare_to;

    /** Deserializer. */
    Decoder     decode;

    /** Destructor. */
    Destructor  destroy;

    /** Serializer. */
    Encoder     encode;

    Criterion2  equals;

    Size_Of     size;

    Sort        sort;

    Walker      walk;
};


#define any_type    ( Type* ) 42
/* #define dist_type   ( Type* ) 1001 */


/** Constructor. */
extern Type *
type__new( const char *name, int flags );


/** Destructor. */
extern void
type__delete( Type *type );


/******************************************************************************/

extern Type *
type__create_type( const char *name, int flags );


#endif  /* TYPE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
