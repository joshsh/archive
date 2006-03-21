/**

\file  p2_type.h

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

#ifndef P2_TYPE_H
#define P2_TYPE_H

#include <p2.h>


typedef struct p2_type p2_type;

enum p2_type__flags
{
    TYPE__IS_OBJ_COLL         = 0x1,
    TYPE__OWNS_DESCENDANTS    = 0x2
};


/** */
struct p2_type
{
    /** */
    char *name;

    int flags;

    p2_type *type_arg;

    /** Copy constructor. */
    copy_cons   clone;

    comparator  compare_to;

    /** Deserializer. */
    decoder     decode;

    /** Destructor. */
    destructor  destroy;

    distributor distribute;

    /** Serializer. */
    encoder     encode;

    criterion2  equals;

    sort_f      sort;
};


#define any_type    ( p2_type* ) 42


/** Constructor. */
p2_type *p2_type__new( const char *name, int flags );


/** Destructor. */
void p2_type__delete( p2_type *type );


#endif  /* P2_TYPE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
