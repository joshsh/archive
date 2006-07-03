/**
    \file  Memory_Manager.h

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

#include <Object.h>
#include <util/Bunch.h>
#include <util/Set.h>


/** \note  A memory manager is a closed system.  It owns all objects passed to
    it via memory_manager__add, and these objects are not allowed to
    reference any first-class objects which are not themselves owned by the
    manager. */
typedef struct Memory_Manager Memory_Manager;


/******************************************************************************/

/** Constructor. */
extern Memory_Manager *
memory_manager__new();

/** Destructor. */
extern void
memory_manager__delete( Memory_Manager *m );

/** \return the number of objects governed by the Memory_Manager */
extern unsigned int
memory_manager__size( Memory_Manager *m );

extern void
memory_manager__set_root( Memory_Manager *m, Object *o );

/** Requests a new object of the manager. */
extern Object *
memory_manager__object( Memory_Manager *m, Type *type, void *value, int flags );


/* Tracing / graph traversal **************************************************/

extern void
memory_manager__walk
    ( Memory_Manager *m, Object *root, Dist_f f, boolean use_bfs, boolean follow_triples );

extern void
memory_manager__trace
    ( Memory_Manager *m, Object *root, Walker walk, Dist_f dist );

/** \return  the set of all objects to which there is more than one path from
    root.  Important for serialization. */
extern Set *
memory_manager__get_multirefs( Memory_Manager *m, Object *root );


/* Mark-and-sweep garbage collection ******************************************/

/** Deletes all nonreachable objects. */
extern void
memory_manager__collect( Memory_Manager *m );

/** Invokes memory_manager__collect if the heap has exceeded a threshold
    determined after the last collection cycle. */
extern void
memory_manager__collect_if_needed( Memory_Manager *m );


#endif  /* MEMORY_MANAGER_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
