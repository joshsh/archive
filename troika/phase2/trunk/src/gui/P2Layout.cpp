#include "P2Layout.h"

    #include <iostream.h>


////////////////////////////////////////////////////////////////////////////////


void printGeometry( QWidget *widget )
{
    cout << "[" << (int) widget << "]geometry = ("
         << widget->geometry().x() << ", "
         << widget->geometry().y() << "), "
         << widget->size().width() << ", "
         << widget->size().height() << endl;
}







P2Layout::P2Layout( QWidget *parent )
    : QLayout( parent )
{
    #ifdef DEBUG
        cout << "P2Layout[" << (int) this << "]::P2Layout( "
             << (int) parent << " )" << endl;
    #endif

cout << "on layout creation: ";
printGeometry( parentWidget() );

    // Width-1px border.
    setMargin( 1 );

    // No resizing for now.
    setSizeConstraint( QLayout::SetFixedSize );

    // Minimum distance of 1px between child widgets.
    setSpacing( 1 );

    // Initially empty layout has a minimal content rectangle.
    contentRectangle = QRect( QPoint( 2, 2 ), QSize( 0, 0 ) );
    size = contentRectangle.size() + QSize( 4, 4 );
}


P2Layout::~P2Layout()
{
    QLayoutItem *l;

    // Delete all child widgets.
    while ( ( l = takeAt( 0 ) ) )
        delete l;
}


////////////////////////////////////////////////////////////////////////////////


void P2Layout::addItem( QLayoutItem *item )
{
    children.append( new P2LayoutItem( item ) );

    if ( children.size() == 1 )
        contentRectangle = item->geometry();
    else
        contentRectangle = contentRectangle.unite( item->geometry() );

    size = contentRectangle.size() + QSize( 4, 4 );

    justifyContents();
}


void P2Layout::addWidget( QWidget *widget, QPoint position )
{
    QWidgetItem *item = new QWidgetItem( widget );
    item->setGeometry( QRect( position, item->sizeHint() ) );
    addItem( item );
}


int P2Layout::count() const
{
    return children.size();
}


QLayoutItem *P2Layout::itemAt( int index ) const
{
    P2LayoutItem *p2item = children.value(index);
    if (p2item)
        return p2item->item;
    else
        return 0;
}


QLayoutItem *P2Layout::takeAt(int index)
{
    if ( index >= 0 && index < children.size() )
        return children.takeAt(index)->item;
    else
        return 0;
}


// Size geometry ///////////////////////////////////////////////////////////////


Qt::Orientations P2Layout::expandingDirections() const
{
    // Really, P2Layouts will not be resizable.
    return Qt::Horizontal | Qt::Vertical;
}


bool P2Layout::hasHeightForWidth() const
{
    // P2Layouts are rigid, with no wrapping behavior.
    return false;
}


QSize P2Layout::minimumSize() const
{
    return size;
}


QSize P2Layout::sizeHint() const
{
    return size;
}


void P2Layout::setGeometry( const QRect &rect )
{
    QLayout::setGeometry(rect);

    //...
}


void P2Layout::justifyContents()
{
    // Content origin should be at (2, 2).
    int xoffset = contentRectangle.x() - 2;
    int yoffset = contentRectangle.y() - 2;

    if ( xoffset || yoffset )
    {
        QPoint negOffset( -xoffset, -yoffset );

        for ( int i = 0; i < children.size(); i++ )
        {
            QLayoutItem *item = children.at( i )->item;
            item->setGeometry( item->geometry().translated( negOffset ) );
        }

        contentRectangle = contentRectangle.translated( negOffset );
    }
}

