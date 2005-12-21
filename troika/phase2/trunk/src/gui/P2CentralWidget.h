#ifndef P2CentralWidget_h
#define P2CentralWidget_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2BasicWidget.h"

////////////////////////////////////////////////////////////////////////////////

class P2CentralWidget : public P2BasicWidget
{

public:

    P2CentralWidget( int fixedWidth );

    bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin ) { return false; }
    bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) { return false; }

    bool isFrame() { return false; }

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2CentralWidget_h
