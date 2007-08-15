#ifndef P2Text_h
#define P2Text_h

////////////////////////////////////////////////////////////////////////////////

#include <P2AbstractText.h>

////////////////////////////////////////////////////////////////////////////////

/** A display widget for short plain-text strings. */
class P2Text : public P2AbstractText
{

public:

    /** \param text  character string content
        \param color  text color (defaults to the foreground color) */
    P2Text( QString text, const QColor &color );

protected:

    QColor constColor;
    QColor color();

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Text_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
