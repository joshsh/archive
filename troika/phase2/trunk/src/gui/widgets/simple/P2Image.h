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

    const QString className();

protected:

    bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin ) { return false; }
    bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) { return false; }

    void paintEvent( QPaintEvent *event );

private:

    QImage image;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Image_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
