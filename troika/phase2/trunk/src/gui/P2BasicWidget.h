#ifndef P2BasicWidget_h
#define P2BasicWidget_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2Environment.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class P2BasicWidget : public QWidget
{

public:

    bool isDependent;

    /** Shortcut to the global environment. */
    P2Environment *environment;

    /** A convenience method. */
    void setPosition( QPoint p );

    virtual bool isBinder() = 0;

    void setEnvironment( P2Environment *e );

    virtual bool handleMousePressEvent( QMouseEvent *event, bool childIsBinder ) = 0;

protected:

    /** Constructs an empty P2BasicWidget. */
    P2BasicWidget();

    /** Constructs an empty P2BasicWidget. */
    //P2BasicWidget( QWidget* parent, P2Environment *e );

    void mousePressEvent( QMouseEvent *event );

    bool mousePressEventWrapper( QMouseEvent *event, bool childIsBinder );

private:

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2BasicWidget_h
