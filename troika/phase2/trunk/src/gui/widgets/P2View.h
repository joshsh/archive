#ifndef P2View_h
#define P2View_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

#include <P2Binder.h>
#include <widgets/P2Widget.h>
#include <widgets/bindings/P2ObjectFrame.h>

extern "C"
{
#include <Object.h>
}

////////////////////////////////////////////////////////////////////////////////

class P2View : public P2Widget
{
    Q_OBJECT

public:

    P2View( Object *o, P2Environment *env );

    P2Widget *focusWidget();

    //QSize sizeHint() const;

    const P2Binder *binder() const;

signals:

    void objectViewRequest( Object *o );

public slots:

    void refresh( const P2Environment &env );

    void update( QResizeEvent *event );

private:

    P2ObjectFrame *objectFrame;

    QWidget *borderWidget;
    QScrollArea *scrollArea;

    P2Binder *constBinder;

private slots:

    void resizeScrollArea( QResizeEvent *event );

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2View_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
