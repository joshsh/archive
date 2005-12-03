#include "P2Binder.h"
#include "P2Layout.h"
//#include "ToggleWidget.h"

#include <QtGui>
//    #include <QCheckBox>

////////////////////////////////////////////////////////////////////////////////

//! Temporary.
QWidget *special;


P2Binder::P2Binder( QWidget* parent )
    : QWidget( parent, 0 )
{
    initialize();
}


void P2Binder::addWidget( QWidget *widget )
{
    (( P2Layout* ) layout())->addWidget( widget );
}


// Note: event object is not used.
void P2Binder::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );

    // Eventually, borders and padding will be outside of the P2Binder, handled
    // by the P2Layout instead.
    painter.setPen( Qt::blue );
    QRect borderRect( geometry().topLeft(), geometry().size() - QSize( 1, 1) );
    painter.drawRect( borderRect );

    //int minx = geometry().topLeft().x();
    //int miny = geometry().topLeft().y();
    //int maxx = geometry().bottomRight().x();
    //int maxy = geometry().bottomRight().y();
    //QRect r(minx, miny, maxx - minx, maxy - miny - 1 );
    //painter.setPen( Qt::black );
    //painter.drawRect( r );

    //painter.setPen( Qt::red );
    //painter.drawRect( ( ( P2Layout* ) layout() )->boundingRectangle() );

    //painter.setPen( Qt::blue );
    //painter.drawRect( special->geometry() );
}


bool P2Binder::acceptDrops () const
{
    return true;
}


void P2Binder::initialize()
{
    // Note: no need to call setLayout after using this constructor.
    P2Layout *layout = new P2Layout( this );

    //resize( layout->calculateSize() );

    //layout->setGeometry( QRect( 0, 0, 300, 300 ) );
}


void P2Binder::resize( QSize newSize )
{
    QSize oldSize = geometry().size();
    QPoint center = geometry().center();
    setGeometry(
        center.x() - newSize.width()/2,
        center.y() - newSize.height()/2,
        newSize.width(),
        newSize.height() );
}

