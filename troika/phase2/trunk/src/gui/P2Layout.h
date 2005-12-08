#ifndef P2Layout_h
#define P2Layout_h 1

///////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include <QtGui>

///////////////////////////////////////////////////////////////////////////////

// You can also test to see if two QRects intersect with intersects(), or retrieve the intersection as a QRect using intersect():

class P2Layout : public QLayout
{

public:

    P2Layout( QWidget *parent );

    ~P2Layout();

    /** Mandatory overloaded method from QLayout. */
    void addItem( QLayoutItem *item );

    /** Create a QLayoutItem to hold a QWidget (a QWidgetItem) and add it. */
    void addWidget( QWidget *widget, QPoint position );

    int count() const;
    QLayoutItem *itemAt( int index ) const;
    QLayoutItem *takeAt( int index );

    /** \note  minimumSize() and sizeHint() are not distinguished for now. */
    QSize sizeHint() const;  // Must be const.
    QSize minimumSize() const;
    void setGeometry( const QRect &rect );
    bool hasHeightForWidth() const;
    Qt::Orientations expandingDirections() const;

private:

    /** A wrapper for QLayoutItem which holds some additional (graph)
        information needed by the P2Layout. */
    struct P2LayoutItem
    {
        P2LayoutItem( QLayoutItem *i )
        {
            item = i;
        }

        QLayoutItem *item;
        //QList<P2LayoutEdge *> outboundEdges;
    };

    /** A graph edge which describes the relative position of a neighboring
        vertex in terms of direction and distance. */
    struct P2LayoutEdge
    {
        P2LayoutEdge( P2LayoutItem *i, int o, double a )
        {
            targetItem = i;
            offset = o;
            angle = a;
        }

        /** The other vertex of the edge. */
        P2LayoutItem *targetItem;

        /** Distance (in pixels) to targetItem. */
        int offset;

        /** Angle (in radians) to targetItem. */
        double angle;
    };

    /** The root of a weighted tree data structure which defines the ideal
        relative positions of this P2Layout's child widgets. */
    //P2LayoutItem *root;

    /** An auxiliary QList for quick iteration by index over this P2Layout's
        child widgets. */
    QList<P2LayoutItem *> children;

    QRect contentRectangle;

    void justifyContents();

    // "Calling QLayoutItem::sizeHint(), etc. may be expensive, so you should
    // store the value in a local variable if you need it again later in the
    // same function."
    QSize size;

};

///////////////////////////////////////////////////////////////////////////////

#endif  // P2Layout_h
