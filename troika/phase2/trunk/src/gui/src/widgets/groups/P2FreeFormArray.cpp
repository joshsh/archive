#include <widgets/groups/P2FreeFormArray.h>
#include <layout/P2FreeFormLayout.h>


////////////////////////////////////////////////////////////////////////////////


P2FreeFormArray::P2FreeFormArray()
    : P2Widget()
{
    // Note: no need to call setLayout after using this constructor.
    P2FreeFormLayout *layout = new P2FreeFormLayout( this );

    connect(
        layout, SIGNAL( resized() ),
        this,   SLOT( update() ) );

    // For now...
    setAcceptDrops( false );
}


////////////////////////////////////////////////////////////////////////////////


void P2FreeFormArray::add( P2Widget *widget, const QPoint &position )
{
    ( ( P2FreeFormLayout* ) layout() )->add( widget, position );

    // "When you use a layout, you don't need to pass a parent when constructing
    // the child widgets. The layout will automatically reparent the widgets
    // (using QWidget::setParent()) so that they are children of the widget on
    // which the layout is installed.
    widget->setParent( this );
}


// Event handling //////////////////////////////////////////////////////////////


void P2FreeFormArray::refresh( const P2Environment &env )
{
    ( ( P2FreeFormLayout* ) layout() )->refresh( env );
}


void P2FreeFormArray::update()
{
    //( ( P2FreeFormLayout* ) layout() )->update();
    resize( layout()->sizeHint() );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
