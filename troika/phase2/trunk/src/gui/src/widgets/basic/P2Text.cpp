#include <widgets/basic/P2Text.h>


////////////////////////////////////////////////////////////////////////////////


P2Text::P2Text( QString text, const QColor &color )
        : P2AbstractText( text )
{
    constColor = color;

    QFont f = font();
    f.setBold( true );
    setFont( f );

    update2();
}


QColor P2Text::color()
{
    return constColor;
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
