#include <widgets/bindings/P2TermWidget.h>


////////////////////////////////////////////////////////////////////////////////


static QString
getText( Object *o )
{
    char buffer[100];
    object__type( o )->encode( object__value( o ), buffer );
    return QString( buffer );
}


P2TermWidget::P2TermWidget( Object *o )
    : P2Text( getText( o ), QColor( TERM__COLOR ))
{
    //...
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
