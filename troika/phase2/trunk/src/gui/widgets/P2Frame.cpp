#include "P2Frame.h"

#include <QtGui>


////////////////////////////////////////////////////////////////////////////////


P2Frame::P2Frame( P2Widget *widget, const P2Environment &env )
    : P2Widget()
{
    focusChild = 0;

    contentWidget = widget;
    contentWidget->setParent( this );

    connect(
        contentWidget,  SIGNAL( resized( QResizeEvent* ) ),
        this,           SLOT( childResizeEvent( QResizeEvent* ) ) );

    environment = &env;

    //~
    cachedSizeHint = QSize( 0, 0 );

    // For now, frame title = name of content widget.
    setTitle( objectName() );
    connect(
        this,  SIGNAL( renamed( QString ) ),
        this,           SLOT( setTitle( QString ) ) );

    // Constant for now.
    margin = FRAME__CONTENTS__PADDING;
    borderThickness = FRAME__BORDER_THICKNESS;

    //~
    refresh( env );
}


////////////////////////////////////////////////////////////////////////////////


QSize P2Frame::sizeHint() const
{
    return cachedSizeHint;
}


void P2Frame::refresh( const P2Environment &env )
{
cout << "P2Frame::refresh(" << ( int ) &env << ")" << endl;

    showBorder = env.getIdleFrameVisibility();
    showTitle = env.getNameVisibility() && !title.isNull();

    contentWidget->refresh( env );

    //~
    update();
}


void P2Frame::update()
{
cout << "# before: (" << size().width() << ", " << size().height() << ")" << endl;
//cout << "# sizeHint: (" << layout()->sizeHint().width() << ", " << layout()->sizeHint().height() << ")" << endl;
//cout << "bar" << endl; cout.flush();
//    resize( layout()->sizeHint() );
    QSize before = cachedSizeHint;
//cout << "contentWidget->size() = (" << contentWidget->width() << ", " << contentWidget->height() << ")" << endl;
//cout << "contentWidget->sizeHint() = (" << contentWidget->width() << ", " << contentWidget->height() << ")" << endl;

    // Note: it's important to use the content widget's sizeHint() here, rather
    // than its size(), which may not yet be meaningful.
    int xsize = contentWidget->sizeHint().width() + ( 2 * ( borderThickness + margin ) );
    int ysize = contentWidget->sizeHint().height() + borderThickness + ( 2 * margin );

    QSize titleSize = fontMetrics().boundingRect( title ).size()
        + QSize( 2 * FRAME__LABEL__X_PADDING, 2 );

    if ( showTitle )
    {
        ysize += titleSize.height();

        int minWidth = titleSize.width()
            + ( 2 * ( borderThickness + FRAME__LABEL__X_MARGIN ) );

        if ( xsize < minWidth )
            xsize = minWidth;

        int yoffset = titleSize.height() + margin;
        int xoffset;

        switch ( FRAME__CONTENTS__ALIGNMENT )
        {
            case Qt::AlignLeft:

                xoffset = margin + borderThickness;
                break;

            case Qt::AlignRight:

                xoffset = xsize - ( margin + borderThickness + contentWidget->sizeHint().width() );
                break;

            case Qt::AlignHCenter:

                xoffset = ( xsize - contentWidget->sizeHint().width() ) / 2;
                break;
        }

cout << "1 contentWidget->setPosition(" << xoffset << ", " << yoffset << ")" << endl;
        contentWidget->setPosition( QPoint( xoffset, yoffset ) );

        yoffset = 0;

        switch ( FRAME__LABEL__ALIGNMENT )
        {
            case Qt::AlignLeft:

                xoffset = borderThickness + FRAME__LABEL__X_MARGIN;
                break;

            case Qt::AlignRight:

                xoffset = xsize - ( borderThickness + FRAME__LABEL__X_MARGIN + titleSize.width() );
                break;

            case Qt::AlignHCenter:

                xoffset = ( xsize - titleSize.width() ) / 2;
                break;
        }

        titleRect = QRect( QPoint( xoffset, yoffset ), titleSize );

        yoffset = titleSize.height() / 2;
        borderRect = QRect( QPoint( 0, yoffset ), QSize( xsize, ysize ) - QSize( 1, 1 + yoffset ) );
    }

    else
    {
        ysize += borderThickness;

cout << "2 contentWidget->setPosition(" << margin + borderThickness << ", " << margin + borderThickness << ")" << endl;
        contentWidget->setPosition(
            QPoint( margin + borderThickness, margin + borderThickness ) );

        borderRect = QRect( QPoint( 0, 0 ), QSize( xsize, ysize ) - QSize( 1, 1) );
    }

    cachedSizeHint = QSize( xsize, ysize );
//cachedSizeHint = QSize( 50, 50 );

    if ( cachedSizeHint != before )
    {
        resize( cachedSizeHint );
        //emit resized( 0 );
    }
cout << "# after: (" << size().width() << ", " << size().height() << ")" << endl;
}


void P2Frame::childResizeEvent( QResizeEvent *event )
{
    update();
}


void P2Frame::setTitle( QString s )
{
    if ( s != title )
    {
        title = s;

        QWidget::update();
    }
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

    //if ( isDependent )
    //    ( ( P2Frame* ) parentWidget() )->setFocus( this );
}


void P2Frame::unfocus()
{
    if ( focusChild )
    {
        if ( focusChild != this )
            focusChild->unfocus();

        focusChild = 0;
        QWidget::update();
    }
}


// Event handling //////////////////////////////////////////////////////////////

    #include <iostream.h>

void P2Frame::showInfo()
{
/*
    cout << "P2Frame[" << (int) this << "]::showInfo():" << endl;
    cout << "    Geometry: (" << geometry().x() << ", " << geometry().y()
         << "), " << geometry().width() << " by " << geometry().height()
         << " pixels." << endl;
    cout << "    Children: " << ( ( P2Layout* ) layout() )->count() << endl;
*/
}


bool P2Frame::handleMousePressEvent( QMouseEvent *event, EventOrigin origin )
{
    if ( ( origin == CHILD_FRAME )
      || ( origin == CHILD_NOFRAME && focusChild == this ) )

        return true;

    else if ( event->button() == Qt::LeftButton )
    {
        setFocus( this );
        QWidget::update();
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
/*
cout << "paintEvent!" << endl; cout.flush();
cout << "  borderRect.topLeft() = (" << borderRect.x() << ", " << borderRect.y() << ")" << endl;
cout << "  borderRect.size() = (" << borderRect.width() << ", " << borderRect.height() << ")" << endl;
cout << "  titleRect.topLeft() = (" << titleRect.x() << ", " << titleRect.y() << ")" << endl;
cout << "  titleRect.size() = (" << titleRect.width() << ", " << titleRect.height() << ")" << endl;
//*/
    QPainter painter( this );
    QColor activeColor( COLOR__FRAME__ACTIVE );

    if ( focusChild != this )
    {
        QColor inactiveColor = showBorder
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

    // Draw border with title.
    if ( showTitle )
    {
        // Draw border rectangle.
        painter.drawRect( borderRect );

        // Draw opaque background rectangle for text.
        painter.setPen( QColor( COLOR__BACKGROUND ) );
        painter.setBrush( QBrush( QColor( COLOR__BACKGROUND ), Qt::SolidPattern ) );
        painter.drawRect( titleRect );

        // Draw text.
        painter.setPen( activeColor );
        painter.setBackgroundMode( Qt::OpaqueMode );
        painter.setBackground( QBrush( QColor( COLOR__BACKGROUND ), Qt::SolidPattern ) );
        painter.drawText( titleRect.x() + FRAME__LABEL__X_PADDING, titleRect.height() - 2, title );
    }

    // Draw border with no title.
    else
    {
        painter.drawRect( borderRect );
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
