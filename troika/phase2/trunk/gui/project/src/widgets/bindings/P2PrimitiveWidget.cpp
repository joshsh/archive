#include <widgets/bindings/P2PrimitiveWidget.h>


////////////////////////////////////////////////////////////////////////////////


static QString
getText( const Object *o )
{
    char buffer[100];
    object__encode( o, buffer );
    return QString( buffer );
}


P2PrimitiveWidget::P2PrimitiveWidget( const Object *o )
    : P2Text( getText( o ), QColor( PRIM__COLOR ))
{
    //...
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
