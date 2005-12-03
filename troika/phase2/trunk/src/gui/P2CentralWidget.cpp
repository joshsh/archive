#include "P2CentralWidget.h"  // QWidget, P2CentralWidget
#include "P2Binder.h"  // P2Binder

    #include "ToggleWidget.h"
    #include <QCheckBox>
    #include <iostream.h>

#include <QPalette>  // QColorGroup, QPalette
#include <QColor>  // QColor

    #include <QPainter> // QPainter
    #include <QImage> // QImage

//Added by qt3to4:
#include <QPaintEvent>


///////////////////////////////////////////////////////////////////////////////


/** All child widgets inherit this background color, rather than defining their
    own. */
#define BACKGROUND_COLOR        255, 255, 255


P2CentralWidget::P2CentralWidget( QWidget *parent )
        : QWidget( parent )
{
    setPalette( QPalette( QColor( BACKGROUND_COLOR ) ) );

    //setMinimumSize( 240, 320 );
    //setMaximumSize( 240, 320 );

    // Top-level binder.
    P2Binder *b = new P2Binder( this );

    P2Binder *b2;
    ToggleWidget *tw;

    for ( int i = 0; i < 3; i++)
    {
        b2 = new P2Binder( b );
        b2->addWidget( new ToggleWidget( b2 ) );
        b->addWidget( b2 );
    }

    //QCheckBox *cbox = new QCheckBox( this );
    //b->addWidget( new P2Binder( b, cbox ) );
}


/* XPM *//*
const char * light_off_xpm[] = {
"16 16 4 1",
"       c None",
".      c #000000000000",
"X      c #6B6B6C6C6C6C",
"o      c #FFFF6C6C0000",
"                ",
"                ",
"       ...      ",
"      .   .     ",
"     .    X.    ",
"    .      X.   ",
"    .    XXX.   ",
"    .   X XX.   ",
"     .   XX.    ",
"     .  XXX.    ",
"      .  X.     ",
"      .  X..    ",
"      .ooo..    ",
"      .ooo..    ",
"       .o..     ",
"        ..      "};


// Fill this with a real draw procedure...
void P2CentralWidget::paintEvent( QPaintEvent * )
{
    // No need for painter.begin() or painter.end() when using this constructor.
    QPainter painter( this );

    QImage image( light_off_xpm );
    painter.drawImage( 5, 50,  // dest. origin
        image,
        0, 0,  // source origin
        -1, -1,  // source maxima
        0 );  // conversionFlags
}

*/








