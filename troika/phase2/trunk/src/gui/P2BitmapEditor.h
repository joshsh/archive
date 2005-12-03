#ifndef P2BitmapEditor_h
#define P2BitmapEditor_h  1

////////////////////////////////////////////////////////////////////////////////

#include "compile-target.h"  // Macros specific to the target architecture.

#include <qwidget.h>  // QWidget
#include <qimage.h>  // QImage, QColor
//Added by qt3to4:
#include <QMouseEvent>
#include <QPaintEvent>
//? // QSize

////////////////////////////////////////////////////////////////////////////////

class P2BitmapEditor : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QColor penColor READ penColor WRITE setPenColor )
    Q_PROPERTY( QImage iconImage READ iconImage WRITE setIconImage )
    Q_PROPERTY( int zoomFactor READ zoomFactor WRITE setZoomFactor )
    Q_PROPERTY( bool showGridLines READ getShowGridLines WRITE setShowGridLines )

public:

    P2BitmapEditor( QWidget *parent = 0, const char *name = 0,
        QImage *image0 = 0, int zoom0 = 1, bool showGridLines0 = false );
    //P2BitmapEditor( QWidget *parent = 0, const char *name = 0 );
    //~P2BitmapEditor();

    void setPenColor( const QColor &newColor );
    QColor penColor() const { return curColor; }
    void setZoomFactor( int newZoom );
    int zoomFactor() const { return zoom; }
    void setIconImage( const QImage &newImage );
    const QImage &iconImage() const { return image; }
    QSize sizeHint() const;
    void setShowGridLines( bool newShowGridLines );
    bool getShowGridLines() const { return showGridLines; }

protected:

    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void paintEvent( QPaintEvent *event );

private:

    void drawImagePixel( QPainter *painter, int i, int j );
    void setImagePixel( const QPoint &pos, bool opaque );

    QColor curColor;
    QImage image;
    int zoom;
    bool showGridLines;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2BitmapEditor_h


