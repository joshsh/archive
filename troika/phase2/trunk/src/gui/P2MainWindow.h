#ifndef P2MainWindow_h
#define P2MainWindow_h

////////////////////////////////////////////////////////////////////////////////

#include "compile-target.h"  // Macros specific to the target architecture.

#include <QWidget>  // QWidget
#include <QMainWindow>  // QMainWindow

////////////////////////////////////////////////////////////////////////////////

class P2MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    P2MainWindow( QWidget* parent, Qt::WFlags fl );

public slots:

    void quit( );

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2MainWindow_h
