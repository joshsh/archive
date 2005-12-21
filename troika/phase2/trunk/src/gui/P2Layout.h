#ifndef P2Layout_h
#define P2Layout_h

///////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include <QtGui>

#include "P2BasicWidget.h"

///////////////////////////////////////////////////////////////////////////////

/** A QLayoutItem which holds some additional (graph) information needed
    by the P2Layout. */
class P2LayoutItem : public QLayoutItem
{

public:

private:

    //QList<P2LayoutEdge *> outboundEdges;

};

/** A graph edge which describes the relative position of a neighboring
    vertex in terms of a minimum x and y offset.
    \note  In future, a weighted average of each of the two sets of four
    offset combos (as opposed to picking the "best one") might give smoother
    resizing behavior. */
class P2LayoutEdge
{

public:

    /** \warning  Assumes that the two P2LayoutItems do not intersect. */
    P2LayoutEdge( P2LayoutItem *src, P2LayoutItem *dest );

    int compareTo( const P2LayoutEdge &otherEdge );

    void setDestPosition( P2LayoutItem *src, P2LayoutItem *dest );

private:

    /** x-offset (in pixels) to targetItem. */
    int x_offset;

    /** y-offset (in pixels) to targetItem. */
    int y_offset;

    /** A value from 0 to 4, indicating which side of the src and of the dest
        rectangle are to be off-set by x_offset. */
    int x_combo;

    /** A value from 0 to 4, indicating which side of the src and of the dest
        rectangle are to be off-set by y_offset. */
    int y_combo;
};

////////////////////////////////////////////////////////////////////////////////

class P2Layout : public QLayout
{

public:

    P2Layout( QWidget *parent );

    ~P2Layout();

    /** Mandatory overloaded method from QLayout. */
    void addItem( QLayoutItem *item );

    /** Create a QLayoutItem to hold a QWidget (a QWidgetItem) and add it. */
    void addWidget( P2BasicWidget *widget, const QPoint &position );

    int count() const;
    QLayoutItem *itemAt( int index ) const;
    QLayoutItem *takeAt( int index );

    /** \note  minimumSize() and sizeHint() are not distinguished for now. */
    QSize sizeHint() const;  // Must be const.
    QSize minimumSize() const;

    void setGeometry( const QRect &rect );
    bool hasHeightForWidth() const;
    Qt::Orientations expandingDirections() const;

    void adjustGeometry();

private:

    /** The root of a weighted tree data structure which defines the ideal
        relative positions of this P2Layout's child widgets. */
    //P2LayoutItem *root;

    /** An auxiliary QList for quick iteration by index over this P2Layout's
        child widgets. */
    QList<P2LayoutItem *> children;

    QRect contentRectangle;

    // "Calling QLayoutItem::sizeHint(), etc. may be expensive, so you should
    // store the value in a local variable if you need it again later in the
    // same function."
    QSize cachedSizeHint;

    void refreshContentRectangle();
    void justifyContents();

    /** Resolves collisions by repeatedly shoving interfering widgets apart
        until they are all at a proper distance from one another.
        \return  the number of iterations it took to do this */
    int resolveCollisions();

};

///////////////////////////////////////////////////////////////////////////////

#endif  // P2Layout_h
