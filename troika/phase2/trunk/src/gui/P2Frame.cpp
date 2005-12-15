#include "P2Frame.h"
#include "P2Layout.h"

#include <QtGui>


////////////////////////////////////////////////////////////////////////////////


P2Frame::P2Frame()
    : P2BasicWidget()
{
    #ifdef DEBUG
        cout << "P2Frame[" <<  (int) this << "]::P2Frame()" << endl;
    #endif

    // Note: no need to call setLayout after using this constructor.
    new P2Layout( this );

    focusChild = 0;
}


void P2Frame::addChild( P2BasicWidget *widget, QPoint position )
{
    #ifdef DEBUG
        cout << "P2Frame[" << (int) this << "]::addChild( " << (int) widget << " )" << endl;
    #endif

    // "When you use a layout, you don't need to pass a parent when constructing
    // the child widgets. The layout will automatically reparent the widgets
    // (using QWidget::setParent()) so that they are children of the widget on
    // which the layout is installed.
    widget->setParent( this );

    widget->isDependent = true;

    (( P2Layout* ) layout())->addWidget( widget, position );

    updateGeometry();  //~
}


// Size geometry ///////////////////////////////////////////////////////////////


QSize P2Frame::sizeHint() const
{
    return layout()->sizeHint();
}


QSize P2Frame::minimumSizeHint() const
{
    return layout()->minimumSize();
}


QSizePolicy P2Frame::sizePolicy() const
{
    return QSizePolicy();
}


void P2Frame::resize( QSize &newSize )
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


void P2Frame::setFocus( P2Frame *child )
{
//cout << (int) this << " setFocus( " << (int) child << " )" << endl;

    // If P2Frame or one of its children is already in focus...
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
        ( ( P2Frame* ) parentWidget() )->setFocus( this );
}


void P2Frame::unfocus()
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

    #include <iostream.h>

void P2Frame::showInfo()
{
    cout << "P2Frame[" << (int) this << "]::showInfo():" << endl;
    cout << "    Geometry: (" << geometry().x() << ", " << geometry().y()
         << "), " << geometry().width() << " by " << geometry().height()
         << " pixels." << endl;
    cout << "    Children: " << ( ( P2Layout* ) layout() )->count() << endl;
}


bool P2Frame::handleMousePressEvent( QMouseEvent *event, bool childIsBinder )
{
    if ( event->button() == Qt::RightButton )
    {
        // If the P2Frame is in focus...
        if ( focusChild == this )
        {
            showInfo();
            return false;
        }

        else
            return true;
    }

    else if ( event->button() == Qt::LeftButton )
    {
        // If the P2Frame is already in focus...
        if ( focusChild == this )
        {
            //setFocus( this );
            //update();
            return true;
            //if ( depth <= 1 )
            //    return true;
        }

        else if ( childIsBinder )
            return true;

        else
        {
            setFocus( this );
            update();
            return false;
        }

        // "Second tap" event allows access to the wrapped QWidget, if not itself
        // a P2Frame.
        //else
        //    return true;
    }

    // Don't care about other QMouseEvents.
    else
        return true;
}


// Note: QPaintEvent object is not used.
void P2Frame::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );

    if ( focusChild != this )
    {
        QColor inactiveColor = environment()->idleFramesAreVisible
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


bool P2Frame::acceptDrops () const
{
    return true;
}


