#ifndef P2BitmapEditor_h
#define P2BitmapEditor_h

////////////////////////////////////////////////////////////////////////////////

#include <widgets/P2BasicWidget.h>

////////////////////////////////////////////////////////////////////////////////

class P2BitmapEditor : public P2BasicWidget
//class P2BitmapEditor : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QColor penColor READ penColor WRITE setPenColor )
    Q_PROPERTY( QImage iconImage READ iconImage WRITE setIconImage )
    Q_PROPERTY( int zoomFactor READ zoomFactor WRITE setZoomFactor )
    Q_PROPERTY( bool showGridLines READ getShowGridLines WRITE setShowGridLines )

public:

    P2BitmapEditor( QImage *image0 = 0, int zoom0 = 1, bool showGridLines0 = false );
    //P2BitmapEditor( QWidget *parent = 0,
    //    QImage *image0 = 0, int zoom0 = 1, bool showGridLines0 = false );
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

    //void drawImagePixel( QPainter *painter, int i, int j );
    void setImagePixel( const QPoint &pos );

    QColor curColor;
    QImage image;
    int zoom;
    bool showGridLines;

    enum DrawMode { DRAW, ERASE, NODRAW };

    DrawMode drawMode;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2BitmapEditor_h


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
