#include "P2Layout.h"


////////////////////////////////////////////////////////////////////////////////


P2Layout::P2Layout() // QWidget *parent )
//    : QLayout( parent )
{
    receivedMinimumSize = QSize( 0, 0 );
}


/*
P2Layout::~P2Layout()
{
    // Delete all layout items.
    for ( int i = 0; i < kinder.size(); i++ )
        delete kinder.at( i );
}
*/


////////////////////////////////////////////////////////////////////////////////


void P2Layout::addItem( QLayoutItem *item )
{
cout << "P2Layout[" << ( int ) this << "]::addItem" << endl;
    kinder.append( item );
cout << "--> kinder.size() = " << kinder.size() << endl;
    //adjustGeometry();
}


void P2Layout::addWidget( P2Widget *widget )
{
    addItem( new QWidgetItem( widget ) );
}


int P2Layout::count() const
{
    return kinder.size();
}


QLayoutItem *P2Layout::itemAt( int index ) const
{
    return kinder.value( index );
}


QLayoutItem *P2Layout::takeAt( int index )
{
    if ( index >= 0 && index < kinder.size() )
        return kinder.takeAt( index );
    else
        return 0;
}


void P2Layout::refreshChildren( const P2Environment &env )
{
cout << "P2Layout[" << ( int ) this << "]::refreshChildren (kinder.size() = " << kinder.size() << ")" << endl;
    // Propagate the signal to all children.
    for ( int i = 0; i < kinder.size(); i++ )
    {
        P2Widget *child = ( P2Widget* ) kinder.at( i )->widget();
        child->refresh( env );
    }
}


void P2Layout::showChildren() const
{
    for ( int i = 0; i < kinder.size(); i++ )
    {
        cout << indent()
             << i << ":\t";

        P2Widget *child = ( P2Widget* ) kinder.at( i )->widget();
        cout << child->className().toStdString() << "\t";
        if ( child->objectName() != 0 )
            cout << child->objectName().toStdString();

        cout << endl;
    }
}


////////////////////////////////////////////////////////////////////////////////


/*
void P2Layout::resized()
{
    //...
}
*/


void P2Layout::setGeometry( const QRect &rect )
{
    //QLayout::setGeometry( rect );

    //...
}


QSize P2Layout::minimumSize() const
{
    return cachedSizeHint;
}


QSize P2Layout::sizeHint() const
{
    return cachedSizeHint;
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
