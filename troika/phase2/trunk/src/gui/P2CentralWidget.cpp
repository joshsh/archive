#include "P2CentralWidget.h"  // P2CentralWidget
#include "P2Binder.h"  // P2Binder
#include "P2BitmapEditor.h"  // P2BitmapEditor

#include <qapplication.h>  // qApp

#include <qwidget.h>  // QWidget
#include <qpushbutton.h>  // QPushButton

#include <qpalette.h>  // QColorGroup, QPalette
#include <qcolor.h>  // QColor

    #include <qpainter.h> // QPainter
    #include <qstring.h> // QString

    #include <qimage.h> // QImage
    #include <qpoint.h> // QPoint

    #include <qbutton.h>  // QButton
    #include <qpixmap.h>  // QPixmap

    //#include <qtextedit.h>  ! Apparently not supported...

    #include <qurloperator.h>
    #include <qnetworkprotocol.h>


///////////////////////////////////////////////////////////////////////////////


/* XPM */
static const char * ohio_xpm[] = {
"28 7 2 1",
"       c None",
".      c #000000000000",
" ....   ..  ..  ....   .... ",
"..  ..  ..  ..   ..   ..  ..",
"..  ..  ..  ..   ..   ..  ..",
"..  ..  ......   ..   ..  ..",
"..  ..  ..  ..   ..   ..  ..",
"..  ..  ..  ..   ..   ..  ..",
" ....   ..  ..  ....   .... "};

/* XPM */
static const char * sk_s_xpm[] = {
"10 11 2 1",
"       c None",
".      c #000000000000",
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
".      c #000000000000",
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
".      c #000000000000",
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


/* XPM */
static const char * troika_xpm[] = {
"41 7 2 1",
"       c None",
".      c #00000000FFFF",
"....  ....    ....   ....  ..  ..   .... ",
" ..   .. ..  ..  ..   ..   ..  ..  ..  ..",
" ..   .. ..  ..  ..   ..   .. ..   ..  ..",
" ..   ....   ..  ..   ..   ....    ......",
" ..   .. ..  ..  ..   ..   .. ..   ..  ..",
" ..   .. ..  ..  ..   ..   ..  ..  ..  ..",
" ..   .. ..   ....   ....  ..  ..  ..  .."};


#define BACKGROUND_COLOR        255, 255, 255


P2CentralWidget::P2CentralWidget( QWidget *parent, const char *name )
        : QWidget( parent, name )
{
    setPalette( QPalette( QColor(BACKGROUND_COLOR) ) );

/*
    QColorGroup cg = this->colorGroup();    // if this is a widget.
    cg.setColor(QColorGroup::Background, Qt::white);
    // But how do you re-define QWidget::PaletteBackground?
    setBackgroundMode(QWidget::PaletteBackground);
*/
    //setMinimumSize( 240, 320 );
    //setMaximumSize( 240, 320 );

    // This my button.
    QPushButton *bleah = new QPushButton( "Hide", this, "bleah" );
    bleah->setGeometry( 30, 10, 75, 30 );
    bleah->setFont( QFont( "Times", 18, QFont::Bold ));

    // "The widget name has nothing to do with the button text; it just happens
    // to be similar in this case."
    QPushButton *quit = new QPushButton( "Quit", this, "quit" );
    quit->setGeometry( 30, 40, 75, 30 );
    quit->setFont( QFont( "Times", 18, QFont::Bold ));

    //QTextEdit *editor = new QTextEdit(this, "editor");
    //editor->setGeometry( 20, 70, 50, 50 );

    // "Because the MyWidget class doesn't know about the application object, it
    // has to connect to Qt's pointer to it, qApp."
    connect( quit, SIGNAL( clicked( )), qApp, SLOT( quit( )));

    connect( bleah, SIGNAL( clicked( )), quit, SLOT (hide( )));
    //connect( bleah, SIGNAL(clicked()), (P2CentralWidget *) this, SLOT(foobar()) );

    P2Binder *binder = new P2Binder( this, "test binder" );
    binder->move( 5, 20 );

    QImage *image = new QImage( );
    // 16x16 dimensions, 32-bit depth.
    image->create( 11, 11, 32 );
    // R, G, B and alpha color components.
    image->fill( qRgba( 0, 0, 0, 0 ));
    // Enable transparency.
    image->setAlphaBuffer( true );
    P2BitmapEditor *editor = new P2BitmapEditor( this, "editor", image, 5, true );
    //P2BitmapEditor *editor = new P2BitmapEditor( this, "editor" );
    editor->move( 110, 10 );


/*
    // Ohio as a QPixmap (below, it's a QImage).
    QButton ohio( this, "Button", 0 );
    //const QPixmap ohio_pix(ohio_xpm);
    //ohio.setPixmap(ohio_pix);
    ohio.setGeometry( 100, 60, 30, 30 );
    ohio.setFont( QFont( "Times", 18, QFont::Bold ) );
    ohio.setText( QString("foobar") );
*/


}


int delayX = 5;

// Fill this with a real draw procedure...
void P2CentralWidget::paintEvent( QPaintEvent * )
{
    QPainter painter( this );
    //painter.begin(&anyPaintDevice);

    painter.setBrush( Qt::blue );
    //QBrush brush( Qt::blue );
    //painter.setBrush( brush );
    painter.setPen( NoPen );
    painter.drawRect ( 102, 152, 10, 10 );
    painter.setBrush( NoBrush );
    painter.setPen( Qt::red );
    painter.drawRect ( 100, 150, 14, 14 );

    //painter.drawText( rect(), AlignCenter, "The Text" );

    QString s = "The answer = " + QString::number( 42 );
    painter.drawText( 100, 200, s );

    painter.setPen( Qt::black );
    painter.drawPoint( 20, 20 );
    painter.drawPoint( 23, 20 );
    painter.drawPoint( 20, 23 );
    painter.drawPoint( 23, 23 );

/* XPM */
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



QImage image( light_off_xpm );
painter.drawImage( 50, 180,  // dest. origin
            image,
            0, 0,  // source origin
            -1, -1,  // source maxima
            0 );  // conversionFlags

QImage image2( ohio_xpm );
painter.drawImage( 70, 260,  // dest. origin
            image2,
            0, 0,  // source origin
            -1, -1,  // source maxima
            0 );  // conversionFlags

QImage image2a( troika_xpm );
painter.drawImage( 50, 240,  // dest. origin
            image2a,
            0, 0,  // source origin
            -1, -1,  // source maxima
            0 );  // conversionFlags

QImage image3(sk_s_xpm);
painter.drawImage( 5, 100,  // dest. origin
            image3,
            0, 0,  // source origin
            -1, -1,  // source maxima
            0 );  // conversionFlags

QImage image4( sk_k_xpm );
painter.drawImage( 5, 120,  // dest. origin
            image4,
            0, 0,  // source origin
            -1, -1,  // source maxima
            0 );  // conversionFlags

QImage image5( sk_i_xpm );
painter.drawImage( 5, 140,  // dest. origin
            image5,
            0, 0,  // source origin
            -1, -1,  // source maxima
            0 );  // conversionFlags

    //~
    #ifndef ARM_COMPILE
    QUrlOperator op( "ftp://ftp.is.co.za/rfc/" );
    // do some other stuff like op.listChildren() or op.mkdir( "new Dir" )
    const QNetworkOperation *no = op.get( "rfc1808.txt" );
    int state = no ? no->state( ) : -1;
    QString s2 = "State = " + QString::number( state );
    //cout << no->arg(0).latin1();
    painter.setBrush( NoBrush );
    painter.setPen( Qt::red );
    painter.drawText( 20, 220, s2 );
    #endif

/*
    // ...look at this again.
    for ( int i = 0; i < 1000000000; i++ )
    {
        i++;
        i--;
    }
    painter.setPen( Qt::black );
    painter.setBrush( NoBrush );
    painter.drawRect ( delayX, 5, 4, 4 );
    delayX += 5;
    //painter.drawPoint( delayX, 5 );
//*/


    //painter.end( );
}










