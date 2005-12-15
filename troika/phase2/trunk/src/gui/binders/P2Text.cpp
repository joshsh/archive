#include "P2Text.h"


////////////////////////////////////////////////////////////////////////////////


P2Text::P2Text( char *text )
        : P2BasicWidget()
{
    #ifdef DEBUG
        cout << "P2Text[" <<  (int) this << "]::P2Text()" << endl;
    #endif

    this->text = text;

    size = QSize( 40, 20 );

    QPainter painter( this );
    //QRect boundingRect = painter.boundingRect( QRect( 0, 0, 0, 0 ), 0, text );
//cout << "(" << boundingRect.x() << ", " << boundingRect.y() << "), "
  //   << boundingRect.width() << " x " << boundingRect.height() << endl;
    //size = boundingRect.size();

}


QSize P2Text::sizeHint() const
{
    return size;
}


bool P2Text::handleMousePressEvent( QMouseEvent *event, bool childIsBinder )
{
    return false;
}


void P2Text::paintEvent( QPaintEvent *event )
{
cout << "Drawing '" << text << "'." << endl;
    QPainter painter( this );
    QRect boundingRect = painter.boundingRect( QRect( 0, 0, 0, 0 ), 0, text );
cout << "(" << boundingRect.x() << ", " << boundingRect.y() << "), "
     << boundingRect.width() << " x " << boundingRect.height() << endl;
    //size = boundingRect.size();
    //setGeometry( boundingRect );
    //painter.setPen( Qt::black );
    painter.setPen( palette().foreground().color() );
    painter.setBrush( Qt::NoBrush );
    //painter.drawText( QPoint( 0, 0 ), text );
    //painter.drawText( QPoint( 0, 0 ), QString( text ) );

QRect borderRect( QPoint( 0, 0 ), geometry().size() - QSize( 1, 1) );
painter.drawRect( borderRect );
borderRect = QRect( QPoint( 0, 0 ), geometry().size() - QSize( 10, 10 ) );
painter.drawRect( borderRect );

    painter.drawText( 0, 10, QString( text ) );

}

