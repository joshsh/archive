#include <widgets/P2ViewStack.h>


////////////////////////////////////////////////////////////////////////////////


P2ViewStack::
P2ViewStack( P2Environment *env, QWidget *parent )
  : QStackedWidget( parent ),
    environment( env )
{
    //...
}


void P2ViewStack::
goBack()
{
    if ( currentIndex() > 0 )
    {
        setCurrentIndex( currentIndex() - 1 );
        refresh( *environment );

        if ( currentIndex() == 0 )
            emit backDisabled();

        emit forwardEnabled();
    }
}


void P2ViewStack::
goForward()
{
    if ( currentIndex() < count() - 1 )
    {
        setCurrentIndex( currentIndex() + 1 );
        refresh( *environment );

        if ( currentIndex() == count() - 1 )
            emit forwardDisabled();

        emit backEnabled();
    }
}


void P2ViewStack::
addView( const Object *o )
{
    // Remove all widgets forward of the current index (these become
    // unreachable once the new widget it added.
    int i = count() - ( currentIndex() + 1 );
    for ( int j = 0; j < i; j++ )
        remove( currentIndex() + 1 );

    P2View *newView = new P2View( o, environment );
    addWidget( newView );

    connect(
        newView->binder(),    SIGNAL( objectViewRequest( const Object* ) ),
        this,               SLOT( addView( const Object* ) ) );

    // connect...

    // The new widget becomes the visible widget.
    setCurrentIndex( count() - 1 );

    // If there are too many widgets in the stack, remove the bottommost widget.
    if ( count() > VIEWSTACK__MAX_DEPTH )
        remove( 0 );

    if ( count() > 1 )
        emit backEnabled();

    emit forwardDisabled();
}


P2Widget *P2ViewStack::
focusWidget()
{
    if ( count() )
        return ( ( P2View* ) currentWidget() )->focusWidget();

    else
        return 0;
}


////////////////////////////////////////////////////////////////////////////////


void P2ViewStack::
refresh( const P2Environment &env )
{
    if ( count() > 0 )
        ( ( P2Widget* ) currentWidget() )->refresh( env );

    QWidget::update();
}


void P2ViewStack::
update()
{
    QWidget::update();
}


void P2ViewStack::
remove( int index )
{
    QWidget *w = widget( index );
    removeWidget( w );
    delete w;
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
