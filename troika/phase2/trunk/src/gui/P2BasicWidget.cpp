#include "P2BasicWidget.h"


////////////////////////////////////////////////////////////////////////////////


P2BasicWidget::P2BasicWidget()
    : QWidget( 0, 0 )
{
    #ifdef DEBUG
        cout << "P2BasicWidget[" << (int) this << "]::P2BasicWidget()" << endl;
    #endif

    isDependent = false;
}


void P2BasicWidget::setPosition( QPoint p )
{
    setGeometry( QRect( p, geometry().size() ) );
}


bool P2BasicWidget::mousePressEventWrapper( QMouseEvent *event, bool childIsBinder )
{
    if ( isDependent )
    {
        // Note: the position info in the QMouseEvent will not be meaningful
        // to the parent P2Binder.
        if ( ( ( P2BasicWidget* ) parentWidget() )->mousePressEventWrapper( event, isBinder() ) )
            handleMousePressEvent( event, childIsBinder );
    }

    // No parent to tell this widget what to do.
    else
        handleMousePressEvent( event, childIsBinder );
}


void P2BasicWidget::mousePressEvent( QMouseEvent *event )
{
    mousePressEventWrapper( event, false );
}

