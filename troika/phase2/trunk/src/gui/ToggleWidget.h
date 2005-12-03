#ifndef ToggleWidget_h
#define ToggleWidget_h

////////////////////////////////////////////////////////////////////////////////

//#include "compile-target.h"  // Macros specific to the target architecture.

#include <qwidget.h>  // QWidget
//Added by qt3to4:
#include <QMouseEvent>
#include <QPaintEvent>

////////////////////////////////////////////////////////////////////////////////

class ToggleWidget : public QWidget
{
    Q_OBJECT

public:

    // ToggleWidget( ToggleWidget *parentBinder, const char *name = 0 );
    ToggleWidget( QWidget* parent );

protected:

    void mousePressEvent( QMouseEvent *event );
    void paintEvent( QPaintEvent *event );

    //void resizePixmap( QSize newSize );

private:

    //QSize size;  // Size of the pixmap, plus border padding.
    //QPixmap pixmap;

    int toggle;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // ToggleWidget_h

