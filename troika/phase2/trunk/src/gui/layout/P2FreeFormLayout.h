#ifndef P2FreeFormLayout_h
#define P2FreeFormLayout_h

///////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2Widget.h"
#include "P2Layout.h"
#include "P2FreeFormLayoutTree.h"
#include "P2Environment.h"

#include <QtGui>

///////////////////////////////////////////////////////////////////////////////

class P2FreeFormLayout : public P2Layout
{
    Q_OBJECT

public:

    P2FreeFormLayout( QWidget *parent );

    //~P2FreeFormLayout();

    /** Mandatory overloaded method from QLayout. */
    //void addItem( QLayoutItem *item );

    void add( P2Widget *widget, const QPoint &position );

    //int count() const;
    //QLayoutItem *itemAt( int index ) const;
    //QLayoutItem *takeAt( int index );

    bool hasHeightForWidth() const;
    Qt::Orientations expandingDirections() const;

    void adjustGeometry();

private:

    /** A weighted tree data structure which defines the ideal relative
        positions of this P2FreeFormLayout's child widgets. */
    P2FreeFormLayoutTree tree;

    void justifyContents();

    /** \return  whether two layout rectangles are within spacing() pixels of
        one another */
    bool tooClose( const QRect &a, const QRect &b );

    /** Resolves collisions by repeatedly shoving interfering widgets apart
        until they are all at a proper distance from one another.
        \return  the number of iterations it took to do this */
    int resolveCollisions();

    /** \return  the closest non-interfering position of an item to be placed
        into the layout tree (does not affect existing layout items) */
    QPoint findBestPosition( const QRect &rect );

    void generateSpanningTree();
    void applySpanningTree();

};

///////////////////////////////////////////////////////////////////////////////

#endif  // P2FreeFormLayout_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
