#include "P2Widget.h"


////////////////////////////////////////////////////////////////////////////////


P2Widget::P2Widget()
    : QWidget( 0, 0 )
{
    #ifdef DEBUG
        cout << indent()
             << "P2Widget[" << (int) this << "]::P2Widget()" << endl;
    #endif

    isDependent = false;

    //setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );
}


void P2Widget::setCenter( const QPoint &p )
{
    setGeometry( geometry().translated( p - geometry().center() ) );
}


void P2Widget::setPosition( const QPoint &p )
{
    setGeometry( geometry().translated( p - geometry().topLeft() ) );
}


bool P2Widget::mousePressEventWrapper( QMouseEvent *event, EventOrigin origin )
{
    if ( event->button() == Qt::LeftButton )
        dragStartPosition = event->pos();

    clearFocus();

    if ( isDependent )
    {
        // Note: the position info in the QMouseEvent will not be meaningful
        // to the parent P2Binder.
        if ( ( ( P2Widget* ) parentWidget() )->mousePressEventWrapper( event, originIfFrame() ) )
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


bool P2Widget::mouseMoveEventWrapper( QMouseEvent *event, EventOrigin origin )
{
    clearFocus();

    if ( isDependent )
    {
        // Note: the position info in the QMouseEvent will not be meaningful
        // to the parent P2Binder.
        if ( ( ( P2Widget* ) parentWidget() )->mouseMoveEventWrapper( event, originIfFrame() ) )
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
bool P2Widget::mousePressEventWrapper( QMouseEvent *event, bool childIsBinder )
{
    clearFocus();

    if ( isDependent )
    {
        // Note: the position info in the QMouseEvent will not be meaningful
        // to the parent P2Binder.
        if ( ( ( P2Widget* ) parentWidget() )->mousePressEventWrapper( event, isFrame() ) )
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

void P2Widget::mousePressEvent( QMouseEvent *event )
{
    mousePressEventWrapper( event, SELF );

/*
            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;

            //mimeData->setText(commentEdit->toPlainText());
            //drag->setMimeData(mimeData);
            //drag->setPixmap(iconPixmap);
            mimeData->setText( "P2Widget" );
            drag->setMimeData( mimeData );

            Qt::DropAction dropAction = drag->start();
*/
}


void P2Widget::mouseMoveEvent( QMouseEvent *event )
{
    mouseMoveEventWrapper( event, SELF );
}


void P2Widget::resizeEvent( QResizeEvent *event )
{
    emit resized( event );
}

