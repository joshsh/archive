/**
    \file  Memory_Manager.h

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

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <Object.h>
#include <util/Bunch.h>
#include <util/Set.h>


typedef struct Memory_Manager Memory_Manager;


/******************************************************************************/

extern Memory_Manager *
memory_manager__new( Object *root );

extern void
memory_manager__delete( Memory_Manager *m );

extern unsigned int
memory_manager__size( Memory_Manager *m );

extern void
memory_manager__set_root( Memory_Manager *m, Object *o );

extern Object *
memory_manager__add( Memory_Manager *m, Object *o );


/* Tracing / graph traversal **************************************************/

extern void
memory_manager__distribute( Memory_Manager *m, Closure *p );

extern Set *
memory_manager__get_multirefs( Memory_Manager *m, Object *root );


/* Mark-and-sweep garbage collection ******************************************/

extern void
memory_manager__mark_and_sweep( Memory_Manager *m );


#endif  /* MEMORY_MANAGER_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
