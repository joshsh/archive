#ifndef P2ObjectFrame_h
#define P2ObjectFrame_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
}

#include <widgets/P2Widget.h>
#include <widgets/P2PlusMinus.h>
#include <P2Binder.h>
#include <widgets/basic/P2Text.h>
#include <widgets/P2Frame.h>

////////////////////////////////////////////////////////////////////////////////

class P2ObjectFrame : public P2Widget
{
    Q_OBJECT

public:

    P2ObjectFrame( Object *o, QString title, P2Binder &b, bool initiallyExpanded = false );

const QString className() { return QString( "P2ObjectFrame" ); }
bool isFrame() { return false; }

bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin ) { return false; }
bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) { return false; }

    QSize sizeHint() const;

    void setTitle( QString s );

public slots:

    void refresh( const P2Environment &env );
    void update( QResizeEvent *e );

private:

    Object *object;
    QString title;
    bool expanded;
    P2Binder *binder;

    P2PlusMinus *plusMinus;

    QStackedWidget *stackedWidget;
    P2Text *titleWidget;
    P2Frame *frame;

private slots:

    void expand();
    void contract();

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2ObjectFrame_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
