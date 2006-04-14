#ifndef P2CharWidget_h
#define P2CharWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
}

#include <widgets/basic/P2Text.h>

////////////////////////////////////////////////////////////////////////////////

class P2CharWidget : public P2Text
{
public:

    P2CharWidget( Object *o );
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2CharWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
