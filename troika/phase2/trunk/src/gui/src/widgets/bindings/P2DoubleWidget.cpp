#include <widgets/bindings/P2DoubleWidget.h>


////////////////////////////////////////////////////////////////////////////////


static QString
getText( const Object *o )
{
    char buffer[100];
    object__type( o )->encode( object__value( o ), buffer );
    return QString( buffer );
}


P2DoubleWidget::P2DoubleWidget( const Object *o )
    : P2Text( getText( o ), QColor( DOUBLE__COLOR ))
{
    //...
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
