#include "P2MainWindow.h"  // P2MainWindow

#include <qapplication.h>  // QApplication

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    P2MainWindow w;
    a.setMainWidget(&w);

    // If the display area is small, maximize the main window.
    #ifdef ARM_COMPILE
    w.showMaximized();
    #else
    w.show();
    #endif

    return a.exec();
}
