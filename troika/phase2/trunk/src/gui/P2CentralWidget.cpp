#include "P2CentralWidget.h"
#include "P2Binder.h"

    #include "ToggleWidget.h"
    #include "P2BitmapEditor.h"
    #include "binders/P2CheckBox.h"

///////////////////////////////////////////////////////////////////////////////


P2CentralWidget::P2CentralWidget( P2Environment *e )
//P2CentralWidget::P2CentralWidget( QWidget *parent, P2Environment *e )
//        : P2BasicWidget( parent, e )
        : P2BasicWidget()
{
    #ifdef DEBUG
        cout << "P2CentralWidget[" << (int) this << "]::P2CentralWidget()"
             << endl;
    #endif

    setEnvironment( e );

    setPalette( QPalette(
        palette().foreground(),  // Foreground color.
        palette().button(),  // Button color.
        palette().light(),
        palette().dark(),
        palette().mid(),
        palette().text(),
        palette().brightText(),
        palette().base(),
        QBrush( QColor( BACKGROUND_COLOR ), Qt::SolidPattern ) ) );  // Background color.

    //setPalette( QPalette( QColor( BACKGROUND_COLOR ) ) );

    //setMinimumSize( 240, 320 );
    //setMaximumSize( 240, 320 );

    // Top-level binder.
    P2Binder *b = new P2Binder();
    b->setParent( this );
    b->setEnvironment( environment );
    b->setPosition( QPoint( 5, 5 ) );

    P2Binder *b2;
    ToggleWidget *tw;

    QPoint p( 0, 0 );
    //QPoint p( -55, 0 );

    for ( int i = 0; i < 3; i++)
    {
        b2 = new P2Binder();
        tw = new ToggleWidget();
        b2->addDependentWidget( tw, QPoint( 0, 0 ) );

        b->addDependentWidget( b2, p );

        //p = p + QPoint( 14, 15 );
    }

    // Empty binders are tolerated.
    b2 = new P2Binder();
    b->addDependentWidget( b2, p );

    b2 = new P2Binder();
    P2CheckBox *cbox = new P2CheckBox();
    b2->addDependentWidget( cbox, QPoint( 0, 0 ) );
    b->addDependentWidget( b2, p );


    //QCheckBox *cb = new QCheckBox( this );

    //P2CheckBox *cb = new P2CheckBox();
    //cb->setParent( this );
    //cb->setEnvironment( environment );


    //QCheckBox *cbox = new QCheckBox( this );
    //b->addWidget( new P2Binder( b, cbox ) );

//*
    P2BitmapEditor *editor = new P2BitmapEditor( this, 0, 8, true );
    editor->setGeometry(
        QRect( editor->geometry().topLeft() + QPoint( 60, 60 ),
        editor->geometry().size() ) );
//*/

   QTextEdit *text = new QTextEdit( "Testing...", this );
   text->setGeometry( text->geometry().translated( QPoint ( 0, 140 ) ) );
   text->setReadOnly( true );
}


bool P2CentralWidget::handleMousePressEvent( QMouseEvent *event, bool childIsBinder )
{
    return true;
}

