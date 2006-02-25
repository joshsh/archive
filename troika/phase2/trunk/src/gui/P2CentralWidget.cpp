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


P2CentralWidget::P2CentralWidget( const P2Environment &env )
    : P2Widget()
{
    #ifdef DEBUG
        cout << indent()
             << "P2CentralWidget[" << (int) this << "]::P2CentralWidget()"
             << endl;
    #endif

    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );

    // Top-level binder.
    P2Frame *b = centralFrame = new P2Frame( env ), *b2, *b3;
    b->setParent( this );
    b->setObjectName( "Top-level P2Frame" );

    QPoint p( 0, 0 );

//*
    P2Frame *togglers = new P2Frame( env );
    togglers->setObjectName( "ToggleWidgets" );

    // ( Really, togglers should be fully constructed before it is added )
    b->addChild( togglers, QPoint( 0, 30 ) );

    ToggleWidget *tw;

    //QPoint p( -55, 0 );

    for ( int i = 0; i < 3; i++)
    {
        b2 = new P2Frame( env );
        tw = new ToggleWidget();
        b2->addChild( tw, QPoint( 30, 0 ) );
        togglers->addChild( b2, p );
        //p = p + QPoint( 14, 15 );
    }

    // Empty binders are tolerated.
    b2 = new P2Frame( env );
    b->addChild( b2, p );

    b2 = new P2Frame( env );
    P2CheckBox *cbox = new P2CheckBox();
    b2->addChild( cbox, QPoint( 5, 0 ) );
    b->addChild( b2, p );

    P2Image *image = new P2Image( troika_xpm );
    b2 = new P2Frame( env );
    b2->addChild( image, QPoint( 30, 10 ) );
    b->addChild( b2, p );

    image = new P2Image( troika_icon_big_xpm );
    b2 = new P2Frame( env );
    b2->addChild( image, QPoint( 0, 0 ) );
    b->addChild( b2, QPoint( 0, 130 ) );

/*
    P2TextEdit *text = new P2TextEdit( "Testing<FONT COLOR='#FF0000'>...</FONT>", false );
    b2 = new P2Frame( env );
    b2->addChild( text, QPoint( 0, 0 ) );
    b->addChild( b2, p );
//*/

    P2Text *t = new P2Text( "Bleah.", 0 );
    b2 = new P2Frame( env );
    b2->addChild( t, QPoint( 0, 0 ) );
    b->addChild( b2, QPoint( 10, 0 ) );

    P2BitmapEditor *editor = new P2BitmapEditor( 0, 8, false );
    editor->setGeometry(
        QRect( editor->geometry().topLeft() + QPoint( 60, 60 ),
        editor->geometry().size() ) );
    b2 = new P2Frame( env );
    b2->addChild( editor, QPoint( 0, 0 ) );
    b3 = new P2Frame( env );
    b3->addChild( b2, QPoint( 0, 0 ) );
    b->addChild( b3, QPoint( 0, 100 ) );
    b3->setObjectName( "P2BitmapEditor" );

    ResizeWidget *rs = new ResizeWidget();
    b->addChild( rs, QPoint( 0, 20 ) );

    // Resizable widgets.
    P2Frame *resizers = new P2Frame( env );
    for ( int i = 0; i < 4; i++)
    {
        rs = new ResizeWidget();
        b2 = new P2Frame( env );
        b2->addChild( rs, QPoint( 10, 0 ) );
        resizers->addChild( b2, QPoint( 0, 0 ) );
    }
    b->addChild( resizers, QPoint( 0, 30 ) );
    resizers->setObjectName( "ResizeWidgets" );

    centralFrame->setPosition(
        QPoint( WINDOW__CONTENTS__PADDING, WINDOW__CONTENTS__PADDING ) );
    connect( centralFrame, SIGNAL( resized( QResizeEvent* ) ),
             this, SLOT( childResizeEvent( QResizeEvent* ) ) );

    centralFrame->setFocus( centralFrame );
    refresh( env );
}


P2Frame *P2CentralWidget::focusFrame()
{
    return centralFrame->focusFrame();
}


const QString P2CentralWidget::className()
{
    return QString( "P2CentralWidget" );
}


void P2CentralWidget::refresh( const P2Environment &env )
{
    centralFrame->refresh( env );
}


void P2CentralWidget::childResizeEvent( QResizeEvent *event )
{
    resize( centralFrame->sizeHint()
        + QSize( 2 * WINDOW__CONTENTS__PADDING, 2 * WINDOW__CONTENTS__PADDING ) );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on