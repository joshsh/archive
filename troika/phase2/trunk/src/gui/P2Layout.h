#ifndef P2Layout_h
#define P2Layout_h 1

///////////////////////////////////////////////////////////////////////////////

#include <QWidget>  // QWidget
#include <QLayout>  // QLayout
#include <QRect>  // QRect
#include <QWidgetItem>  // QWidgetItem

///////////////////////////////////////////////////////////////////////////////

class P2Layout : public QLayout
{
public:

    P2Layout ( QWidget *parent );

    ~P2Layout();

    void addItem( QLayoutItem *item );
    void addWidget( QWidget *widget );

    //QLayoutIterator iterator();
    int count() const;
    QLayoutItem *itemAt( int index ) const;
    QLayoutItem *takeAt( int index );

    QSize sizeHint() const;  // Must be const.
    void setGeometry( const QRect &rect );
    QSize minimumSize() const;
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

        QSize size()
        {
            return item->geometry().size() + QSize( 3, 3 );
        }
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

    //QRect boundingRectangle;
    QSize size;
    void findBoundingRectangle();// const;
    QSize calculateSize() const;
};

///////////////////////////////////////////////////////////////////////////////

#endif  // P2Layout_h
