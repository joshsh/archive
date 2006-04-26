#ifndef P2Binder_h
#define P2Binder_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Environment.h>
}

#include <widgets/P2Widget.h>

class P2Binder;
typedef P2Widget *(*objectWidgetConstructor)( Object *o, P2Binder *eb );

////////////////////////////////////////////////////////////////////////////////

class P2Binder
{

public:

    P2Binder( P2Environment &e );

    P2Widget *objectWidget( Object *o );
    QColor objectColor( Object *o );

    P2Environment *getEnv();

private:

    P2Environment *env;
    Environment *environment;

    QHash<int, objectWidgetConstructor> constructors;

    QHash<int, QColor> colors;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Binder_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
