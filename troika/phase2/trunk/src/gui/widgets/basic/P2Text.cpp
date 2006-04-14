#include <widgets/basic/P2Text.h>


////////////////////////////////////////////////////////////////////////////////


P2Text::P2Text( QString text, QColor *color )
        : P2BasicWidget()
{
    #ifdef DEBUG
        cout << indent()
             << "P2Text[" <<  (int) this << "]::P2Text()" << endl;
    #endif

    this->text = text;

    this->color = color
        ? *color
        : palette().foreground().color();

    cachedSizeHint = fontMetrics().boundingRect( text ).size() + QSize( 0, 2 );
}


const QString P2Text::className()
{
    return QString( "P2Text" );
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
    painter.drawText( 0, cachedSizeHint.height() - 2, text );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
