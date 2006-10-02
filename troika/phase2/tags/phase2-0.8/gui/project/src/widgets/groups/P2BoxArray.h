#ifndef P2BoxArray_h
#define P2BoxArray_h

#include <P2WidgetArray.h>

////////////////////////////////////////////////////////////////////////////////

class P2BoxArray : public P2Widget
{
    Q_OBJECT

public:

    /** Constructs an empty P2BoxArray. */
    P2BoxArray( QBoxLayout::Direction dir );

    void add( P2Widget *widget );

    QSize sizeHint() const;

public slots:

    void refresh( const P2Environment &env );

protected:

    P2WidgetArray array;

protected slots:

    void update();
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2BoxArray_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
