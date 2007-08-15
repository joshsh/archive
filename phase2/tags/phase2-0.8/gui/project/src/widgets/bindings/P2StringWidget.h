#ifndef P2StringWidget_h
#define P2StringWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
}

//#include <widgets/basic/P2Text.h>
#include <widgets/basic/P2TextEdit.h>

////////////////////////////////////////////////////////////////////////////////

//class P2StringWidget : public P2Text
class P2StringWidget : public P2TextEdit
{
public:

    P2StringWidget( const Object *o );
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2StringWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
