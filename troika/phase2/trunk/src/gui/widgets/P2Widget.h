#ifndef P2Widget_h
#define P2Widget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

#include <P2Environment.h>

////////////////////////////////////////////////////////////////////////////////

class P2Widget : public QWidget
{
    Q_OBJECT

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

    virtual bool isFrame() = 0;

    enum EventOrigin { SELF, CHILD_FRAME, CHILD_NOFRAME };

    virtual bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin ) = 0;
    virtual bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) = 0;

    /** Overloaded method which additionally emits the 'renamed' signal. */
    void setObjectName( QString s );

public slots:

    virtual void refresh( const P2Environment &env ) = 0;

signals:

    void resized( QResizeEvent *event );
    void renamed( QString name );

    void mousePress( QMouseEvent *event, P2Widget *origin );
    //virtual void handleMousePress( QMouseEvent *event ) = 0;

protected:

    /** Constructs an empty P2Widget. */
    P2Widget();

    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );

    void resizeEvent( QResizeEvent *event );

    QPoint dragStartPosition;

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

#endif  // P2Widget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
