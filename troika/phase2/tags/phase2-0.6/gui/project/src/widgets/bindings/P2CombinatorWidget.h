#ifndef P2CombinatorWidget_h
#define P2CombinatorWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
}

#include <widgets/basic/P2Text.h>

////////////////////////////////////////////////////////////////////////////////

class P2CombinatorWidget : public P2Text
{
public:

    P2CombinatorWidget( const Object *o );
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2CombinatorWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
