#include "P2BitmapEditor.h"  // P2BitmapEditor, ...

//#include <qwidget.h>
#include <qnamespace.h>
#include <qpainter.h>  // QPainter

////////////////////////////////////////////////////////////////////////////////

/*
P2BitmapEditor::P2BitmapEditor( QWidget* parent, const char* name )
        : QWidget( parent, name, 0 ) //WStaticContents )
{
    //setSizePolicy( QSizePolicy::Minimum ); //, QSizePolicy::Minimum );
    curColor = black;
    zoom = 5;

    // 16x16 dimensions, 32-bit depth.
    image.create( 11, 11, 32 );

    // R, G, B and alpha color components.
    image.fill( qRgba( 0, 0, 0, 0 ) );

    // Enable transparency.
    image.setAlphaBuffer( true );

    setFixedSize( sizeHint() );
}
*/


//P2BitmapEditor::P2BitmapEditor( QWidget* parent, const char* name,
//    QImage *image0 = 0, int zoom0 = 1, bool showGridLines0 = false)
P2BitmapEditor::P2BitmapEditor( QWidget* parent, const char* name,
    QImage *image0, int zoom0, bool showGridLines0 )
        : QWidget( parent, name, 0 ) //WStaticContents )
{
    if ( image0 )
        image = *image0;
    else
    {
        // 16x16 dimensions, 32-bit depth.
        image.create( 11, 11, 32 );
        // R, G, B and alpha color components.
        image.fill( qRgba( 0, 0, 0, 0 ) );
        // Enable transparency.
        image.setAlphaBuffer( true );
    }

    setZoomFactor( zoom0 );
    showGridLines = showGridLines0;

    //setSizePolicy( QSizePolicy::Minimum ); //, QSizePolicy::Minimum );
    curColor = black;
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
        image = newImage.convertDepth( 32 );
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
    if ( event->button() == LeftButton )
        setImagePixel( event->pos(), true );
    else if ( event->button() == RightButton )
        setImagePixel( event->pos(), false );
}



void P2BitmapEditor::mouseMoveEvent( QMouseEvent *event )
{
    if ( event->state() & LeftButton )
        setImagePixel( event->pos(), true );
    else if ( event->state() & RightButton )
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
        painter.setPen( colorGroup().foreground() );
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
        color = colorGroup().base();
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


