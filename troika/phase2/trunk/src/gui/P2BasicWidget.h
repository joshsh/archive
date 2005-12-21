#ifndef P2BasicWidget_h
#define P2BasicWidget_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class P2BasicWidget : public QWidget
{

public:

    /** A value which determines whether this is a top-level P2BasicWidget, in
        terms of input event handling.  If true, it is assumed that the widget's
        parent is also a P2BasicWidget, and input events are handled solely at
        the discretion of the parent. */
    bool isDependent;

    /** A convenience method. */
    void setCenter( const QPoint &p );

    /** A convenience method. */
    void setPosition( const QPoint &p );

    void setSize( const QSize &s );

    virtual bool isFrame() = 0;

    enum EventOrigin { SELF, CHILD_FRAME, CHILD_NOFRAME };

    virtual bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin ) = 0;
    virtual bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) = 0;

protected:

    /** Constructs an empty P2BasicWidget. */
    P2BasicWidget();

    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );

private:

    EventOrigin originIfFrame()
    {
        if ( isFrame() )
            return CHILD_FRAME;
        else
            return CHILD_NOFRAME;
    }

    bool mousePressEventWrapper( QMouseEvent *event, EventOrigin origin );
    bool mouseMoveEventWrapper( QMouseEvent *event, EventOrigin origin );
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2BasicWidget_h
