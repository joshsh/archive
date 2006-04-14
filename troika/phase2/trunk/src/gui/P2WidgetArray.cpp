#include <P2WidgetArray.h>


////////////////////////////////////////////////////////////////////////////////


P2WidgetArray::P2WidgetArray()
{
    //...
}


P2WidgetArray::~P2WidgetArray()
{
    // Delete all layout items.
    for ( int i = 0; i < kids.size(); i++ )
        delete kids.at( i );
}


QList<QLayoutItem*> P2WidgetArray::list()
{
    return kids;
}


////////////////////////////////////////////////////////////////////////////////


void P2WidgetArray::addItem( QLayoutItem *item )
{
    kids.append( item );
}


void P2WidgetArray::add( P2Widget *widget )
{
    QLayoutItem *item = new QWidgetItem( widget );
cout << "widget.size() = (" << widget->width() << ", " << widget->height() << ")" << endl;
cout << "widget.sizeHint() = (" << widget->sizeHint().width() << ", " << widget->sizeHint().height() << ")" << endl;
    //item->resize( widget->sizeHint() );
    addItem( item );

    connect(
        widget, SIGNAL( resized( QResizeEvent* ) ),
        this,   SLOT( childResizeEvent( QResizeEvent* ) ) );
}


int P2WidgetArray::count() const
{
    return kids.size();
}


QLayoutItem *P2WidgetArray::itemAt( int i ) const
{
    return kids.value( i );
}


QLayoutItem *P2WidgetArray::takeAt( int i )
{
    if ( i >= 0 && i < kids.size() )
        return kids.takeAt( i );
    else
        return 0;
}


void P2WidgetArray::showChildren() const
{
    for ( int i = 0; i < kids.size(); i++ )
    {
        cout << indent()
             << i << ":\t";

        P2Widget *child = ( P2Widget* ) kids.at( i )->widget();
        cout << child->className().toStdString() << "\t";
        if ( child->objectName() != 0 )
            cout << child->objectName().toStdString();

        cout << endl;
    }
}


////////////////////////////////////////////////////////////////////////////////


void P2WidgetArray::refresh( const P2Environment &env )
{
    // Propagate the signal to all children.
    for ( int i = 0; i < kids.size(); i++ )
    {
        P2Widget *child = ( P2Widget* ) kids.at( i )->widget();
        child->refresh( env );
    }
}


void P2WidgetArray::childResizeEvent(  QResizeEvent* event )
{
    emit resized();
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
