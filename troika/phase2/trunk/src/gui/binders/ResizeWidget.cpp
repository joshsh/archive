#include "ResizeWidget.h"  // ResizeWidget


////////////////////////////////////////////////////////////////////////////////


ResizeWidget::ResizeWidget()
        : P2BasicWidget()
{
    #ifdef DEBUG
        cout << indent()
             << "ResizeWidget[" <<  (int) this << "]::ResizeWidget()" << endl;
    #endif

    toggle = 0;
}


const QString ResizeWidget::className()
{
    return QString( "ResizeWidget" );
}


QSize ResizeWidget::sizeHint() const
{
    int len = 10 * ( toggle + 1 ) ;
    return QSize( len, len );
}


bool ResizeWidget::handleMousePressEvent( QMouseEvent *event, EventOrigin origin )
{
    if ( event->button() == Qt::LeftButton )
    {
        toggle = ( toggle + 1 ) % 3;
        resize( sizeHint() );
        update();
    }

    // Objects of this class are not meant to have dependent P2Widgets.
    return false;
}


void ResizeWidget::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );
    painter.setPen( QColor( 0x00, 0x00, 0x00, 0xFF ) );
    painter.setBrush( QBrush( QColor( 0xBF, 0xBF, 0xBF, 0xFF ) ) );
    QRect borderRect( QPoint( 0, 0 ), geometry().size() - QSize( 1, 1) );
    painter.drawRect( borderRect );
}
