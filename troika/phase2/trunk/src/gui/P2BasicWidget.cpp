#include "P2BasicWidget.h"
#include "P2Layout.h"


////////////////////////////////////////////////////////////////////////////////


P2BasicWidget::P2BasicWidget()
    : QWidget( 0, 0 )
{
    #ifdef DEBUG
        cout << indent()
             << "P2BasicWidget[" << (int) this << "]::P2BasicWidget()" << endl;
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
    setGeometry( geometry().translated( p - geometry().topLeft() ) );
}


void P2BasicWidget::setSize( const QSize &s )
{
    // Only if the widget needs to resize...
    if ( s != size() )
    {
        resize( s );

        if ( isDependent )
            ( ( P2Layout* ) parentWidget()->layout() )->adjustGeometry();
    }
}


bool P2BasicWidget::mousePressEventWrapper( QMouseEvent *event, EventOrigin origin )
{
    clearFocus();

    if ( isDependent )
    {
        // Note: the position info in the QMouseEvent will not be meaningful
        // to the parent P2Binder.
        if ( ( ( P2BasicWidget* ) parentWidget() )->mousePressEventWrapper( event, originIfFrame() ) )
        {
            #ifndef DISABLE_FOCUS
                setFocus();
            #endif
            return handleMousePressEvent( event, origin );
        }

        else
            return false;
    }

    // No parent to tell this widget what to do.
    else
    {
        #ifndef DISABLE_FOCUS
            setFocus();
        #endif
        return handleMousePressEvent( event, origin );
    }
}


bool P2BasicWidget::mouseMoveEventWrapper( QMouseEvent *event, EventOrigin origin )
{
    clearFocus();

    if ( isDependent )
    {
        // Note: the position info in the QMouseEvent will not be meaningful
        // to the parent P2Binder.
        if ( ( ( P2BasicWidget* ) parentWidget() )->mouseMoveEventWrapper( event, originIfFrame() ) )
        {
            #ifndef DISABLE_FOCUS
                setFocus();
            #endif
            return handleMouseMoveEvent( event, origin );
        }

        else
            return false;
    }

    // No parent to tell this widget what to do.
    else
    {
        #ifndef DISABLE_FOCUS
            setFocus();
        #endif
        return handleMouseMoveEvent( event, origin );
    }
}


/*
bool P2BasicWidget::mousePressEventWrapper( QMouseEvent *event, bool childIsBinder )
{
    clearFocus();

    if ( isDependent )
    {
        // Note: the position info in the QMouseEvent will not be meaningful
        // to the parent P2Binder.
        if ( ( ( P2BasicWidget* ) parentWidget() )->mousePressEventWrapper( event, isFrame() ) )
        {
            setFocus();
            return handleMousePressEvent( event, childIsBinder );
        }

        else
            return false;
    }

    // No parent to tell this widget what to do.
    else
    {
        setFocus();
        return handleMousePressEvent( event, childIsBinder );
    }
}
//*/

void P2BasicWidget::mousePressEvent( QMouseEvent *event )
{
    mousePressEventWrapper( event, SELF );
}


void P2BasicWidget::mouseMoveEvent( QMouseEvent *event )
{
    mouseMoveEventWrapper( event, SELF );
}

