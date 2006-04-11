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

    /** Create a QLayoutItem to hold a QWidget (a QWidgetItem) and add it. */
    void addWidget( P2Widget *widget, const QPoint &position );

    //int count() const;
    //QLayoutItem *itemAt( int index ) const;
    //QLayoutItem *takeAt( int index );

    /** \note  minimumSize() and sizeHint() are not distinguished for now. */
    QSize sizeHint() const;  // Must be const.
    QSize minimumSize() const;

    void setGeometry( const QRect &rect );
    bool hasHeightForWidth() const;
    Qt::Orientations expandingDirections() const;

    void setContentOffset( const QPoint &offset );

    void setMinimumSize( const QSize &size );

    void refreshChildren( const P2Environment &env );

    void adjustGeometry();

public slots:

    void childResizeEvent( QResizeEvent *event );

//signals:
//
//    void resized();

//protected:

    /** Show names and types of children.  A debugging method. */
    //void showChildren() const;

private:

    /** An auxiliary QList for quick iteration by index over this P2FreeFormLayout's
        child widgets. */
    //QList<QLayoutItem *> children;

    /** A weighted tree data structure which defines the ideal relative
        positions of this P2FreeFormLayout's child widgets. */
    P2FreeFormLayoutTree tree;

    /** Bounding rectangle of layout contents. */
    QRect contentRectangle;

    /** Upper left hand corner of layout content. */
    QPoint contentOffset;

    QSize receivedMinimumSize;

    /** "Calling QLayoutItem::sizeHint(), etc. may be expensive, so you should
        store the value in a local variable if you need it again later in the
        same function." */
    QSize cachedSizeHint;

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
