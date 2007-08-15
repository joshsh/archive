#ifndef P2AbstractText_h
#define P2AbstractText_h

////////////////////////////////////////////////////////////////////////////////

#include <widgets/P2BasicWidget.h>

////////////////////////////////////////////////////////////////////////////////

/** A display widget for short plain-text strings. */
class P2AbstractText : public P2BasicWidget
{

public:

    QSize sizeHint() const;  // Must be const.

protected:

    /** \param text  character string content */
    P2AbstractText( QString text );

    void paintEvent( QPaintEvent *event );

public slots:

    void update2();

protected:

    QString text;
    virtual QColor color() = 0;

private:

    QSize cachedSizeHint;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2AbstractText_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
