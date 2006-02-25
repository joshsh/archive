#include "P2Frame.h"
#include "layout/P2Layout.h"

#include <QtGui>


////////////////////////////////////////////////////////////////////////////////


P2Frame::P2Frame( const P2Environment &env )
    : P2Widget()
{
    #ifdef DEBUG__FRAME
        cout << indentPlus()
             << "P2Frame[" <<  (int) this << "]::P2Frame()" << endl;
    #endif

    // Note: no need to call setLayout after using this constructor.
    P2Layout *layout = new P2Layout( this );

    connect( layout, SIGNAL( resized() ), this, SLOT( layoutResizedEvent() ) );

    focusChild = 0;

    environment = &env;

setAcceptDrops(true);

    #ifdef DEBUG__FRAME
        indentMinus();
    #endif
}


const QString P2Frame::className()
{
    return QString( "P2Frame" );
}


void P2Frame::addChild( P2Widget *widget, const QPoint &position )
{
    #ifdef DEBUG__FRAME
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


void P2Frame::refresh( const P2Environment &env )
{
    P2Layout *l = ( P2Layout* ) layout();

    QPoint offset;
    int minWidth;

    // Find label offset and minimum width.
    if ( env.getNameVisibility() && objectName() != 0 )
    {
        QSize labelSize = fontMetrics().boundingRect( objectName() ).size();
        offset = QPoint( 0, labelSize.height() + FRAME__CONTENTS__PADDING );
        minWidth = labelSize.width()
            + ( 2 * ( FRAME__LABEL__X_PADDING + FRAME__LABEL__X_MARGIN ) );
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
    l->refreshChildren( env );
}


// Focus ///////////////////////////////////////////////////////////////////////


P2Frame *P2Frame::focusFrame()
{
    return focusChild;
}


void P2Frame::setFocus( P2Frame *child )
{
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
    if ( focusChild )
    {
        if ( focusChild != this )
            focusChild->unfocus();

        focusChild = 0;
        update();
    }
}


void P2Frame::layoutResizedEvent()
{
    resize( layout()->sizeHint() );
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

    if ( ( origin == CHILD_FRAME )
          || ( origin == CHILD_NOFRAME && focusChild == this ) )
        return true;
    else
    {




    if ( (event->buttons() & Qt::LeftButton)
      && !( ( event->pos() - dragStartPosition ).manhattanLength()
         < QApplication::startDragDistance() ) )
    {
        QDrag *drag = new QDrag( this );
        QMimeData *mimeData = new QMimeData;

        //mimeData->setData( mimeType, data );
        mimeData->setText( "P2Widget" );
        drag->setMimeData( mimeData );

        Qt::DropAction dropAction = drag->start( Qt::CopyAction | Qt::MoveAction );
        //...
    }




        return false;
    }
}


// Note: QPaintEvent object is not used.
void P2Frame::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );
    QColor activeColor( COLOR__FRAME__ACTIVE );

    if ( focusChild != this )
    {
        QColor inactiveColor = environment->getIdleFrameVisibility()
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
    if ( ( objectName() != 0 ) && environment->getNameVisibility() )
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


    void P2Frame::dragEnterEvent( QDragEnterEvent *event )
    {
        //if (event->mimeData()->hasFormat("text/plain"))
            event->acceptProposedAction();
    }


    void P2Frame::dropEvent( QDropEvent *event )
    {
        cout << "Accepting a drop event: " << event->mimeData()->text().toStdString() << endl;

        //textBrowser->setPlainText(event->mimeData()->text());
        //mimeTypeCombo->clear();
        //mimeTypeCombo->addItems(event->mimeData()->formats());

        event->acceptProposedAction();
    }

    // S.a.  dragMoveEvent, dragLeaveEvent


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on