#include <widgets/bindings/P2CombinatorWidget.h>


////////////////////////////////////////////////////////////////////////////////


static QString
getText( const Object *o )
{
    char buffer[100];
    object__type( o )->encode( object__value( o ), buffer );
    return QString( buffer );
}


P2CombinatorWidget::P2CombinatorWidget( const Object *o )
    : P2Text( getText( o ), QColor( COMBINATOR__COLOR ))
{
    //...
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
