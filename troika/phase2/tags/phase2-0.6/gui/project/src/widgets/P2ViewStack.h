#ifndef P2ViewStack_h
#define P2ViewStack_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
}

#include <P2Environment.h>
#include <widgets/P2View.h>

////////////////////////////////////////////////////////////////////////////////

class P2ViewStack : public QStackedWidget
{
    Q_OBJECT

public:

    P2ViewStack( P2Environment *env, QWidget *parent );

    P2Widget *focusWidget();
    const Object *focusObject();

signals:

    void backEnabled();
    void backDisabled();
    void forwardEnabled();
    void forwardDisabled();

    void hasFocusObject( bool b );

public slots:

    void goBack();
    void goForward();

    void addView( const Object *o );

    void refresh( const P2Environment &env );
    void update();

private:

    P2Environment *environment;

    void remove( int index );
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2ViewStack_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
