#ifndef P2Title_h
#define P2Title_h

////////////////////////////////////////////////////////////////////////////////

#include <P2AbstractText.h>

////////////////////////////////////////////////////////////////////////////////

/** A display widget for short plain-text strings. */
class P2Title : public P2AbstractText
{

public:

    /** \param text  character string content */
    P2Title( QString text );

    QColor color();

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Title_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
