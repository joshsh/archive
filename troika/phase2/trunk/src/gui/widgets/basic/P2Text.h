#ifndef P2Text_h
#define P2Text_h

////////////////////////////////////////////////////////////////////////////////

#include <widgets/P2BasicWidget.h>

////////////////////////////////////////////////////////////////////////////////

/** A display widget for short plain-text strings. */
class P2Text : public P2BasicWidget
{

public:

    /** \param text  character string content
        \param color  text color (defaults to the foreground color) */
    P2Text( QString text, const QColor &color );

    const QString className();

    QSize sizeHint() const;  // Must be const.

protected:

    bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin ) { return false; }
    bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) { return false; }

    void paintEvent( QPaintEvent *event );

private:

    QString text;
    QSize cachedSizeHint;
    QColor color;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Text_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
