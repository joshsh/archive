#include "P2Image.h"


////////////////////////////////////////////////////////////////////////////////


P2Image::P2Image( const char * const xpm[] )
        : P2BasicWidget()
{
    #ifdef DEBUG
        cout << "P2Image[" <<  (int) this << "]::P2Image()" << endl;
    #endif

    image = QImage( xpm );

    setFixedSize( image.size() );
}


bool P2Image::handleMousePressEvent( QMouseEvent *event, bool childIsBinder )
{
    // Objects of this class are not meant to have dependent P2BasicWidgets.
    return false;
}


void P2Image::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );
    painter.drawImage(
        0, 0,  // dest. origin
        image,
        0, 0,  // source origin
        -1, -1,  // source maxima
        0 );  // conversionFlags
}

