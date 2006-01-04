#ifndef P2MainWindow_h
#define P2MainWindow_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class P2MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    P2MainWindow( QWidget* parent );

public slots:

    void quit( );

    void consoleView();

    /** Turns inactive frame visibility on or off. */
    void toggleFrameVisibility();

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2MainWindow_h
