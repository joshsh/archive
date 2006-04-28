#include <widgets/basic/P2BitmapEditor.h>


////////////////////////////////////////////////////////////////////////////////


P2BitmapEditor::P2BitmapEditor( QImage *image0, int zoom0, bool showGridLines0 )
        : P2BasicWidget()
{
    #ifdef DEBUG
        cout << indent()
             << "P2BitmapEditor[" <<  (int) this << "]::P2BitmapEditor( QImage *, int, bool )" << endl;
    #endif

    if ( image0 )
        image = *image0;
    else
    {
        // 16x16 dimensions, 32-bit depth.
        image = QImage( QSize( 11, 11 ), QImage::Format_RGB32 );
        //image.create( 11, 11, 32 );

        // R, G, B and alpha color components.
        image.fill( QColor( 0xFF, 0xFF, 0xFF, 0xFF ).rgb() );

        // Enable transparency.
        //image.setAlphaBuffer( true );
    }

    setZoomFactor( zoom0 );
    showGridLines = showGridLines0;

    //setSizePolicy( QSizePolicy::Minimum ); //, QSizePolicy::Minimum );
    curColor = Qt::black;
    zoom = 5;

    setFixedSize( sizeHint() );

    drawMode = DRAW;
}


void P2BitmapEditor::setPenColor( const QColor &newColor )
{
    curColor = newColor;
}


void P2BitmapEditor::setZoomFactor( int newZoom )
{
    if ( newZoom < 1)
        newZoom = 1;

    if ( newZoom != zoom )
    {
         zoom = newZoom;
         update();  // Schedule a paint event.
         updateGeometry();
    }
}


void P2BitmapEditor::setIconImage( const QImage &newImage )
{
    if ( newImage != image )
    {
        //image = newImage.convertDepth( 32 );
        image.detach();
        update();  // Schedule a paint event.
        updateGeometry();
    }
}


QSize P2BitmapEditor::sizeHint() const
{
    QSize size = zoom * image.size();
    if ( ( zoom >= 3 ) && ( showGridLines ) )
        size += QSize( 1, 1 );
    return size;
}


void P2BitmapEditor::setShowGridLines( bool newShowGridLines )
{
    showGridLines = newShowGridLines;
}


void P2BitmapEditor::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton )
        drawMode = DRAW;
    else if ( event->button() == Qt::RightButton )
        drawMode = ERASE;
    else
        drawMode = NODRAW;

    setImagePixel( event->pos() );
}


void P2BitmapEditor::mouseMoveEvent( QMouseEvent *event )
{
    setImagePixel( event->pos() );
}


//- Consider double-buffering so as to avoid having to re-draw the entire
//  grid each time.
void P2BitmapEditor::paintEvent( QPaintEvent *event )
{
    event = 0;

    QPainter painter( this );

    for ( int i = 0; i < image.width(); ++i )
        for ( int j = 0; j < image.height(); ++j )
        {
            QColor color;
            QRgb rgb = image.pixel( i, j );

            if ( qAlpha( rgb ) == 0 )
                color = palette().base().color();
            else
                color.setRgb( rgb );

            if ( ( zoom >= 3 ) && ( showGridLines ) )
                painter.fillRect( zoom * i + 1, zoom * j + 1,
                          zoom - 1, zoom - 1, color );
            else
                painter.fillRect( zoom * i, zoom * j,
                          zoom, zoom, color );
        }
        //    drawImagePixel( &painter, i, j );

    // Draw grid lines.
    if ( ( zoom >= 3 ) && ( showGridLines ) )
    {
        painter.setPen( palette().foreground().color() );
        for ( int i = 0; i <= image.width(); ++i )
            painter.drawLine( zoom * i, 0,
                              zoom * i, zoom * image.height() );
        for ( int j = 0; j <= image.height(); ++j )
            painter.drawLine( 0, zoom * j,
                              zoom * image.width(), zoom * j );
    }
}

/*
void P2BitmapEditor::drawImagePixel( QPainter *painter, int i, int j )
{
    QColor color;
    QRgb rgb = image.pixel( i, j );

    if ( qAlpha( rgb ) == 0 )
        color = palette().base().color();
    else
        color.setRgb( rgb );

    if ( ( zoom >= 3 ) && ( showGridLines ) )
        painter->fillRect( zoom * i + 1, zoom * j + 1,
                          zoom - 1, zoom - 1, color );
    else
        painter->fillRect( zoom * i, zoom * j,
                          zoom, zoom, color );
}
*/

void P2BitmapEditor::setImagePixel( const QPoint &pos )
{
    if ( drawMode != NODRAW )
    {
        int i = pos.x() / zoom;
        int j = pos.y() / zoom;

        if ( image.rect().contains( i, j ) )
        {
            if ( drawMode == DRAW )
                image.setPixel( i, j, penColor().rgb() );
            else
                image.setPixel( i, j, qRgba( 0, 0, 0, 0 ) );

            //QPainter painter( this );
            //drawImagePixel( &painter, i, j );
            update();
        }
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
