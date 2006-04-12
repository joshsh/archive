#include "P2BoxLayout.h"


////////////////////////////////////////////////////////////////////////////////


P2BoxLayout::P2BoxLayout( Direction dir, QWidget *parent )
    : QBoxLayout( dir, parent ), P2Layout( parent )
{
    // The initially empty layout has a minimal content rectangle.
    setContentOffset( QPoint( 0, 0 ) );
}


////////////////////////////////////////////////////////////////////////////////

/*
// Warning: this is NOT an efficient way to add multiple items at a time.
void P2BoxLayout::add( P2Widget *widget, const QPoint &position )
{
    // Adjust the position of the new item so it does not collide.
    QPoint adjustedPosition = findBestPosition(
        QRect( position, widget->sizeHint() ) );

    widget->setGeometry( QRect( adjustedPosition, widget->sizeHint() ) );

    addWidget( widget );
}
*/

// Size geometry ///////////////////////////////////////////////////////////////


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


void P2BoxLayout::adjustGeometry()
{
    //...
}




// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
