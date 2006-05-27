/**

\file  graph.h

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

#ifndef GRAPH_H
#define GRAPH_H

#include <Memory_Manager.h>
#include <util/Array.h>

#include "Apply.h"
#include "../settings.h"


extern Type *apply_type, *indirection_type;


extern void
graph_init( Type *combinator_t, Type *primitive_t );

extern void
graph_end();

extern Object *
reduce__graph_lazy( Object *o, Array *spine, Memory_Manager *m );


#endif  /* GRAPH_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
