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

#ifndef OBJECT_IMPL_H
#define OBJECT_IMPL_H


#include <phase2/Object.h>
#include "../type/Type-impl.h"


#if TRIPLES__GLOBAL
#include <phase2/Hash_Map.h>
#endif


#ifndef DEBUG__OBJECT
#define DEBUG__OBJECT   0
#endif


enum Object__Flags
{
    OBJECT__IMMUTABLE           = 0x1,
    OBJECT__IRREDUCIBLE         = 0x2,
    OBJECT__VISITED             = 0x4,
    /*OBJECT__NATIVE*/
    OBJECT__OWNED               = 0x8
};


/** A typed constant.  This is the least addressable unit of data in a Phase2
    namespace. */
struct Object
{
    /** A reference to the object's data type. */
    Type *type;

    /** A reference to the object's data. */
    void *value;

    /** A mutable value which holds tracing and state information about the
        object. */
    unsigned int flags;

    #if TRIPLES__GLOBAL__IN_EDGES
    /** Associative edges pointing towards the atom.
        For example, (x, y) from (x, y, Z). */
    Hash_Map *inbound_edges;
    #endif

    #if TRIPLES__GLOBAL__OUT_EDGES
    /** Associative edges pointing away from the atom.
        For example, (y, z) from (X, y, z). */
    Hash_Map *outbound_edges;
    #endif

    #if TRIPLES__GLOBAL__TRANS_EDGES
    /** Associative edges pointing "through" the atom.
        For example, (x, z) from (x, Y, z). */
    Hash_Map *trans_edges;
    #endif
};


/* Association ****************************************************************/

#if TRIPLES__GLOBAL

extern Object *
object__multiply( Object *subj, Object *pred );

extern Object *
object__associate( Object *subj, Object *pred, Object *obj );

extern Object *
object__dissociate( Object *subj, Object *pred );

#ifdef NOT_FINISHED
/* ~ */
extern Type set_t;

extern Object *
object__union_associate( Object *subj, Object *pred, Object *obj );
#endif

#endif


#endif  /* OBJECT_IMPL_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
