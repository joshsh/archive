#ifndef P2Binder_h
#define P2Binder_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <phase2/Environment.h>
}

#include <widgets/P2Widget.h>
//#include <widgets/P2View.h>

//class P2View;
class P2Binder;
typedef P2Widget *(*objectWidgetConstructor)( const Object *o, P2Binder *eb );

////////////////////////////////////////////////////////////////////////////////

class P2Binder : public QObject
{
    Q_OBJECT

public:

    P2Binder( P2Environment *e );

    P2Widget *objectWidget( const Object *o );
    QColor objectColor( const Object *o );
    QString objectName( const Object *o );
    QString objectFullName( const Object *o );
    P2Widget *objectNameWidget( const Object *o );

    P2Environment *getEnv();

    P2Widget *getFocusWidget() const;
    void setFocusWidget( P2Widget *w );
    void clearFocusWidget( P2Widget *w );

    void requestObjectView( const Object *o );

signals:

    void hasFocusWidget( bool b );

    void objectViewRequest( const Object *o );

private:

    P2Widget *focusWidget;

    P2Environment *env;
    Environment *environment;

    QHash<int, objectWidgetConstructor> constructors;

    QHash<int, QColor> colors;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Binder_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
