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
    void setPosition( QPoint p );

    virtual bool isBinder() = 0;

    virtual bool handleMousePressEvent( QMouseEvent *event, bool childIsBinder ) = 0;

protected:

    /** Constructs an empty P2BasicWidget. */
    P2BasicWidget();

    void mousePressEvent( QMouseEvent *event );

    bool mousePressEventWrapper( QMouseEvent *event, bool childIsBinder );

private:

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2BasicWidget_h
