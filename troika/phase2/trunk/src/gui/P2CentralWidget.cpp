#include "P2CentralWidget.h"
#include "P2Frame.h"

    #include "binders/P2BitmapEditor.h"
    #include "binders/P2CheckBox.h"
    #include "binders/P2Text.h"
    #include "binders/P2TextEdit.h"
    #include "binders/P2Image.h"

    #include "binders/ToggleWidget.h"
    #include "binders/ResizeWidget.h"

static const char * troika_xpm[] = {
"44 7 2 1",
"       c None",
".      c #00000000FFFF",
"......  .....    ....   ....  ..  ..   .... ",
"  ..    ..  ..  ..  ..   ..   ..  ..  ..  ..",
"  ..    ..  ..  ..  ..   ..   .. ..   ..  ..",
"  ..    .....   ..  ..   ..   ....    ......",
"  ..    ..  ..  ..  ..   ..   .. ..   ..  ..",
"  ..    ..  ..  ..  ..   ..   ..  ..  ..  ..",
"  ..    ..  ..   ....   ....  ..  ..  ..  .."};

static const char * troika_icon_xpm[] = {
"5 5 2 1",
"       c None",
".      c #000000000000",
"  .  ",
"     ",
" ... ",
"  .  ",
". . ."};

static const char * troika_icon_big_xpm[] = {
"10 10 2 1",
"       c None",
".      c #000000000000",
"    ..    ",
"    ..    ",
"          ",
"          ",
"  ......  ",
"  ......  ",
"    ..    ",
"    ..    ",
"..  ..  ..",
"..  ..  .."};

///////////////////////////////////////////////////////////////////////////////


P2CentralWidget::P2CentralWidget( int fixedWidth )
    : P2BasicWidget()
{
    #ifdef DEBUG
        cout << "P2CentralWidget[" << (int) this << "]::P2CentralWidget()"
             << endl;
    #endif

    setFixedWidth( fixedWidth );

    // Top-level binder.
    P2Frame *b = new P2Frame(), *b2;
    b->setParent( this );
    b->setPosition( QPoint( 5, 5 ) );

    QPoint p( 0, 0 );

//*
    P2Frame *togglers = new P2Frame();

    // ( Really, togglers should be fully constructed before it is added )
    b->addChild( togglers, QPoint( 0, 0 ) );

    ToggleWidget *tw;

    //QPoint p( -55, 0 );

    for ( int i = 0; i < 3; i++)
    {
        b2 = new P2Frame();
        tw = new ToggleWidget();
        b2->addChild( tw, QPoint( 0, 0 ) );
        togglers->addChild( b2, p );
        //p = p + QPoint( 14, 15 );
    }

    // Empty binders are tolerated.
    b2 = new P2Frame();
    b->addChild( b2, p );

    b2 = new P2Frame();
    P2CheckBox *cbox = new P2CheckBox();
    b2->addChild( cbox, QPoint( 0, 0 ) );
    b->addChild( b2, p );

    P2Image *image = new P2Image( troika_xpm );
    b2 = new P2Frame();
    b2->addChild( image, QPoint( 0, 0 ) );
    b->addChild( b2, p );

    image = new P2Image( troika_icon_big_xpm );
    b2 = new P2Frame();
    b2->addChild( image, QPoint( 0, 0 ) );
    b->addChild( b2, p );
/*
    P2TextEdit *text = new P2TextEdit( "Testing<FONT COLOR='#FF0000'>...</FONT>", false );
    b2 = new P2Frame();
    b2->addChild( text, QPoint( 0, 0 ) );
    b->addChild( b2, p );
//*/

    P2Text *t = new P2Text( "Bleah.", 0 );
    b2 = new P2Frame();
    b2->addChild( t, QPoint( 0, 0 ) );
    b->addChild( b2, QPoint( 0, 0 ) );

    P2BitmapEditor *editor = new P2BitmapEditor( 0, 8, false );
    editor->setGeometry(
        QRect( editor->geometry().topLeft() + QPoint( 60, 60 ),
        editor->geometry().size() ) );
    b2 = new P2Frame();
    b2->addChild( editor, QPoint( 0, 0 ) );
    b->addChild( b2, p );

    ResizeWidget *rs = new ResizeWidget();
    b2 = new P2Frame();
    b2->addChild( rs, QPoint( 0, 0 ) );
    b->addChild( b2, QPoint( 0, 0 ) );

    rs = new ResizeWidget();
    b->addChild( rs, QPoint( 0, 0 ) );

    //QPoint center( this->geometry().width() / 2, this->geometry().height() / 2 );
    //b->setCenter( center );
}

