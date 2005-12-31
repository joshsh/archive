#ifndef P2Layout_h
#define P2Layout_h

///////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include <QtGui>

#include "P2BasicWidget.h"
#include "P2FreeFormLayoutTree.h"

///////////////////////////////////////////////////////////////////////////////

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

    /** An auxiliary QList for quick iteration by index over this P2Layout's
        child widgets. */
    QList<QLayoutItem *> children;

    /** A weighted tree data structure which defines the ideal relative
        positions of this P2Layout's child widgets. */
    P2FreeFormLayoutTree tree;

    QRect contentRectangle;

    // "Calling QLayoutItem::sizeHint(), etc. may be expensive, so you should
    // store the value in a local variable if you need it again later in the
    // same function."
    QSize cachedSizeHint;

    void refreshContentRectangle();
    void justifyContents();

    /** \todo  It would be nice if these were not necessary. */
    void expandChildrenByMargin();
    void shrinkChildrenByMargin();

    /** Resolves collisions by repeatedly shoving interfering widgets apart
        until they are all at a proper distance from one another.
        \return  the number of iterations it took to do this */
    int resolveCollisions();

    QPoint findBestPosition( const QRect &rect );

    void generateSpanningTree();
    void applySpanningTree();

};

///////////////////////////////////////////////////////////////////////////////

#endif  // P2Layout_h
