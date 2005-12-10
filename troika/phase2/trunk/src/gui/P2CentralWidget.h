#ifndef P2CentralWidget_h
#define P2CentralWidget_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2BasicWidget.h"
#include "P2Environment.h"

////////////////////////////////////////////////////////////////////////////////

class P2CentralWidget : public P2BasicWidget
{

public:

    P2CentralWidget( P2Environment *e );
    //P2CentralWidget( QWidget *parent, P2Environment *e );

    bool handleMousePressEvent( QMouseEvent *event, bool childIsBinder );

    bool isBinder() { return false; }

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2CentralWidget_h
