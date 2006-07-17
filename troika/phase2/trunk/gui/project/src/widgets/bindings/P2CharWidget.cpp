#include <widgets/bindings/P2CharWidget.h>


////////////////////////////////////////////////////////////////////////////////


static QString
getText( const Object *o )
{
    char buffer[100];
    object__encode( o, buffer );
    return QString( buffer );
}


P2CharWidget::P2CharWidget( const Object *o )
    : P2Text( getText( o ), QColor( CHAR__COLOR ))
{
    //...
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
