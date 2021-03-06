/**

\file  Manager.h

\brief  Provides a class which creates, mutates and destroys object graphs.
Includes a tracing garbage collector.

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

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <phase2/Object.h>
#include <phase2/Bunch.h>
#include <phase2/Set.h>


/** \note  A memory manager is a closed system.  It owns all objects passed to
    it via manager__add, and these objects are not allowed to
    reference any first-class objects which are not themselves owned by the
    manager. */
typedef struct Manager Manager;


/******************************************************************************/

/** Constructor. */
extern Manager *
manager__new();

/** Destructor. */
extern void
manager__delete( Manager *m );

/** \return the number of objects governed by the Manager */
extern unsigned int
manager__size( Manager *m );

extern void
manager__set_root( Manager *m, Object *o );

/** Requests a new object of the manager. */
extern Object *
manager__object( Manager *m, Type *type, void *value, int flags );


/* Tracing / graph traversal **************************************************/

/** Distributes a visitor function to each object (exactly once, or until told
    to halt) in the graph rooted the given object.
    \param root  the object to start at.  If NULL, the algorithm will walk from
    the default root of the object graph
    \param use_bfs  if TRUE, traverse the graph in a breadth-first rather than
    a depth-first manner
    \param follow_triples  if TRUE, visit all statements of which the visited
    object is the subject */
extern void
manager__walk
    ( Manager *m, Object *root, Visitor f, boolean use_bfs, boolean follow_triples );

/** Like manager__walk, but using a customized walk function.  The
    algorithm will not visit any object more than once. */
extern void
manager__trace
    ( Manager *m, Object *root, Walker walk, Visitor dist );

/** \return  the set of all objects to which there is more than one path from
    root.  Important for serialization. */
extern Set *
manager__get_multirefs( Manager *m, Object *root );


/* Mark-and-sweep garbage collection ******************************************/

/** Deletes all nonreachable objects if the heap has exceeded a threshold
    determined after the last collection cycle.
    \param force  collect even if the threshold has not been reached
    \param echo  print a message indicating what was collected */
extern void
manager__collect( Manager *m, boolean force, boolean echo );


#endif  /* MEMORY_MANAGER_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
