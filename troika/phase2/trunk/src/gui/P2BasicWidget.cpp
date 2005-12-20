#include "P2BasicWidget.h"
#include "P2Layout.h"


////////////////////////////////////////////////////////////////////////////////


P2BasicWidget::P2BasicWidget()
    : QWidget( 0, 0 )
{
    #ifdef DEBUG
        cout << "P2BasicWidget[" << (int) this << "]::P2BasicWidget()" << endl;
    #endif

    isDependent = false;

    //setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );
}


void P2BasicWidget::setCenter( const QPoint &p )
{
    setGeometry( geometry().translated( p - geometry().center() ) );
}


void P2BasicWidget::setPosition( const QPoint &p )
{
    setGeometry( QRect( p, geometry().size() ) );
}


void P2BasicWidget::setSize( const QSize &s )
{
cout << "P2BasicWidget[" << (int) this << "]::setSize( QSize( "
     << s.width() << ", " << s.height() << ") )" << endl;

    if ( s != size() )
    {
        // Reset sizeHint.
        //setMinimumSize( s );

        resize( s );

        if ( isDependent )
            ( ( P2Layout* ) parentWidget()->layout() )->adjustGeometry();
        //if ( isDependent )
        //    parentWidget()->layout()->parentWidget()->updateGeometry();
    }
}


bool P2BasicWidget::mousePressEventWrapper( QMouseEvent *event, bool childIsBinder )
{
    clearFocus();

    if ( isDependent )
    {
        // Note: the position info in the QMouseEvent will not be meaningful
        // to the parent P2Binder.
        if ( ( ( P2BasicWidget* ) parentWidget() )->mousePressEventWrapper( event, isFrame() ) )
            return handleMousePressEvent( event, childIsBinder );
    }

    // No parent to tell this widget what to do.
    else
    {
        setFocus();
        return handleMousePressEvent( event, childIsBinder );
    }

    return false;
}


void P2BasicWidget::mousePressEvent( QMouseEvent *event )
{
    mousePressEventWrapper( event, false );
}

