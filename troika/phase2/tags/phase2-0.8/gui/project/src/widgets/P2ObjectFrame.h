#ifndef P2ObjectFrame_h
#define P2ObjectFrame_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
}

#include <P2Binder.h>
#include <widgets/P2Widget.h>
#include <widgets/P2PlusMinus.h>
#include <widgets/basic/P2Title.h>

////////////////////////////////////////////////////////////////////////////////

/** An expandable widget to represent a Phase2 Object. */
class P2ObjectFrame : public P2Widget
{
    Q_OBJECT

public:

    P2ObjectFrame( const Object *o, QString title, P2Binder &b, bool initiallyExpanded = false );

    QSize sizeHint() const;

public slots:

    void refresh( const P2Environment &env );
    void update();

    /** \return  the Phase2 object represented by this widget */
    const Object *object() const;

protected:

    void paintEvent( QPaintEvent *event );

    void mousePressEvent( QMouseEvent *event );
    void mouseDoubleClickEvent( QMouseEvent *event );

    void focusInEvent( QFocusEvent *event );
    void focusOutEvent( QFocusEvent *event );

private:

    const Object *constObject;

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
