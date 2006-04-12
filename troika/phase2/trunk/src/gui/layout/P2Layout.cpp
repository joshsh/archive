#include "P2Layout.h"


////////////////////////////////////////////////////////////////////////////////


P2Layout::P2Layout( QWidget *parent )
    : QLayout( parent )
{
    // Frames may be (programmatically) resized, but may not be smaller than
    // minimumSize.
    setSizeConstraint( QLayout::SetDefaultConstraint );

    // Minimum distance of 1 pixel between child widgets.
    setSpacing( FRAME__CONTENTS__SPACING );

    // Border padding of 2 pixels around content rectangle.
    setMargin( FRAME__CONTENTS__SPACING + FRAME__CONTENTS__PADDING );

    receivedMinimumSize = QSize( 0, 0 );
}


P2Layout::~P2Layout()
{
    // Delete all layout items.
    for ( int i = 0; i < children.size(); i++ )
        delete children.at( i );
}


////////////////////////////////////////////////////////////////////////////////


void P2Layout::addItem( QLayoutItem *item )
{
    children.append( item );

    adjustGeometry();
}


void P2Layout::addWidget( P2Widget *widget )
{
    addItem( new QWidgetItem( widget ) );

    connect( widget, SIGNAL( resized( QResizeEvent* ) ),
             this, SLOT( childResizeEvent( QResizeEvent* ) ) );
}


int P2Layout::count() const
{
    return children.size();
}


QLayoutItem *P2Layout::itemAt( int index ) const
{
    return children.value( index );
}


QLayoutItem *P2Layout::takeAt( int index )
{
    if ( index >= 0 && index < children.size() )
        return children.takeAt( index );
    else
        return 0;
}


void P2Layout::refreshChildren( const P2Environment &env )
{
    // Propagate the signal to all children.
    for ( int i = 0; i < children.size(); i++ )
    {
        P2Widget *child = ( P2Widget* ) children.at( i )->widget();
        child->refresh( env );
    }
}


void P2Layout::showChildren() const
{
    for ( int i = 0; i < children.size(); i++ )
    {
        cout << indent()
             << i << ":\t";

        P2Widget *child = ( P2Widget* ) children.at( i )->widget();
        cout << child->className().toStdString() << "\t";
        if ( child->objectName() != 0 )
            cout << child->objectName().toStdString();

        cout << endl;
    }
}


////////////////////////////////////////////////////////////////////////////////


void P2Layout::setGeometry( const QRect &rect )
{
    QLayout::setGeometry( rect );

    //...
}


void P2Layout::childResizeEvent( QResizeEvent *event )
{
    adjustGeometry();
}


QSize P2Layout::minimumSize() const
{
    return cachedSizeHint;
}


QSize P2Layout::sizeHint() const
{
    return cachedSizeHint;
}


void P2Layout::setContentOffset( const QPoint &offset )
{
    QPoint newOffset = offset;

    if ( newOffset.x() < margin() )
        newOffset.setX( margin() );
    if ( newOffset.y() < margin() )
        newOffset.setY( margin() );

    if ( newOffset != contentOffset )
    {
        contentOffset = newOffset;
        adjustGeometry();
    }
}


void P2Layout::setMinimumSize( const QSize &size )
{
    if ( size != receivedMinimumSize )
    {
        receivedMinimumSize = size;
        adjustGeometry();
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
