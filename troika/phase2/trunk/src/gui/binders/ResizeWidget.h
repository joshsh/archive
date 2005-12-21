#ifndef ResizeWidget_h
#define ResizeWidget_h

////////////////////////////////////////////////////////////////////////////////

#include "../global.h"
#include "../P2BasicWidget.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class ResizeWidget : public P2BasicWidget
{

public:

    ResizeWidget();

    bool isFrame() { return false; }

    QSize sizeHint() const;

protected:

    bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin );
    bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) { return false; }

    void paintEvent( QPaintEvent *event );

private:

    int toggle;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // ResizeWidget_h

