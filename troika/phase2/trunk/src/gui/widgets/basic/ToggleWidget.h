#ifndef ToggleWidget_h
#define ToggleWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <widgets/P2BasicWidget.h>

////////////////////////////////////////////////////////////////////////////////

class ToggleWidget : public P2BasicWidget
{

public:

    ToggleWidget();

    QSize sizeHint() const;

protected:

    void mousePressEvent( QMouseEvent *event );

    void paintEvent( QPaintEvent *event );

private:

    QList<QImage> *imageList;

    int toggle;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // ToggleWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
