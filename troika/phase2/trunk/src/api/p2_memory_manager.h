/**
    \file  p2_memory_manager.h

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

#ifndef P2_MEMORY_MANAGER_H
#define P2_MEMORY_MANAGER_H

#include <p2_object.h>
#include <util/p2_bunch.h>
#include <util/p2_set.h>


/** \note  A memory manager is a closed system.  It owns all objects passed to
    it via p2_memory_manager__add, and these objects are not allowed to
    reference any first-class objects which are not themselves owned by the
    manager. */
typedef struct _p2_memory_manager
{
    p2_bunch *objects;
        p2_object *objects_o;

    p2_object *root;

    /** Whether the manager contains only unmarked objects.  Any marked objects
        need to be unmarked prior to any algorithm which relies on object
        marking. */
    int clean;

} p2_memory_manager;


/******************************************************************************/

p2_memory_manager *p2_memory_manager__new( p2_object *root );
void p2_memory_manager__delete( p2_memory_manager *m );

unsigned int p2_memory_manager__size( p2_memory_manager *m );

p2_object *p2_memory_manager__add( p2_memory_manager *m, p2_object *o );


/* Tracing / graph traversal **************************************************/

void p2_memory_manager__distribute( p2_memory_manager *m, p2_procedure *p );

p2_set *p2_memory_manager__get_multirefs( p2_memory_manager *m );


/* Mark-and-sweep garbage collection ******************************************/

void p2_memory_manager__mark_and_sweep( p2_memory_manager *m );


#endif  /* P2_MEMORY_MANAGER_H */
/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
