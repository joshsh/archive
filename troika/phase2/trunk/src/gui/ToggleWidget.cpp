#include "ToggleWidget.h"  // ToggleWidget

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


ToggleWidget::ToggleWidget()
        : P2BasicWidget()
{
    setFixedSize( 10, 11 );

    imageList = new QList<QImage>();
    imageList->append( QImage( sk_s_xpm ) );
    imageList->append( QImage( sk_k_xpm ) );
    imageList->append( QImage( sk_i_xpm ) );

    toggle = 0;
}



// Note: the ButtonState is not used for now, as
// LeftButton, RightButton, MidButton and NoButton are all to do the same thing.
bool ToggleWidget::handleMousePressEvent( QMouseEvent *event )
{
    toggle = ( toggle + 1 ) % 3;

    update();

    return false;
}



void ToggleWidget::paintEvent( QPaintEvent *event )
{
    //erase( );

    QPainter painter( this );
    painter.drawImage(
        0, 0,  // dest. origin
        imageList->at( toggle ),
        0, 0,  // source origin
        -1, -1,  // source maxima
        0 );  // conversionFlags
}


