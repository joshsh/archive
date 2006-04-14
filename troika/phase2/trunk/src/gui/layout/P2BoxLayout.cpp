#include "P2BoxLayout.h"


////////////////////////////////////////////////////////////////////////////////


P2BoxLayout::P2BoxLayout( QBoxLayout::Direction dir, QWidget *parent )
    : QBoxLayout( dir, parent ), P2Layout()
{
    // Frames may be (programmatically) resized, but may not be smaller than
    // minimumSize.
    setSizeConstraint( QLayout::SetDefaultConstraint );

    // Minimum distance of 1 pixel between child widgets.
    setSpacing( FRAME__CONTENTS__SPACING );

    // Border padding of 2 pixels around content rectangle.
    setMargin( FRAME__CONTENTS__SPACING + FRAME__CONTENTS__PADDING );


    // The initially empty layout has a minimal content rectangle.
    setContentOffset( QPoint( 0, 0 ) );
}


////////////////////////////////////////////////////////////////////////////////


void P2BoxLayout::add( P2Widget *widget )
{
    P2Layout::addWidget( widget );

    QBoxLayout::addWidget( widget );

    connect( widget, SIGNAL( resized( QResizeEvent* ) ),
             this, SLOT( childResizeEvent( QResizeEvent* ) ) );
}


// Size geometry ///////////////////////////////////////////////////////////////


void P2BoxLayout::setContentOffset( const QPoint &offset )
{
    QPoint newOffset = offset;

    if ( newOffset.x() < margin() )
        newOffset.setX( margin() );
    if ( newOffset.y() < margin() )
        newOffset.setY( margin() );

    if ( newOffset != contentOffset )
    {
        contentOffset = newOffset;
        //adjustGeometry();
    }
}


void P2BoxLayout::setMinimumSize( const QSize &size )
{
    if ( size != receivedMinimumSize )
    {
        receivedMinimumSize = size;
        //adjustGeometry();
    }
}


Qt::Orientations P2BoxLayout::expandingDirections() const
{
    // Really, P2BoxLayouts will not be resizable.
    return Qt::Horizontal | Qt::Vertical;
}


bool P2BoxLayout::hasHeightForWidth() const
{
    // P2BoxLayouts are rigid, with no wrapping behavior.
    return false;
}


// Coordination ////////////////////////////////////////////////////////////////


void P2BoxLayout::childResizeEvent( QResizeEvent *event )
{
    //adjustGeometry();

    /* Tell the QBoxLayout to update its sizeHint. */
    update();

    emit resized();
}


/*
void P2BoxLayout::adjustGeometry()
{
    //...
}
*/


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
