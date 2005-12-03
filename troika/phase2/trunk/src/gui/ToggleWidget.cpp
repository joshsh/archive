#include "ToggleWidget.h"  // ToggleWidget

#include <qwidget.h>  // QWidget

#include <qpainter.h>  // QPainter
#include <qimage.h>  // QImage

#include <qevent.h>  // QMouseEvent

#include <qlist.h>  // QList
//Added by qt3to4:
#include <QMouseEvent>
#include <QPaintEvent>

    //#include <iostream.h>

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

QList<QImage> *imageList = new QList<QImage>();

ToggleWidget::ToggleWidget( QWidget* parent )
        : QWidget( parent, 0 )
{
    setFixedSize( 10, 11 );
/*
    imageList->append( new QImage( sk_s_xpm ) );
    imageList->append( new QImage( sk_k_xpm ) );
    imageList->append( new QImage( sk_i_xpm ) );
*/
    imageList->append( QImage( sk_s_xpm ) );
    imageList->append( QImage( sk_k_xpm ) );
    imageList->append( QImage( sk_i_xpm ) );

    toggle = 0;
}



// Note: the ButtonState is not used for now, as
// LeftButton, RightButton, MidButton and NoButton are all to do the same thing.
void ToggleWidget::mousePressEvent( QMouseEvent *event )
{
    toggle = ( toggle + 1 ) % 3;

//setGeometry(QRect(QPoint(geometry().topLeft()), QSize(geometry().size()) + QSize(5, 5)));
    paintEvent( 0 );
}



void ToggleWidget::paintEvent( QPaintEvent *event )
{
    //erase( );
    QSize size = frameSize();
    //cout << "width = " << size.width() << ", height = " << size.height() << "." << endl;

    QPainter painter( this );
    painter.drawImage(
        0, 0,  // dest. origin
        imageList->at( toggle ),
        0, 0,  // source origin
        -1, -1,  // source maxima
        0 );  // conversionFlags
}


