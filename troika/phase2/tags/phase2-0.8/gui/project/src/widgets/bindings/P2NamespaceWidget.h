#ifndef P2NamespaceWidget_h
#define P2NamespaceWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
}

#include <widgets/groups/P2BoxArray.h>
#include <P2Binder.h>

////////////////////////////////////////////////////////////////////////////////

class P2NamespaceWidget : public P2BoxArray
{
public:

    P2NamespaceWidget( const Object *o, P2Binder *eb );

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2NamespaceWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
