#include "P2CentralWidget.h"
#include "P2Binder.h"

    #include "ToggleWidget.h"
    #include "P2BitmapEditor.h"

///////////////////////////////////////////////////////////////////////////////


P2CentralWidget::P2CentralWidget( QWidget *parent, P2Environment *e )
        : P2BasicWidget( parent, e )
{
    #ifdef DEBUG
        cout << "P2CentralWidget[" << (int) this << "]::P2CentralWidget( "
             << (int) parent << ", "
             << (int) e << " )" << endl;
    #endif

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
    P2Binder *b = new P2Binder( this, environment );
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

    b2 = new P2Binder();
    b->addDependentWidget( b2, p );

    //QCheckBox *cbox = new QCheckBox( this );
    //b->addWidget( new P2Binder( b, cbox ) );

/*
    P2BitmapEditor editor( this, 0, 8, true );
    editor.setGeometry(
        QRect( editor.geometry().topLeft() + QPoint( 60, 60 ),
        editor.geometry().size() ) );
//*/
}


bool P2CentralWidget::handleMousePressEvent( QMouseEvent *event )
{
    return true;
}

