#include <widgets/basic/P2AbstractText.h>


////////////////////////////////////////////////////////////////////////////////


/* FIXME: this does no good, because newlines are treated as ordinary characters
   by QFontMetrics. */
static QString
wrapText( QString text )
{
    int l = 80;

    for ( int i = l; i < text.size(); i += l + 1 )
        text.insert( i, QChar( '\n' ) );

    return text;
}


P2AbstractText::P2AbstractText( QString text )
        : P2BasicWidget()
{
    this->text = wrapText( text );

    update2();
}


QSize P2AbstractText::sizeHint() const
{
    return cachedSizeHint;
}


void P2AbstractText::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );

    painter.setPen( color() );

    // Note: assumes the same font as the one used to find the bounding
    // rectangle at construction time.
    painter.drawText( 0, cachedSizeHint.height() - 4, text );
//painter.drawRect( QRect( QPoint( 0, 0 ), cachedSizeHint - QSize( 1, 1 ) ) );
}


void P2AbstractText::update2()
{
    int h = fontMetrics().height();

    cachedSizeHint = QSize( fontMetrics().boundingRect( this->text ).width() + 1, h );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
