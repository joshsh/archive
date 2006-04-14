#include "P2CharWidget.h"


////////////////////////////////////////////////////////////////////////////////


static QString
getText( Object *o )
{
    char buffer[100];
    object__type( o )->encode( object__value( o ), buffer );
    return QString( buffer );
}


P2CharWidget::P2CharWidget( Object *o )
    : P2Text( getText( o ), &QColor( CHAR__COLOR ))
{
    //...
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
