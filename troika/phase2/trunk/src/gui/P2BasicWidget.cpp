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


P2BasicWidget::P2BasicWidget( QWidget *parent, P2Environment *e )
    : QWidget( parent, 0 )
{
    #ifdef DEBUG
        cout << "P2BasicWidget[" << (int) this << "]::P2BasicWidget( "
             << (int) parent << ", "
             << (int) e << " )" << endl;
    #endif

    isDependent = false;
    environment = e;
}


void P2BasicWidget::setPosition( QPoint p )
{
    setGeometry( QRect( p, geometry().size() ) );
}


bool P2BasicWidget::mousePressEventWrapper( QMouseEvent *event )
{
    if ( isDependent )
    {
        // Note: the position info in the QMouseEvent will not be meaningful
        // to the parent P2Binder.
        if ( ( ( P2BasicWidget* ) parentWidget() )->mousePressEventWrapper( event ) )
            handleMousePressEvent( event );
    }

    // No parent to tell this widget what to do.
    else
        handleMousePressEvent( event );
}


void P2BasicWidget::mousePressEvent( QMouseEvent *event )
{
    mousePressEventWrapper( event );
}

