#ifndef P2MainWindow_h
#define P2MainWindow_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2Environment.h"
#include "P2CentralWidget.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class P2MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    P2MainWindow( QWidget* parent, Qt::WFlags fl );
    ~P2MainWindow();

public slots:

    void quit( );
    void showIdleFrames( );
    void hideIdleFrames( );

private:

    P2Environment *environment;
    P2CentralWidget *centralWidget;

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2MainWindow_h
