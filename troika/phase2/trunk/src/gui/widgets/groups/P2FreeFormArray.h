#ifndef P2FreeFormArray_h
#define P2FreeFormArray_h

#include <P2WidgetArray.h>

////////////////////////////////////////////////////////////////////////////////

class P2FreeFormArray : public P2Widget
{
    Q_OBJECT

public:

    /** Constructs an empty P2FreeFormArray. */
    P2FreeFormArray();

    const QString className();

    void add( P2Widget *widget, const QPoint &position );

bool isFrame() { return false; }
bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin ) { return false; }
bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) { return false; }

public slots:

    void refresh( const P2Environment &env );

protected:

    P2WidgetArray array;

protected slots:

    void update();
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2FreeFormArray_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
