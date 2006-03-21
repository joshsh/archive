/**

\file  p2_object.h

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

#ifndef P2_OBJECT_H
#define P2_OBJECT_H


#include <p2_type.h>

#ifdef TRIPLES__GLOBAL
#include <util/p2_lookup_table.h>
#endif


enum p2_object__flags
{
    OBJECT__IMMUTABLE           = 0x1,
    OBJECT__VISITED             = 0x2,
    /*OBJECT__NATIVE*/
    OBJECT__OWNED               = 0x4
};


typedef struct p2_object p2_object;

/** A typed constant.  This is the least addressable unit of data in a Phase2
    namespace. */
struct p2_object
{
    /** A reference to the object's data type. */
    p2_type *type;

    /** A reference to the object's data. */
    void *value;

    /** A mutable value which holds tracing and state information about the
        object. */
    int flags;

    #if TRIPLES__GLOBAL__IN_EDGES
    /** Associative edges pointing towards the atom.
        For example, (x, y) from (x, y, Z). */
    p2_lookup_table *inbound_edges;
    #endif

    #if TRIPLES__GLOBAL__OUT_EDGES
    /** Associative edges pointing away from the atom.
        For example, (y, z) from (X, y, z). */
    p2_lookup_table *outbound_edges;
    #endif

    #if TRIPLES__GLOBAL__TRANS_EDGES
    /** Associative edges pointing "through" the atom.
        For example, (x, z) from (x, Y, z). */
    p2_lookup_table *trans_edges;
    #endif
};

typedef p2_object Object;


/******************************************************************************/

/** Constructor.
    \param type  the new object's data type
    \param value  the new object's data.  The object assumes ownership of this
    data, and will deallocate it on deletion as per its data type. */
p2_object *p2_object__new( p2_type *type, void *value, int flags );

/** Destructor. */
void p2_object__delete( p2_object *o );


/* Accessors ******************************************************************/
/*
extern Type *object__type( Object *o );

extern void
*/

/* Graph traversal ************************************************************/

/** A (depth-first) recursive distributor. */
void p2_object__trace( p2_object *o, p2_procedure *p );

/** A breadth-first recursive distributor. */
void p2_object__trace_bfs( p2_object *o, p2_procedure *p );


/* Association ****************************************************************/

#if TRIPLES__GLOBAL

p2_object *p2_object__multiply
    ( p2_object *subj, p2_object *pred );

p2_object *p2_object__associate
    ( p2_object *subj, p2_object *pred, p2_object *obj );

#endif


/******************************************************************************/


#endif  /* P2_OBJECT_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
