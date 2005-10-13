#include <qmainwindow.h>
//#include <qpe/qpeapplication.h>
//#include <qtopia/qpeapplication.h>
#include <qapplication.h>

class P2MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    P2MainWindow(QWidget* parent=0, const char* name=0, WFlags fl=0);

public slots:

    void    cmdFileQuit();

};
