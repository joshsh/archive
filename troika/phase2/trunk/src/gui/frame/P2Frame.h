#ifndef P2Frame_h
#define P2Frame_h

////////////////////////////////////////////////////////////////////////////////

#include "P2Widget.h"

////////////////////////////////////////////////////////////////////////////////

/** \note  P2Frame is unrelated to QFrame. */
class P2Frame : public P2Widget
{
    Q_OBJECT

public:

    /** Constructs an empty P2Frame. */
    P2Frame( P2Widget *widget, const P2Environment &env );

    //- "Reimplement QWidget::resizeEvent() to calculate the required
    // distribution of sizes and call setGeometry() on each child."

    //- "The widget will get an event of type QEvent::LayoutHint when the layout
    // needs to be recalculated. Reimplement QWidget::event() to handle
    // QEvent::LayoutHint events."

    bool isFrame() { return true; }

    P2Frame *focusFrame();

    //~ Only public for the sake of P2CentralWidget...
    void setFocus( P2Frame *child );

public slots:

    void refresh( const P2Environment &env );
    void update();
    void childResizeEvent( QResizeEvent *event );

protected:

    void paintEvent( QPaintEvent *event );

    bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin );
    bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin );

    void showInfo();

    P2Frame *focusChild;
    void unfocus();

private:

    P2Widget *contentWidget;

    const P2Environment *environment;

    void setContentOffset( const QPoint &offset );
    void setMinimumSize( const QSize &size );

    QSize cachedSizeHint;

    QSize receivedMinimumSize;

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Frame_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
