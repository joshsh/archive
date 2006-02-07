#ifndef P2BasicWidget_h
#define P2BasicWidget_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2Widget.h"
#include "P2Environment.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class P2BasicWidget : public P2Widget
{

public:

    bool isFrame();

public slots:

    void refresh( const P2Environment &env );

protected:

    /** Constructs an empty P2BasicWidget. */
    P2BasicWidget();

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2BasicWidget_h
