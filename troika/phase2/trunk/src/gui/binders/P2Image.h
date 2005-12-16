#ifndef P2Image_h
#define P2Image_h

////////////////////////////////////////////////////////////////////////////////

#include "../global.h"
#include "../P2BasicWidget.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

/** A wrapper for QImage. */
class P2Image : public P2BasicWidget
{

public:

    P2Image( const char * const xpm[] );

    bool isFrame() { return false; }

protected:

    bool handleMousePressEvent( QMouseEvent *event, bool childIsBinder );
    void paintEvent( QPaintEvent *event );

private:

    QImage image;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Image_h

