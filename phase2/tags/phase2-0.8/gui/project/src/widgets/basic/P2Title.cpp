#include <widgets/basic/P2Title.h>


////////////////////////////////////////////////////////////////////////////////


P2Title::P2Title( QString text )
    : P2AbstractText( text )
{
    //...
}


QColor P2Title::color()
{
    if ( isActive() )
        return QColor( COLOR__TITLE__ACTIVE );
    else
        return QColor( COLOR__TITLE__INACTIVE );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
