#include <widgets/basic/P2Text.h>


////////////////////////////////////////////////////////////////////////////////


P2Text::P2Text( QString text, const QColor &color )
        : P2BasicWidget()
{
    #ifdef DEBUG
        cout << indent()
             << "P2Text[" <<  (int) this << "]::P2Text()" << endl;
    #endif

    this->text = text;
//this->text += "<FONT COLOR='#FF0000'>...</FONT>";

    this->color = color;

    int h = fontMetrics().height();
    //cachedSizeHint = fontMetrics().boundingRect( text ).size() + QSize( 0, 2 );
    cachedSizeHint = QSize( fontMetrics().boundingRect( this->text ).width() + 1, h );
}


QSize P2Text::sizeHint() const
{
    return cachedSizeHint;
}


void P2Text::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );

    painter.setPen( color );
    //painter.setBrush( Qt::NoBrush );

    // Note: assumes the same font as the one used to find the bounding
    // rectangle at construction time.
    painter.drawText( 0, cachedSizeHint.height() - 4, text );
//painter.drawRect( QRect( QPoint( 0, 0 ), cachedSizeHint - QSize( 1, 1 ) ) );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
