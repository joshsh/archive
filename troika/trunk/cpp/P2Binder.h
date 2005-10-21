#ifndef P2Binder_h
#define P2Binder_h

////////////////////////////////////////////////////////////////////////////////

#include "compile-target.h"  // Macros specific to the target architecture.

#include <qwidget.h>  // QWidget

////////////////////////////////////////////////////////////////////////////////

class P2Binder : public QWidget
{
    Q_OBJECT

public:

    P2Binder(QWidget* parent = 0, const char* name = 0);

protected:

    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Binder_h
