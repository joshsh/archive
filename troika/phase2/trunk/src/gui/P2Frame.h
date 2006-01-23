#ifndef P2Frame_h
#define P2Frame_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2Widget.h"

////////////////////////////////////////////////////////////////////////////////

/** \note  P2Frame is unrelated to QFrame. */
class P2Frame : public P2Widget
{

public:

    /** Constructs an empty P2Frame. */
    P2Frame();

    /** Adds a widget to this P2Frame's P2Layout, which assumes ownership of it.
        \param widget  the child P2Widget to add
        \param position  the intended top left-hand corner of the child's
        geometry(), before collision resolution */
    void addChild( P2Widget *widget, const QPoint &position );

    //- "Reimplement QWidget::resizeEvent() to calculate the required
    // distribution of sizes and call setGeometry() on each child."

    //- "The widget will get an event of type QEvent::LayoutHint when the layout
    // needs to be recalculated. Reimplement QWidget::event() to handle
    // QEvent::LayoutHint events."

    const QString className();
    bool isFrame() { return true; }
    void refresh();

protected:

    void paintEvent( QPaintEvent *event );

    /** A P2Frame may accept drop events... I think. */
    bool acceptDrops () const;

    bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin );
    bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin );

private:

    void showInfo();

    //!
    //void resize( QSize &newSize );

    P2Frame *focusChild;
    void setFocus( P2Frame *child );
    void unfocus();
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Frame_h
