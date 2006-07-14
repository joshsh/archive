/**

\file  Graph.h

\brief  Provides a sparse graph class.

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

#ifndef GRAPH_H
#define GRAPH_H


#include <collection/Hash_Table.h>


/** An object which stands in for an adjacency table.  Good for sparse graphs. */
typedef Hash_Table Graph;


/** \return  a new graph object */
extern Graph *
graph__new( void );

/** Destructor. */
extern void
graph__delete( Graph *g );


/** Adds an edge to the graph. */
extern void
graph__connect( Graph *g, void * const src, void * const dest );

/** Removes an edge from the graph. */
extern void
graph__disconnect( Graph *g, void * const src, void * const dest );


#endif  /* GRAPH_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
