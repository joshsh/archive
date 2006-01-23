#ifndef P2Text_h
#define P2Text_h

////////////////////////////////////////////////////////////////////////////////

#include "../global.h"
#include "../P2BasicWidget.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

/** A display widget for short plain-text strings. */
class P2Text : public P2BasicWidget
{

public:

    /** \param text  character string content
        \param color  text color (defaults to the foreground color) */
    P2Text( char *text, QColor *color );

    const QString className();

    QSize sizeHint() const;  // Must be const.

protected:

    bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin ) { return false; }
    bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) { return false; }

    void paintEvent( QPaintEvent *event );

private:

    char *text;
    QSize size;
    QColor color;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Text_h

