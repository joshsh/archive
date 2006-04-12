#ifndef P2Layout_h
#define P2Layout_h

///////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2Widget.h"
#include "P2Environment.h"

#include <QtGui>

///////////////////////////////////////////////////////////////////////////////

class P2Layout : public QLayout
{
    Q_OBJECT

public:

    P2Layout( QWidget *parent );

    ~P2Layout();

    int count() const;
    QLayoutItem *itemAt( int index ) const;
    QLayoutItem *takeAt( int index );

    /** \note  minimumSize() and sizeHint() are not distinguished for now. */
    QSize sizeHint() const;  // Must be const.
    QSize minimumSize() const;
    void setContentOffset( const QPoint &offset );
    void setMinimumSize( const QSize &size );

    void refreshChildren( const P2Environment &env );

    void setGeometry( const QRect &rect );

    virtual void adjustGeometry() = 0;

public slots:

    void childResizeEvent( QResizeEvent *event );

signals:

    void resized();

protected:

    void addItem( QLayoutItem *item );

    /** Create a QLayoutItem to hold a QWidget (a QWidgetItem) and add it. */
    void addWidget( P2Widget *widget );

    /** Show names and types of children.  A debugging method. */
    void showChildren() const;

    /** An auxiliary QList for quick iteration by index over this P2Layout's
        child widgets. */
    QList<QLayoutItem *> children;

    /** "Calling QLayoutItem::sizeHint(), etc. may be expensive, so you should
        store the value in a local variable if you need it again later in the
        same function." */
    QSize cachedSizeHint;

    QSize receivedMinimumSize;

    /** Bounding rectangle of layout contents. */
    QRect contentRectangle;

    /** Upper left hand corner of layout content. */
    QPoint contentOffset;

};

///////////////////////////////////////////////////////////////////////////////

#endif  // P2Layout_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
