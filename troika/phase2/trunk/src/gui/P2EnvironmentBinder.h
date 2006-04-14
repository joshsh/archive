#ifndef P2EnvironmentBinder_h
#define P2EnvironmentBinder_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Environment.h>
}

#include <widgets/P2Widget.h>

class P2EnvironmentBinder;
typedef P2Widget *(*objectWidgetConstructor)( Object *o, P2EnvironmentBinder *eb );

////////////////////////////////////////////////////////////////////////////////

class P2EnvironmentBinder
{

public:

    P2EnvironmentBinder( const P2Environment &e );

    P2Widget *objectWidget( Object *o );

    const P2Environment *getEnv();

private:

    const P2Environment *env;
    Environment *environment;

    QHash<int, objectWidgetConstructor> constructors;

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2EnvironmentBinder_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
