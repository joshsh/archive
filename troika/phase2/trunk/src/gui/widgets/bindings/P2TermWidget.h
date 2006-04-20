#ifndef P2TermWidget_h
#define P2TermWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
}

#include <widgets/basic/P2Text.h>

////////////////////////////////////////////////////////////////////////////////

class P2TermWidget : public P2Text
{
public:

    P2TermWidget( Object *o );
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2TermWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
