#ifndef P2Binder_h
#define P2Binder_h

////////////////////////////////////////////////////////////////////////////////

#include "compile-target.h"  // Macros specific to the target architecture.

#include <QWidget>  // QWidget
#include <QPaintEvent>  // QPaintEvent

////////////////////////////////////////////////////////////////////////////////

class P2Binder : public QWidget
{
    // Necessary?
    Q_OBJECT

public:

    /** Constructs an empty P2Binder. */
    P2Binder( QWidget* parent );

    /** Adds a QWidget to this P2Binder's P2Layout, which assumes ownership of
        the QWidget and orders it into its P2LayoutItem tree. */
    void addWidget( QWidget *widget );

protected:

    void paintEvent( QPaintEvent *event );

    /** A P2Binder may accept drop events... I think. */
    bool acceptDrops () const;

private:

    /** Common routines for the constructors. */
    void initialize();

    void resize( QSize newSize );
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Binder_h
