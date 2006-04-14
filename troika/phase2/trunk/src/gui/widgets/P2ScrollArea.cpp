#include <widgets/P2ScrollArea.h>


////////////////////////////////////////////////////////////////////////////////


P2ScrollArea::P2ScrollArea( P2Widget *widget )
    : QScrollArea( 0 )
{
    #ifdef DEBUG
        cout << indent()
             << "P2ScrollArea[" << (int) this << "]::P2ScrollArea("
             << (int) widget << ")" << endl;
    #endif

    setWidget( widget );

    connect( widget, SIGNAL( resized( QResizeEvent* ) ),
             this, SLOT( childResizeEvent( QResizeEvent* ) ) );
}


void P2ScrollArea::childResizeEvent( QResizeEvent *event )
{
    //- Center the child widget in x if it is small enough.
    //- Center the child widget in y if it is small enough.

    resize( widget()->size() );

    //setGeometry( QRect( QPoint( 50, 50 ), size() ) );
    //widget()->setGeometry( QPoint( 50, 50 ), widget()->size() );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
