#ifndef P2Image_h
#define P2Image_h

////////////////////////////////////////////////////////////////////////////////

#include <widgets/P2BasicWidget.h>

////////////////////////////////////////////////////////////////////////////////

/** A wrapper for QImage. */
class P2Image : public P2BasicWidget
{

public:

    P2Image( const char * const xpm[] );
    P2Image( const QString &filename );
    P2Image( const QImage &i );

    QSize sizeHint() const;

protected:

    void paintEvent( QPaintEvent *event );

private:

    QImage image;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Image_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
