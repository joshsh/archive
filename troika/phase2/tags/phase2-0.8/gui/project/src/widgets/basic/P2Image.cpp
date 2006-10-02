#include <widgets/basic/P2Image.h>


////////////////////////////////////////////////////////////////////////////////


P2Image::P2Image( const char * const xpm[] )
    : P2BasicWidget()
{
    image = QImage( xpm );

    setFixedSize( image.size() );
}


P2Image::P2Image( const QString &filename )
    : P2BasicWidget()
{
    image = QImage( filename );

    setFixedSize( image.size() );
}


P2Image::P2Image( const QImage &i )
    : P2BasicWidget()
{
    image = i;

    setFixedSize( image.size() );
}


QSize P2Image::sizeHint() const
{
    return image.size();
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
