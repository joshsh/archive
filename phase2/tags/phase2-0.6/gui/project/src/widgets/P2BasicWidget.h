#ifndef P2BasicWidget_h
#define P2BasicWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <widgets/P2Widget.h>
#include <P2Environment.h>

////////////////////////////////////////////////////////////////////////////////

class P2BasicWidget : public P2Widget
{
    Q_OBJECT

public slots:

    void refresh( const P2Environment &env );

protected:

    /** Constructs an empty P2BasicWidget. */
    P2BasicWidget();

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2BasicWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
