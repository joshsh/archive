#include <widgets/bindings/P2TermWidget.h>


////////////////////////////////////////////////////////////////////////////////


static QString
getText( const Object *o )
{
    char buffer[1000];
    object__encode( o, buffer );
    return QString( buffer );
}


P2TermWidget::P2TermWidget( const Object *o )
    : P2Text( getText( o ), QColor( TERM__COLOR ))
{
    //...
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
