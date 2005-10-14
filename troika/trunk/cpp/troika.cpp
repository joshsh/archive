#include "P2MainWindow.h"  // P2MainWindow

#include <qapplication.h>  // QApplication

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    QApplication *a = new QApplication(argc, argv);
    QMainWindow *w = new P2MainWindow();

    a->setMainWidget(w);

    // Maximize the main window if it has not defined its own maximum dimensions.
    w->showMaximized();

    return a->exec();
}
