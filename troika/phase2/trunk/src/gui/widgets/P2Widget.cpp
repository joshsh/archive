#include <widgets/P2Widget.h>


////////////////////////////////////////////////////////////////////////////////


P2Widget::P2Widget()
    : QWidget( 0, 0 ),
      active( false ),
      isDependent( false )
{
    #ifdef DEBUG
        cout << indent()
             << "P2Widget[" << (int) this << "]::P2Widget()" << endl;
    #endif

    //setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );

    connect(    this,   SIGNAL( activated() ),
                this,   SLOT( update() ) );
    connect(    this,   SIGNAL( deactivated() ),
                this,   SLOT( update() ) );
}


bool P2Widget::isActive()
{
    return active;
}


void P2Widget::setActive( bool a )
{
    bool before = active;
    active = a;
    if ( active != before )
    {
        if ( active )
            emit activated();
        else
            emit deactivated();
    }
}


void P2Widget::setObjectName( QString s )
{
    if ( s.compare( objectName() ) )
    {
        QObject::setObjectName( s );
        emit renamed();
    }
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

    emit mousePress( event, this );

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


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
