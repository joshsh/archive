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
#include <widgets/basic/P2Title.h>
#include <widgets/P2Frame.h>

////////////////////////////////////////////////////////////////////////////////

class P2ObjectFrame : public P2Widget
{
    Q_OBJECT

public:

    P2ObjectFrame( Object *o, QString title, P2Binder &b, bool initiallyExpanded = false );

bool isFrame() { return true; }

bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin ) { return false; }
bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) { return false; }

    QSize sizeHint() const;

void focusInEvent ( QFocusEvent * event )
{
    event = 0;
    cout << "Focus is in! --------------" << endl;
    setActive( true );
    plusMinus->setActive( true );
    titleWidget->setActive( true );
}
void focusOutEvent ( QFocusEvent * event )
{
    event = 0;
    cout << "Focus is out... -----------" << endl;
    setActive( false );
    plusMinus->setActive( false );
    titleWidget->setActive( false );
}

public slots:

    void refresh( const P2Environment &env );
    void update( QResizeEvent *e );
    void update2();

protected:

    void paintEvent( QPaintEvent *event );



    void mousePressEvent( QMouseEvent *event )
    {
        setActive( true );
    }




private:

    Object *object;
    P2Binder *binder;

    P2Widget *contentWidget;
    P2Title *titleWidget;
    P2PlusMinus *plusMinus;

    QRect borderRect;
    QSize cachedSizeHint;
    //QStackedWidget *stackedWidget;

    // Visibility of child widgets.
    bool showContent;
    bool showPlusMinus;
    bool showTitle;
    bool showBorder;

    void setTitle( QString s );

private slots:

    void expand();
    void contract();

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2ObjectFrame_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
