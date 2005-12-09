#include "P2BitmapEditor.h"


////////////////////////////////////////////////////////////////////////////////


//P2BitmapEditor::P2BitmapEditor( QWidget* parent, const char* name,
//    QImage *image0 = 0, int zoom0 = 1, bool showGridLines0 = false)
P2BitmapEditor::P2BitmapEditor( QWidget* parent,
    QImage *image0, int zoom0, bool showGridLines0 )
        : QWidget( parent, 0 ) //WStaticContents )
{
    if ( image0 )
        image = *image0;
    else
    {
        // 16x16 dimensions, 32-bit depth.
        image = QImage( QSize( 11, 11 ), QImage::Format_RGB32 );
        //image.create( 11, 11, 32 );

        // R, G, B and alpha color components.
        image.fill( QColor( 0xFF, 0xFF, 0xFF, 0xFF ).rgb() );//.rgb() );
        //image.fill( qRgba( 0, 0, 0, 0 ) );

        // Enable transparency.
        //image.setAlphaBuffer( true );
    }

    setZoomFactor( zoom0 );
    showGridLines = showGridLines0;

    //setSizePolicy( QSizePolicy::Minimum ); //, QSizePolicy::Minimum );
    curColor = Qt::black;
    zoom = 5;

    setFixedSize( sizeHint() );
}


/*
P2BitmapEditor::~P2BitmapEditor()
{
    //...
}
*/


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
        setImagePixel( event->pos(), true );
    else if ( event->button() == Qt::RightButton )
        setImagePixel( event->pos(), false );
}


void P2BitmapEditor::mouseMoveEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton )
    //if ( event->state() & Qt::LeftButton )
        setImagePixel( event->pos(), true );
    else if ( event->button() == Qt::RightButton )
    //else if ( event->state() & Qt::RightButton )
        setImagePixel( event->pos(), false );
}


void P2BitmapEditor::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );

    for ( int i = 0; i < image.width(); ++i )
        for ( int j = 0; j < image.height(); ++j )
            drawImagePixel( &painter, i, j );

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


void P2BitmapEditor::setImagePixel( const QPoint &pos, bool opaque )
{
    int i = pos.x() / zoom;
    int j = pos.y() / zoom;

    if ( image.rect().contains( i, j ) )
    {
        if ( opaque )
            image.setPixel( i, j, penColor().rgb() );
        else
            image.setPixel( i, j, qRgba( 0, 0, 0, 0 ) );

        QPainter painter( this );
        drawImagePixel( &painter, i, j );
    }
}

