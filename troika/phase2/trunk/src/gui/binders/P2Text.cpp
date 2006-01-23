#include "P2Text.h"


////////////////////////////////////////////////////////////////////////////////


P2Text::P2Text( char *text, QColor *color )
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

    size = fontMetrics().boundingRect( text ).size();
}


const QString P2Text::className()
{
    return QString( "P2Text" );
}


QSize P2Text::sizeHint() const
{
    return size;
}


void P2Text::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );

    painter.setPen( color );
    //painter.setBrush( Qt::NoBrush );

    // Note: assumes the same font as the one used to find the bounding
    // rectangle at construction time.
    painter.drawText( 0, size.height(), QString( text ) );
}

