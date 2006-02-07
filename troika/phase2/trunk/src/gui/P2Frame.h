#ifndef P2Frame_h
#define P2Frame_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2Widget.h"
#include "P2Environment.h"

////////////////////////////////////////////////////////////////////////////////

/** \note  P2Frame is unrelated to QFrame. */
class P2Frame : public P2Widget
{
    Q_OBJECT

public:

    /** Constructs an empty P2Frame. */
    P2Frame( const P2Environment &env );

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

    P2Frame *focusFrame();

    //~ Only public for the sake of P2CentralWidget...
    void setFocus( P2Frame *child );

public slots:

    void refresh( const P2Environment &env );

protected:

    void paintEvent( QPaintEvent *event );

    /** A P2Frame may accept drop events... I think. */
    bool acceptDrops () const;

    void dragEnterEvent( QDragEnterEvent *event );
    void dropEvent( QDropEvent *event );

    bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin );
    bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin );

private:

    void showInfo();

    P2Frame *focusChild;
    void unfocus();

    const P2Environment *environment;

private slots:

    void layoutResizedEvent();

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Frame_h
