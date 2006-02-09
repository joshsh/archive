#ifndef P2ScrollArea_h
#define P2ScrollArea_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2Widget.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class P2ScrollArea : public QScrollArea
{
    Q_OBJECT

public:

    P2ScrollArea( P2Widget *widget );

private slots:

    void childResizeEvent( QResizeEvent *event );

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2ScrollArea_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
