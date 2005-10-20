#include "P2MainWindow.h"  // P2MainWindow

#include <qapplication.h>  // QApplication

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    P2MainWindow w;
    a.setMainWidget(&w);

    // Maximize the main window if it has not defined its own maximum dimensions.
    w.showMaximized();

    return a.exec();
}
