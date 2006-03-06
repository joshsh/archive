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

#include "p2_graph.h"


typedef struct _graph_edge
{
    void *src;
    void *dest;

} graph_edge;


p2_procedure__effect graph_edge__delete( graph_edge *edge, void *state )
{
    free( edge );
    return p2_procedure__effect__continue;
}


static unsigned int hash( const graph_edge *edge )
{
    return ( unsigned int ) edge->src;
}


/* Works well enough for addresses so long as there's a one-to-one correspondence
   between addresses and key values. */
static int compare(
    const graph_edge *edge1,
    const void *src2 )
{
    return ( edge1->src != src2 );
}


/******************************************************************************/


p2_graph *p2_graph__new()
{
    p2_hash_table *h = p2_hash_table__new( 0, 0, 0,
        ( hash_f ) hash, ( comparator ) compare );

    return h;
}


void p2_graph__delete( p2_graph *g )
{
    /* Destroy graph entries. */
    p2_procedure p = { ( procedure ) graph_edge__delete, 0 };
    p2_hash_table__distribute( g, &p );

    p2_hash_table__delete( g );
}


void p2_graph__connect
    ( p2_graph *g, void * const src, void * const dest )
{
    graph_edge *edge, *edge_old;

    if ( !( edge = new( graph_edge ) ) )
        return;

    edge->src = src;
    edge->dest = dest;

    if ( ( edge_old = ( graph_edge* ) p2_hash_table__add( g, edge ) ) )
        free( edge_old );
}


void p2_graph__disconnect
    ( p2_graph *g, void * const src, void * const dest )
{
    graph_edge *edge_old;

    if ( ( edge_old = ( graph_edge* ) p2_hash_table__remove( g, src ) ) )
        free( edge_old );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
