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

    P2View( Object *o, P2Binder &b );

bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin ) { return false; }
bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) { return false; }
bool isFrame() { return false; }

    P2ObjectFrame *focusFrame();

    //QSize sizeHint() const;

public slots:

    void refresh( const P2Environment &env );

    void update( QResizeEvent *event );

private:

    P2ObjectFrame *objectFrame;

    QWidget *borderWidget;
    QScrollArea *scrollArea;

    P2Binder *binder;

private slots:

    void resizeScrollArea( QResizeEvent *event );

/*
    class FooScrollArea : public QScrollArea
    {
        FooScrollArea( QWidget *parent )
            : QScrollArea( parent )
        {

        }

        QSize sizeHint() const
        {

        }
    };
*/
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2View_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
