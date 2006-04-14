#ifndef ToggleWidget_h
#define ToggleWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <widgets/P2BasicWidget.h>

////////////////////////////////////////////////////////////////////////////////

class ToggleWidget : public P2BasicWidget
{

public:

    ToggleWidget();

    const QString className();

    QSize sizeHint() const;

protected:

    bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin );
    bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) { return false; }

    void paintEvent( QPaintEvent *event );

    //void resizePixmap( QSize newSize );

private:

    QList<QImage> *imageList;

    int toggle;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // ToggleWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
