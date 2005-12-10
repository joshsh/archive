#ifndef P2Binder_h
#define P2Binder_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2Environment.h"
#include "P2BasicWidget.h"

////////////////////////////////////////////////////////////////////////////////

class P2Binder : public P2BasicWidget
{

public:

    /** Constructs an empty P2Binder. */
    P2Binder();

    /** Constructs an empty P2Binder. */
    //P2Binder( QWidget* parent, P2Environment *e );

    /** Adds a QWidget to this P2Binder's P2Layout, which assumes ownership of
        the QWidget and orders it into its P2LayoutItem tree. */
    void addDependentWidget( P2BasicWidget *widget, QPoint position );

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    QSizePolicy sizePolicy() const;

    //- "Reimplement QWidget::resizeEvent() to calculate the required
    // distribution of sizes and call setGeometry() on each child."

    //- "The widget will get an event of type QEvent::LayoutHint when the layout
    // needs to be recalculated. Reimplement QWidget::event() to handle
    // QEvent::LayoutHint events."

    bool isBinder() { return true; }

protected:

    //void mousePressEvent( QMouseEvent *event );
    void paintEvent( QPaintEvent *event );

    /** A P2Binder may accept drop events... I think. */
    bool acceptDrops () const;

    bool handleMousePressEvent( QMouseEvent *event, bool childIsBinder );

private:

    /** A common routine for the constructors. */
    void initialize();

    void showInfo();

    //!
    void resize( QSize newSize );

    P2Binder *focusChild;
    void setFocus( P2Binder *child );
    void unfocus();
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Binder_h
