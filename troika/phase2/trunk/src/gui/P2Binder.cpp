#include "P2Binder.h"  // P2Binder

#include <qwidget.h>  // QWidget

#include <qpainter.h>  // QPainter
#include <qimage.h>  // QImage

#include <qevent.h>  // QMouseEvent

////////////////////////////////////////////////////////////////////////////////

/* XPM */
static const char * sk_s_xpm[] = {
"10 11 2 1",
"       c None",
".      c #FFFF80000000",
"..........",
".        .",
".  ....  .",
". ..  .. .",
". ..     .",
".  ....  .",
".     .. .",
". ..  .. .",
".  ....  .",
".        .",
".........."};

/* XPM */
static const char * sk_k_xpm[] = {
"10 11 2 1",
"       c None",
".      c #FFFF80000000",
"..........",
".        .",
". ..  .. .",
". ..  .. .",
". .. ..  .",
". ....   .",
". .. ..  .",
". ..  .. .",
". ..  .. .",
".        .",
".........."};

/* XPM */
static const char * sk_i_xpm[] = {
"10 11 2 1",
"       c None",
".      c #FFFF80000000",
"..........",
".        .",
".  ....  .",
".   ..   .",
".   ..   .",
".   ..   .",
".   ..   .",
".   ..   .",
".  ....  .",
".        .",
".........."};

int toggle;
QImage **images = new QImage*[ 3 ];

P2Binder::P2Binder( QWidget* parent, const char* name )
        : QWidget( parent, name )
{
    setFixedSize( 10, 11 );

    images[ 0 ] = new QImage( sk_s_xpm );
    images[ 1 ] = new QImage( sk_k_xpm );
    images[ 2 ] = new QImage( sk_i_xpm );

    toggle = 0;
}



// Note: the ButtonState is not used for now, as
// LeftButton, RightButton, MidButton and NoButton are all to do the same thing.
void P2Binder::mousePressEvent( QMouseEvent *event )
{
    toggle = ( toggle + 1 ) % 3;

    paintEvent( 0 );
}



void P2Binder::paintEvent( QPaintEvent *event )
{
    erase( );

    QPainter painter( this );

    painter.drawImage(
        0, 0,  // dest. origin
        *( images[toggle]),
        0, 0,  // source origin
        -1, -1,  // source maxima
        0 );  // conversionFlags
}


