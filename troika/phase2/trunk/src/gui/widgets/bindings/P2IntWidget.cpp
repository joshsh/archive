#include <widgets/bindings/P2IntWidget.h>


////////////////////////////////////////////////////////////////////////////////


static QString
getText( Object *o )
{
    char buffer[100];
    object__type( o )->encode( object__value( o ), buffer );
    return QString( buffer );
}


P2IntWidget::P2IntWidget( Object *o )
    : P2Text( getText( o ), &QColor( INT__COLOR ))
{
    //...
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
