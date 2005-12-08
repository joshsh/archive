#include "P2Binder.h"
#include "P2Layout.h"

#include <QtGui>

    #include <iostream.h>

////////////////////////////////////////////////////////////////////////////////


P2Binder::P2Binder()
    : P2BasicWidget()
{
    #ifdef DEBUG
        cout << "P2Binder[" <<  (int) this << "]::P2Binder()" << endl;
    #endif

    initialize();
}


P2Binder::P2Binder( QWidget *parent, P2Environment *e )
    : P2BasicWidget( parent, e )
{
    #ifdef DEBUG
        cout << "P2Binder[" << (int) this << "]::P2Binder( "
             << (int) parent << ", "
             << (int) e << " )" << endl;
    #endif

    initialize();
}


void P2Binder::initialize()
{
    // Note: no need to call setLayout after using this constructor.
    new P2Layout( this );

    //parentBinder = 0;
    focusChild = 0;
}


void P2Binder::addDependentWidget( P2BasicWidget *widget, QPoint position )
{
    #ifdef DEBUG
        cout << "P2Binder[" << (int) this << "]::addDependentWidget( " << (int) widget << " )" << endl;
    #endif

    // "When you use a layout, you don't need to pass a parent when constructing
    // the child widgets. The layout will automatically reparent the widgets
    // (using QWidget::setParent()) so that they are children of the widget on
    // which the layout is installed.
    widget->setParent( this );

    widget->isDependent = true;
    widget->environment = environment;

    (( P2Layout* ) layout())->addWidget( widget, position );

    updateGeometry();  //~
}


// Size geometry ///////////////////////////////////////////////////////////////


QSize P2Binder::sizeHint() const
{
    return layout()->sizeHint();
}


QSize P2Binder::minimumSizeHint() const
{
    return layout()->minimumSize();
}


QSizePolicy P2Binder::sizePolicy() const
{
    return QSizePolicy();
}


void P2Binder::resize( QSize newSize )
{
    QSize oldSize = geometry().size();
    QPoint center = geometry().center();
    setGeometry(
        center.x() - newSize.width() / 2,
        center.y() - newSize.height() / 2,
        newSize.width(),
        newSize.height() );
}


// Focus ///////////////////////////////////////////////////////////////////////


void P2Binder::setFocus( P2Binder *child )
{
//cout << (int) this << " setFocus( " << (int) child << " )" << endl;

    // If P2Binder or one of its children is already in focus...
    if ( focusChild )
    {
        // If this is a CHANGE of focus...
        if ( focusChild != child )
        {
            focusChild->unfocus();
            focusChild = child;
        }
    }

    else
        focusChild = child;

    if ( isDependent )
        ( ( P2Binder* ) parentWidget() )->setFocus( this );
}


void P2Binder::unfocus()
{
//cout << (int) this << " unFocus" << endl;

    if ( focusChild )
    {
        if ( focusChild != this )
            focusChild->unfocus();

        focusChild = 0;
        update();
    }
}


// Event handling //////////////////////////////////////////////////////////////


bool P2Binder::handleMousePressEvent( QMouseEvent *event )
{
    // "Second tap" event allows access to the wrapped QWidget, if not itself
    // a P2Binder.
    if ( focusChild == this )
    {
        // ...
        return true;
    }

    // "First tap" event selects the P2Binder and deselects all others.
    else
    {
        setFocus( this );
        update();
        return false;
    }
}


// Note: event object is not used.
void P2Binder::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );

    if ( focusChild != this )
    {
        QColor inactiveColor = environment->idleFramesAreVisible
            ? QColor( 0xBF, 0xBF, 0xFF, 0xFF )  // Light blue.
            : Qt::white;
        painter.setPen( inactiveColor );
    }

    else
    {
        QColor activeColor( 0x00, 0x00, 0xFF, 0xFF );  // Full blue.
        QPen activePen( Qt::NoBrush, 1, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin );
        activePen.setColor( activeColor );
        painter.setPen( activePen );
    }

    QRect borderRect( QPoint( 0, 0 ), geometry().size() - QSize( 1, 1) );
    painter.drawRect( borderRect );
}


bool P2Binder::acceptDrops () const
{
    return true;
}


