#ifndef ResizeWidget_h
#define ResizeWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <widgets/P2BasicWidget.h>

////////////////////////////////////////////////////////////////////////////////

class ResizeWidget : public P2BasicWidget
{

public:

    ResizeWidget();

    const QString className();

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

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
