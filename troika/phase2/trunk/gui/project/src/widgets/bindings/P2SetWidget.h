#ifndef P2SetWidget_h
#define P2SetWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <phase2/Object.h>
}

#include <widgets/groups/P2BoxArray.h>
#include <P2Binder.h>

////////////////////////////////////////////////////////////////////////////////

class P2SetWidget : public P2BoxArray
{
public:

    P2SetWidget( const Object *o, P2Binder *eb );

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2SetWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
