#ifndef P2CentralWidget_h
#define P2CentralWidget_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2BasicWidget.h"
#include "P2Frame.h"

////////////////////////////////////////////////////////////////////////////////

class P2CentralWidget : public P2Widget
{

public:

    P2CentralWidget( const QSize &fixedSize );

    bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin ) { return false; }
    bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) { return false; }

    const QString className();
    bool isFrame() { return false; }
    void refresh();

    QSize sizeHint() const { return centralFrame->sizeHint() + QSize( 4, 4 ); }

    void resizeEvent();

private:

    P2Frame *centralFrame;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2CentralWidget_h
