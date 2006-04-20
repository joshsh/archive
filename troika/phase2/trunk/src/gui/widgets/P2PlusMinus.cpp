#include <widgets/P2PlusMinus.h>


////////////////////////////////////////////////////////////////////////////////


P2PlusMinus::P2PlusMinus( QWidget *parent, bool initiallyPlus )
    : QWidget( parent, 0 )
{
    isPlus = initiallyPlus;

    scale = 2;

    //textSize = ( initiallyPlus )
    //    ? fontMetrics().boundingRect( "+" ).size()
    //    : fontMetrics().boundingRect( "-" ).size();

    //cachedSizeHint = fontMetrics().boundingRect( "+" ).size() + QSize( 4, 6 );
    int len = ( 2 * scale ) + 5;
    cachedSizeHint = QSize( len, len );
}


QSize P2PlusMinus::sizeHint() const
{
    return cachedSizeHint;
}


void P2PlusMinus::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );
    QRect r = QRect( QPoint( 0, 0 ), cachedSizeHint - QSize( 1, 1 ) );

    painter.setBrush( QBrush( QColor( COLOR__BACKGROUND ), Qt::SolidPattern ) );
    painter.drawRect( r );
    painter.setBrush( Qt::NoBrush );

    //painter.setPen( QColor( 0x3F, 0x3F, 0x3F, 0xFF ) );

    painter.setPen( QColor( COLOR__FRAME__INACTIVE ) );

    // Text.
    painter.drawLine( 2, 2 + scale, 2 + ( 2 * scale ), 2 + scale );
    if ( isPlus )
        painter.drawLine( 2 + scale, 2, 2 + scale, 2 + ( 2 * scale ) );

    //painter.setPen( QColor( COLOR__FRAME__INACTIVE ) );

    // Border.
    painter.drawRect( r );
}


void P2PlusMinus::mousePressEvent( QMouseEvent *event )
{
    event = 0;

    if ( isPlus )
    {
        isPlus = false;
        emit clickedPlus();
    }

    else
    {
        isPlus = true;
        emit clickedMinus();
    }

update();
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
