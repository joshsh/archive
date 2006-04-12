#ifndef P2BoxLayout_h
#define P2BoxLayout_h

///////////////////////////////////////////////////////////////////////////////

#include <QtGui>

#include "global.h"
#include "P2Layout.h"

///////////////////////////////////////////////////////////////////////////////

class P2BoxLayout : public P2Layout, virtual public QBoxLayout
{
    Q_OBJECT

public:

    P2BoxLayout( Direction dir, QWidget *parent );

    //void add( P2Widget *widget, const QPoint &position );

    bool hasHeightForWidth() const;
    Qt::Orientations expandingDirections() const;

    void adjustGeometry();

private:


};

///////////////////////////////////////////////////////////////////////////////

#endif  // P2BoxLayout_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
