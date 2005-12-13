#ifndef ToggleWidget_h
#define ToggleWidget_h

////////////////////////////////////////////////////////////////////////////////

#include "../global.h"
#include "../P2BasicWidget.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class ToggleWidget : public P2BasicWidget
{

public:

    ToggleWidget();

    bool isFrame() { return false; }

protected:

    bool handleMousePressEvent( QMouseEvent *event, bool childIsBinder );
    void paintEvent( QPaintEvent *event );

    //void resizePixmap( QSize newSize );

private:

    QList<QImage> *imageList;

    int toggle;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // ToggleWidget_h

