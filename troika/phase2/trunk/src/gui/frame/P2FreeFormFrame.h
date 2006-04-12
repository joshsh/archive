#ifndef P2FreeFormFrame_h
#define P2FreeFormFrame_h

#include "P2Frame.h"

////////////////////////////////////////////////////////////////////////////////

class P2FreeFormFrame : public P2Frame
{
    Q_OBJECT

public:

    /** Constructs an empty P2FreeFormFrame. */
    P2FreeFormFrame( const P2Environment &env );

    /** Adds a widget to this P2FreeFormFrame's P2FreeFormLayout, which assumes ownership of it.
        \param widget  the child P2Widget to add
        \param position  the intended top left-hand corner of the child's
        geometry(), before collision resolution */
    void addChild( P2Widget *widget, const QPoint &position );

    const QString className();

protected:

    /** A P2FreeFormFrame may accept drop events... I think. */
    bool acceptDrops () const;

    void dragEnterEvent( QDragEnterEvent *event );
    void dropEvent( QDropEvent *event );

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2FreeFormFrame_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
