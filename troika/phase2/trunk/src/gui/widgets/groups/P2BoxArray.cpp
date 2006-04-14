#include <QtGui>

#include "P2BoxArray.h"


////////////////////////////////////////////////////////////////////////////////


P2BoxArray::P2BoxArray( QBoxLayout::Direction dir )
    : P2Widget()
{
    // Note: no need to call setLayout after using this constructor.
    new QBoxLayout( dir, this );

    // For now...
    setAcceptDrops( false );

    connect( &array, SIGNAL( resized() ), this, SLOT( update() ) );

    // For now...
    setAcceptDrops( false );
}


const QString P2BoxArray::className()
{
    return QString( "P2BoxArray" );
}


////////////////////////////////////////////////////////////////////////////////


void P2BoxArray::add( P2Widget *widget )
{
    array.add( widget );
    layout()->addWidget( widget );

    // "When you use a layout, you don't need to pass a parent when constructing
    // the child widgets. The layout will automatically reparent the widgets
    // (using QWidget::setParent()) so that they are children of the widget on
    // which the layout is installed.
    widget->setParent( this );

    widget->isDependent = true;
}


// Event handling //////////////////////////////////////////////////////////////


void P2BoxArray::refresh( const P2Environment &env )
{
    array.refresh( env );
}


void P2BoxArray::update()
{
    layout()->update();
    resize( layout()->sizeHint() );

    emit resized( 0 );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
