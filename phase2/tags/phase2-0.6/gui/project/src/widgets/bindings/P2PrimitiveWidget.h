#ifndef P2PrimitiveWidget_h
#define P2PrimitiveWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
}

#include <widgets/basic/P2Text.h>

////////////////////////////////////////////////////////////////////////////////

class P2PrimitiveWidget : public P2Text
{
public:

    P2PrimitiveWidget( const Object *o );
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2PrimitiveWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
