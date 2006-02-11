#include "P2FreeFormLayoutTree.h"


////////////////////////////////////////////////////////////////////////////////


// Arbitrary, fixed index for the root of the spanning tree.
#define ROOT  0


P2FreeFormLayoutTree::P2FreeFormLayoutTree()
    : QList< P2FreeFormLayoutEdge >()
{ }


P2FreeFormLayoutTree::P2FreeFormLayoutTree( const P2FreeFormLayoutTree &other )
    : QList< P2FreeFormLayoutEdge >( other )
{ }


/*
--- Prim's Minimum Spanning Tree Algorithm ---
    http://www.csse.monash.edu.au/~lloyd/tildeAlgDS/Graph/Undirected/

--Graph G = <V, E>

done := {v1}   --initial Tree is <{v1},{}>

for vertex i in V-{v1}
   T[i] := E[1,i]     --direct edges (possibly "missing")
end for

loop |V|-1 times
--Inv: {T[v]|v in done} represents a min' spanning Tree
--     of the nodes in done and
--     {T[u]|u not in done} contains the shortest known
--     distances from the (sub-)spanning Tree to
--     such vertices u.

  find closest vertex to (sub-)spanning Tree in V - done

  done +:= {closest}
  add closest & edge T[closest] to (sub-)spanning Tree

  for vertex j in V - done
    T[j] := min(T[j],   --update knowledge on paths,
                E[closest,j])      --perhaps better?
  end for
end loop

*/
P2FreeFormLayoutTree::P2FreeFormLayoutTree( QList< QLayoutItem* > vertices )
    : QList< P2FreeFormLayoutEdge >()
{
    #ifdef DEBUG__LAYOUT__FF_LAYOUT_TREE
        cout << indent()
             << "P2FreeFormLayoutTree[" << (int) this
             << "]::P2FreeFormLayoutTree("
             << ( int ) &vertices << ")" << endl; cout.flush();
    #endif

    int size = vertices.size();
    if ( !size )
        // Nothing to do.
        return;

    // Generate a weighted adjacency matrix.
    int edgeWeights[ size ][ size ];
    for ( int i = 0; i < size; i++ )
    {
        QLayoutItem *srcItem = vertices.at( i );

        for ( int j = i + 1; j < size; j++ )
            edgeWeights[ i ][ j ] = edgeWeights[ j ][ i ]
                = P2FreeFormLayoutEdge::distance( srcItem, vertices.at( j ) );
    }

    #ifdef DEBUG__LAYOUT__FF_LAYOUT_TREE
        for ( int i = 0; i < size; i++ )
            edgeWeights[ i ][ i ] = 0;

        cout << indent()
             << "P2FreeFormLayoutTree adjacency matrix:\n"
             << indent() << "\t";
        for ( int j = 0; j < size; j++ )
            cout << "\t" << j << ":";
        cout << endl;
        for ( int i = 0; i < size; i++ )
        {
            cout << indent() << "\t" << i << ":\t";
            for ( int j = 0; j < size; j++ )
                cout << edgeWeights[ i ][ j ] << "\t";
            cout << endl;
        }
    #endif

    // A weighted inbound edge.
    struct TableEntry
    {
        int weight;
        int predecessor;
    };

    // Adjacency table to hold the spanning tree.
    TableEntry table[ size ];

    // High end holds vertices already in the spanning tree.  Low end holds
    // vertices not yet added to it.
    int rem[ size ], n = size - 1;
    for ( int v = 0; v < size; v++ )
    {
        rem[ v ] = v;
        table[ v ].weight = edgeWeights[ ROOT ][ v ];
        table[ v ].predecessor = ROOT;
    }

    // Root begins in the spanning tree.
    rem[ ROOT ] = n;
    rem[ n ] = ROOT;

    // Loop ( size - 1 ) times.
    while ( n )
    {
        // Find the closest vertex to the existing spanning tree.
        int src, dest, closest;
        int weight, minweight = INT__INFINITY;
        for ( src = n; src < size; src++ )
        {
            for ( dest = 0; dest < n; dest++ )
            {
                weight = edgeWeights[ rem[ src ] ][ rem[ dest ] ];
                if ( weight < minweight )
                {
                    minweight = weight;
                    closest = dest;
                }
            }
        }

        // Swap the closest vertex into the "finished" end of the array.
        n--;
        src = rem[ closest ];
        rem[ closest ] = rem[ n ];
        rem[ n ] = src;

        // Update shortest path information.
        for ( int v = 0; v < n; v++ )
        {
            dest = rem[ v ];
            weight = edgeWeights[ src ][ dest ];

            if ( weight < table[ dest ].weight )
            {
                table[ dest ].weight = weight;
                table[ dest ].predecessor = src;
            }
        }
    }

    #ifdef DEBUG__LAYOUT__FF_LAYOUT_TREE
        cout << indent()
             << "Spanning tree:\n"
             << indent() << "\t";
        for ( int j = 0; j < size; j++ )
            cout << j << ":\t";
        cout << endl << indent();
        for ( int j = 0; j < size; j++ )
            cout << "\t" << table[ j ].predecessor << "/" << table[ j ].weight;
        cout << endl;
    #endif

    // Arrange the vertices in the order of a depth-first traversal.
    n = size - 1;
    int v = 0;
    table[ ROOT ].weight = 0;  // The weight is now used as a "visited" flag.
    while ( n )
    {
        if ( table[ v ].weight )
        {
            int pred = table[ v ].predecessor;
            if ( table[ pred ].weight )
                v = pred;
            else
            {
                append( P2FreeFormLayoutEdge( vertices, pred, v ) );
                table[ v ].weight = 0;
                n--;
                v = ( v + 1 ) % size;
            }
        }
        else
            v = ( v + 1 ) % size;
    }
}


int P2FreeFormLayoutTree::size()
{
    // Root vertex does not have an inbound edge.
    return QList< P2FreeFormLayoutEdge >::size() + 1;
}


void P2FreeFormLayoutTree::applyTo( QList< QLayoutItem* > vertices )
{
    int size = QList< P2FreeFormLayoutEdge >::size();
    for ( int i = 0; i < size; i++ )
        at( i ).setDestPosition( vertices );
}


int P2FreeFormLayoutEdge::distance( QLayoutItem *src, QLayoutItem *dest )
{
    QRect rectA = src->geometry(), rectB = dest->geometry();

    int x_distance = ( rectA.x() > rectB.x() )
        ? rectA.x() - ( rectB.x() + rectB.width() )
        : rectB.x() - ( rectA.x() + rectA.width() );

    int y_distance = ( rectA.y() > rectB.y() )
        ? rectA.y() - ( rectB.y() + rectB.height() )
        : rectB.y() - ( rectA.y() + rectA.height() );

    return x_distance > y_distance
        ? x_distance
        : y_distance;
}


////////////////////////////////////////////////////////////////////////////////


P2FreeFormLayoutEdge::P2FreeFormLayoutEdge(
    QList< QLayoutItem* > vertices, int src, int dest )
{
    this->src = src;
    this->dest = dest;

    #ifdef LAYOUT__FF__WEIGHTED_ALIGNMENT

        QRect srcRect = vertices.at( src )->geometry();
        QRect destRect = vertices.at( dest )->geometry();

        int src_pos_x[2] = { srcRect.x(), srcRect.x() + srcRect.width() };
        int src_pos_y[2] = { srcRect.y(), srcRect.y() + srcRect.height() };
        int dest_pos_x[2] = { destRect.x(), destRect.x() + destRect.width() };
        int dest_pos_y[2] = { destRect.y(), destRect.y() + destRect.height() };

        // Assuming at least a 32-bit machine.
        #define bigweight  ( unsigned int ) 0xFFFFFFFF

        int x_total = 0, y_total = 0;

        for ( int i = 0; i < 4; i++ )
        {
            int offset;

            offset = dest_pos_x[ i % 2 ] - src_pos_x[ i / 2 ];
            if ( offset )
                weighted_offset[0][i] = bigweight / abs( offset );
            else
            {
                weighted_offset[0][i] = bigweight;
                x_total += bigweight; ..................................
            }
            x_total +=

            offset_weight[1][i] =
                ( offset[1][i] = dest_pos_y[ i % 2 ] - src_pos_y[ i / 2 ] )
                ? bigweight / abs( offset[1][i] )
                : bigweight ;
        }

        //...

    #else

        QRect srcRect = vertices.at( src )->geometry();
        QRect destRect = vertices.at( dest )->geometry();

        int src_pos[2] = { srcRect.x(),
                           srcRect.x() + srcRect.width() };
        int dest_pos[2] = { destRect.x(),
                        destRect.x() + destRect.width() };

        x_combo = 0;
        x_offset = abs( dest_pos[0] - src_pos[0] );

        for ( int i = 1; i < 4; i++ )
        {
            int offset = abs( dest_pos[ i % 2 ] - src_pos[ i / 2 ] );

            if ( offset < x_offset )
            {
                x_combo = i;
                x_offset = offset;
            }
        }

        x_offset = dest_pos[ x_combo % 2 ] - src_pos[ x_combo / 2 ];

        src_pos[0] = srcRect.y();
        src_pos[1] = srcRect.y() + srcRect.height();
        dest_pos[0] = destRect.y();
        dest_pos[1] = destRect.y() + destRect.height();

        y_combo = 0;
        y_offset = abs( dest_pos[0] - src_pos[0] );

        for ( int i = 1; i < 4; i++ )
        {
            int offset = abs( dest_pos[ i % 2 ] - src_pos[ i / 2 ] );

            if ( offset < y_offset )
            {
                y_combo = i;
                y_offset = offset;
            }
        }

        y_offset = dest_pos[ y_combo % 2 ] - src_pos[ y_combo / 2 ];

    #endif  // LAYOUT__FF__WEIGHTED_ALIGNMENT

}


void P2FreeFormLayoutEdge::setDestPosition( QList< QLayoutItem* > vertices ) const
{
    QRect srcRect = vertices.at( src )->geometry();
    QRect destRect = vertices.at( dest )->geometry();

//    cout << "void P2FreeFormLayoutEdge::setDestPosition("
//         << ( int ) src << ", " << ( int ) dest << ")" << endl; cout.flush();

    int x = srcRect.x() + x_offset;
    if ( x_combo / 2 )
        x += srcRect.width();
    if ( x_combo % 2 )
        x -= destRect.width();

    int y = srcRect.y() + y_offset;
    if ( y_combo / 2 )
        y += srcRect.height();
    if ( y_combo % 2 )
        y -= destRect.height();

    vertices.at( dest )->setGeometry( QRect( QPoint( x, y ), destRect.size() ) );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
