#include <widgets/bindings/P2StringWidget.h>


////////////////////////////////////////////////////////////////////////////////


static QString
getText( const Object *o )
{
    char buffer[ENCODING__BUFFER_SIZE];
    object__encode( o, buffer );
    return QString( buffer );
}


P2StringWidget::P2StringWidget( const Object *o )
//    : P2Text( getText( o ), QColor( STRING__COLOR ))
    : P2TextEdit( getText( o ), true, QColor( STRING__COLOR ) )
{
    //...
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
