#ifndef P2FreeFormLayoutTree_h
#define P2FreeFormLayoutTree_h

////////////////////////////////////////////////////////////////////////////////

#include "../global.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

/** A graph edge which describes the relative position of neighboring layout
    items in terms of a minimum x and y offset.
    \note  In future, a weighted average of each of the two sets of four
    offset combos (as opposed to picking the "best one") might give smoother
    resizing behavior. */
class P2FreeFormLayoutEdge
{

public:

    P2FreeFormLayoutEdge( QList< QLayoutItem* > vertices, int src, int dest );

    void setDestPosition( QList< QLayoutItem* > vertices ) const;

    /** \return  the maximum x or y offset between two layout items.  This is
        used as an edge weight in generating the minimum spanning tree. */
    static int distance( QLayoutItem *src, QLayoutItem *dest );

private:

    /** Source vertex. */
    int src;

    /** Destination vertex. */
    int dest;

    /** x-offset (in pixels) from src to dest. */
    int x_offset;

    /** y-offset (in pixels) from src to dest. */
    int y_offset;

    /** A value from 0 to 4, indicating which side of the src and of the dest
        rectangle are to be off-set by x_offset. */
    int x_combo;

    /** A value from 0 to 4, indicating which side of the src and of the dest
        rectangle are to be off-set by y_offset. */
    int y_combo;
};

////////////////////////////////////////////////////////////////////////////////

    // + Eventually, the spanning tree should be separable
    // QList< QList<P2Edge> > spanningTree;

/** ...layout tree data structure...
    A P2FreeFormLayoutTree is separable from the P2Frame which contains it,
    so it is easily passed on from one frame to any number of derivative frames. */
class P2FreeFormLayoutTree : public QList< P2FreeFormLayoutEdge >
{

public:

    P2FreeFormLayoutTree();

    /** Copy constructor. */
    P2FreeFormLayoutTree( const P2FreeFormLayoutTree &other );

    /** Generates the layout tree via Prim's algorithm, where the distance()
        between two layout items is used as the edge weight.
        \warning  Assumes that none of the items intersect. */
    P2FreeFormLayoutTree( QList< QLayoutItem* > vertices );

    /** \return  the number of vertices in the tree */
    int size();

    /** Arranges the given layout items according to the offset information in
        the tree's edges.
        \note  The root vertex stays where it is.  All other vertices are
        arranged relative to its fixed position. */
    void applyTo( QList< QLayoutItem* > vertices );

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2FreeFormLayoutTree_h

