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


static p2_action * graph_edge__delete( graph_edge *edge, void *ignored )
{
    free( edge );
    return 0;
}


static unsigned int hash( const graph_edge *edge )
{
    return ( unsigned int ) edge->dest - ( unsigned int ) edge->src;
}


/* Works well enough for addresses so long as there's a one-to-one correspondence
   between addresses and key values. */
static int compare(
    const graph_edge *edge1,
    const graph_edge *edge2 )
{
    return ( edge1->src != edge2->src || edge1->dest != edge2->dest );
}


/******************************************************************************/


p2_graph *p2_graph__new()
{
    Hash_Table *h = hash_table__new( 0, 0, 0,
        ( hash_f ) hash, ( comparator ) compare );

    return h;
}


void p2_graph__delete( p2_graph *g )
{
    /* Destroy graph entries. */
    p2_procedure p = { ( procedure ) graph_edge__delete, 0 };
    hash_table__distribute( g, &p );

    hash_table__delete( g );
}


void p2_graph__connect
    ( p2_graph *g, void * const src, void * const dest )
{
    graph_edge *edge_new, *edge_old;

    if ( !( edge_new = new( graph_edge ) ) )
        return;

    edge_new->src = src;
    edge_new->dest = dest;

    if ( ( edge_old = ( graph_edge* ) hash_table__add( g, edge_new ) ) )
        free( edge_old );
}


void p2_graph__disconnect
    ( p2_graph *g, void * const src, void * const dest )
{
    graph_edge *edge_old;
    graph_edge edge_cmp;
    edge_cmp.src = src;
    edge_cmp.dest = dest;

    if ( ( edge_old = ( graph_edge* ) hash_table__remove( g, &edge_cmp ) ) )
        free( edge_old );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
