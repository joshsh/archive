#ifndef P2NamespaceWidget_h
#define P2NamespaceWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
#include <Namespace.h>
}

#include "../new/P2BoxArray.h"
#include "P2EnvironmentBinder.h"

////////////////////////////////////////////////////////////////////////////////

class P2NamespaceWidget : public P2BoxArray
{
public:

    P2NamespaceWidget( Object *o, P2EnvironmentBinder *eb );

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2NamespaceWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
