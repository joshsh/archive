#ifndef P2Text_h
#define P2Text_h

////////////////////////////////////////////////////////////////////////////////

#include "../global.h"
#include "../P2BasicWidget.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

/** */
class P2Text : public P2BasicWidget
{

public:

    P2Text( char *text );

    bool isFrame() { return false; }

    QSize sizeHint() const;  // Must be const.

protected:

    bool handleMousePressEvent( QMouseEvent *event, bool childIsBinder );
    void paintEvent( QPaintEvent *event );

private:

    char *text;
    QSize size;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Text_h

