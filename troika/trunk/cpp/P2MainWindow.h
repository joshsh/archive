#ifndef P2MainWindow_h
#define P2MainWindow_h

////////////////////////////////////////////////////////////////////////////////

#include "compile-target.h"  // Macros specific to the target architecture.

#include <qwidget.h>  // QWidget
#include <qmainwindow.h>  // QMainWindow

////////////////////////////////////////////////////////////////////////////////

class P2MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    P2MainWindow(QWidget* parent=0, const char* name=0, WFlags fl=0);

public slots:

    void    quit();

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2MainWindow_h
