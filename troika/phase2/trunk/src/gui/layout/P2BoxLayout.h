#ifndef P2BoxLayout_h
#define P2BoxLayout_h

///////////////////////////////////////////////////////////////////////////////

#include <QtGui>

#include "global.h"
#include "P2Layout.h"

///////////////////////////////////////////////////////////////////////////////

// Note: P2Layout is deliberately inherited before QBoxLayout.
class P2BoxLayout : public QBoxLayout, public P2Layout
{
    Q_OBJECT

public:

    P2BoxLayout( QBoxLayout::Direction dir, QWidget *parent );

    bool hasHeightForWidth() const;
    Qt::Orientations expandingDirections() const;

    void setContentOffset( const QPoint &offset );
    void setMinimumSize( const QSize &size );

//    void adjustGeometry();

    void add( P2Widget *widget );

signals:

    void resized();

public slots:

    void childResizeEvent( QResizeEvent *event );

};

///////////////////////////////////////////////////////////////////////////////

#endif  // P2BoxLayout_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
