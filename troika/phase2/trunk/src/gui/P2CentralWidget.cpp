#include "P2CentralWidget.h"
#include "P2Frame.h"

    #include "binders/ToggleWidget.h"
    #include "binders/P2BitmapEditor.h"
    #include "binders/P2CheckBox.h"

///////////////////////////////////////////////////////////////////////////////


P2CentralWidget::P2CentralWidget( )
    : P2BasicWidget()
{
    #ifdef DEBUG
        cout << "P2CentralWidget[" << (int) this << "]::P2CentralWidget()"
             << endl;
    #endif

    // Top-level binder.
    P2Frame *b = new P2Frame();
    b->setParent( this );
    b->setPosition( QPoint( 5, 5 ) );

    P2Frame *togglers = new P2Frame();

    // ( Really, togglers should be fully constructed before it is added )
    b->addChild( togglers, QPoint( 0, 0 ) );

    P2Frame *b2;
    ToggleWidget *tw;

    QPoint p( 0, 0 );
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

    P2BitmapEditor *editor = new P2BitmapEditor( 0, 8, true );
    editor->setGeometry(
        QRect( editor->geometry().topLeft() + QPoint( 60, 60 ),
        editor->geometry().size() ) );
    b2 = new P2Frame();
    b2->addChild( editor, QPoint( 0, 0 ) );
    b->addChild( b2, p );

    QTextEdit *text = new QTextEdit( "Testing...", this );
    text->setGeometry( text->geometry().translated( QPoint ( 0, 140 ) ) );
    text->setReadOnly( true );

    // QMultiLineEdit *mle = new QMultiLineEdit( this );
    // mle->setText( "Salut!" );
    // setCentralWidget( mle );
}


bool P2CentralWidget::handleMousePressEvent( QMouseEvent *event, bool childIsBinder )
{
    return true;
}

