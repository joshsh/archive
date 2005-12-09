/*
export QTDIR=/usr/local/Trolltech/Qt-4.0.1/
export PATH=$QTDIR/bin:$PATH
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH

NAME=troika
qmake -project -o $NAME.pro
qmake $NAME.pro
make
*/

#include "P2MainWindow.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

int main( int argc, char **argv )
{
    QApplication a( argc, argv );

    // Single top-level widget.
    P2MainWindow w( 0, 0 );

    // If the display area is small, maximize the main window.
    #ifdef ARM_COMPILE
        w.showMaximized( );
    #else
        w.show( );
    #endif

    return a.exec( );
}

