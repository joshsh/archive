#include "P2Frame.h"
#include "layout/P2Layout.h"

#include <QtGui>


////////////////////////////////////////////////////////////////////////////////


P2Frame::P2Frame()
    : P2Widget()
{
    #ifdef DEBUG
        cout << indent()
             << "P2Frame[" <<  (int) this << "]::P2Frame()" << endl;
    #endif

    // Note: no need to call setLayout after using this constructor.
    new P2Layout( this );

    focusChild = 0;
}


const QString P2Frame::className()
{
    return QString( "P2Frame" );
}


void P2Frame::addChild( P2Widget *widget, const QPoint &position )
{
    #ifdef DEBUG
        cout << indent()
             << "P2Frame[" << (int) this << "]::addChild( " << (int) widget << " )" << endl;
    #endif

    // "When you use a layout, you don't need to pass a parent when constructing
    // the child widgets. The layout will automatically reparent the widgets
    // (using QWidget::setParent()) so that they are children of the widget on
    // which the layout is installed.
    widget->setParent( this );

    widget->isDependent = true;

    (( P2Layout* ) layout())->addWidget( widget, position );

    //updateGeometry();  //~
}


void P2Frame::refresh()
{
    P2Layout *l = ( P2Layout* ) layout();
    QSize labelSize = fontMetrics().boundingRect( objectName() ).size();

    QPoint offset;
    int minWidth;

    // Find label offset and minimum width.
    if ( environment()->getNameVisibility() && objectName() != 0 )
    {
        offset = QPoint( 0, labelSize.height() + FRAME__CONTENTS__PADDING );
        minWidth = labelSize.width() + ( 2 * ( FRAME__LABEL__X_PADDING + FRAME__LABEL__X_MARGIN ) );
    }

    else
    {
        offset = QPoint( 0, 0 );
        minWidth = 0;
    }

    // Refresh label params.
    l->setContentOffset( offset );
    l->setMinimumSize( QSize( minWidth, 0 ) );

    // Refresh minimum size.

    // Propagate "refresh" signal to children.
    l->refresh();
}

/*
void P2Frame::resizeEvent()
{
        ( ( P2CentralWidget* ) parentWidget() )->resizeEvent( 0 );
}
*/

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


bool P2Frame::handleMousePressEvent( QMouseEvent *event, EventOrigin origin )
{
    if ( ( origin == CHILD_FRAME )
      || ( origin == CHILD_NOFRAME && focusChild == this ) )

        return true;

    else if ( event->button() == Qt::LeftButton )
    {
        setFocus( this );
        update();
        return false;
    }

    else
        return false;
}


// Caution: this system allows non-frame children to catch move events
// immediately after the mouse click which initiated them.  If it were the
// mouseReleaseEvent which causes a focus, then this would not be an issue.
bool P2Frame::handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin )
{
    return ( ( origin == CHILD_FRAME )
          || ( origin == CHILD_NOFRAME && focusChild == this ) );
}


// Note: QPaintEvent object is not used.
void P2Frame::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );
    QColor activeColor( COLOR__FRAME__ACTIVE );

    if ( focusChild != this )
    {
        QColor inactiveColor = environment()->getIdleFrameVisibility()
            ? QColor( COLOR__FRAME__INACTIVE )
            : Qt::white;
        painter.setPen( inactiveColor );
    }

    else
    {
        QPen activePen( Qt::NoBrush, 1, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin );
        activePen.setColor( activeColor );
        painter.setPen( activePen );
    }

    QRect borderRect;

    // Draw border with object name as a label.
    if ( ( objectName() != 0 ) && environment()->getNameVisibility() )
    {
        QSize size = fontMetrics().boundingRect( objectName() ).size();
        int yoffset = size.height() / 2;
        int xoffset;

        // Draw border rectangle.
        borderRect = QRect( QPoint( 0, yoffset ), geometry().size() - QSize( 1, 1 + yoffset ) );
        painter.drawRect( borderRect );

        switch ( FRAME__LABEL__ALIGNMENT )
        {
            case Qt::AlignLeft:

                xoffset = FRAME__LABEL__X_MARGIN + FRAME__LABEL__X_PADDING;
                break;

            case Qt::AlignRight:

                xoffset = width() - ( FRAME__LABEL__X_MARGIN + FRAME__LABEL__X_PADDING + size.width() );
                break;

            case Qt::AlignHCenter:

                xoffset = ( width() - size.width() ) / 2;
                break;
        }

        QRect textRect = QRect(
            QPoint( xoffset - FRAME__LABEL__X_PADDING, yoffset),
            size + QSize( 2 * FRAME__LABEL__X_PADDING, 0 ) );

        // Draw opaque background rectangle for text.
        painter.setPen( QColor( COLOR__BACKGROUND ) );
        painter.setBrush( QBrush( QColor( COLOR__BACKGROUND ), Qt::SolidPattern ) );
        painter.drawRect( textRect );

        // Draw text.
        painter.setPen( activeColor );
        painter.setBackgroundMode( Qt::OpaqueMode );
        painter.setBackground( QBrush( QColor( COLOR__BACKGROUND ), Qt::SolidPattern ) );
        painter.drawText( xoffset, size.height() - 2, objectName() );
    }

    // Draw border with no label.
    else
    {
        // Draw border rectangle.
        borderRect = QRect( QPoint( 0, 0 ), geometry().size() - QSize( 1, 1) );
        painter.drawRect( borderRect );
    }
}


bool P2Frame::acceptDrops () const
{
    return true;
}


