#ifndef P2CentralWidget_h
#define P2CentralWidget_h 1

////////////////////////////////////////////////////////////////////////////////

#include "compile-target.h"  // Macros specific to the target architecture.

#include <qwidget.h>  // QWidget
//Added by qt3to4:
#include <QPaintEvent>

////////////////////////////////////////////////////////////////////////////////

class P2CentralWidget : public QWidget
{
public:

    P2CentralWidget( QWidget *parent=0, const char *name=0 );

protected:

    void paintEvent( QPaintEvent * );
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2CentralWidget_h
