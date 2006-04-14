#include <widgets/bindings/P2TypeWidget.h>


////////////////////////////////////////////////////////////////////////////////


static QString
getText( Object *o )
{
    char buffer[100];
    object__type( o )->encode( object__value( o ), buffer );
    return QString( buffer );
}


P2TypeWidget::P2TypeWidget( Object *o )
    : P2Text( getText( o ), &QColor( TYPE__COLOR ))
{
    //...
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
