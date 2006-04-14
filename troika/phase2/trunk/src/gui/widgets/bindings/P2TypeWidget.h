#ifndef P2TypeWidget_h
#define P2TypeWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
}

#include "../binders/P2Text.h"

////////////////////////////////////////////////////////////////////////////////

class P2TypeWidget : public P2Text
{
public:

    P2TypeWidget( Object *o );
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2TypeWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
