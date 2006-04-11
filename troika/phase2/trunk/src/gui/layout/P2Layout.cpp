#include "P2Layout.h"


////////////////////////////////////////////////////////////////////////////////


P2Layout::P2Layout( QWidget *parent )
    : QLayout( parent )
{
    // ...
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


int P2Layout::count() const
{
    return children.size();
}


QLayoutItem *P2Layout::itemAt( int index ) const
{
    return children.value(index);
}


QLayoutItem *P2Layout::takeAt( int index )
{
    if ( index >= 0 && index < children.size() )
        return children.takeAt( index );
    else
        return 0;
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


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
