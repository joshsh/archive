#include <qmainwindow.h>
#include <qapplication.h>
#include "P2MainWindow.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    QMainWindow *w = new P2MainWindow();

    a.setMainWidget(w);
    w->show();
    return a.exec();
}
