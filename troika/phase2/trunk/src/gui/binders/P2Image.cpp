#include "P2Image.h"


////////////////////////////////////////////////////////////////////////////////


P2Image::P2Image( const char * const xpm[] )
        : P2BasicWidget()
{
    #ifdef DEBUG
        cout << indent()
             << "P2Image[" <<  (int) this << "]::P2Image()" << endl;
    #endif

    image = QImage( xpm );

    setFixedSize( image.size() );
}


const QString P2Image::className()
{
    return QString( "P2Image" );
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


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
