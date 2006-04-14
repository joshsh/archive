#ifndef P2ObjectWidget_h
#define P2ObjectWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
}

#include "../global.h"
#include "../P2Widget.h"
#include "../P2Environment.h"


////////////////////////////////////////////////////////////////////////////////

class P2ObjectWidget : public P2Widget
{

public:

    bool isFrame();

public slots:

    void refresh( const P2Environment &env );

protected:

    /** Constructs an empty P2ObjectWidget. */
    P2ObjectWidget();

    Object *object;
    P2Widget *widget;

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2ObjectWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
