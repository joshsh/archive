/*
export QTDIR=/usr/local/Trolltech/Qt-4.0.1/
export PATH=$QTDIR/bin:$PATH
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH

NAME=troika
qmake -project -o $NAME.pro
qmake $NAME.pro
make
*/

#include "global.h"
#include "P2MainWindow.h"
#include "P2Environment.h"

#include <QtGui>


////////////////////////////////////////////////////////////////////////////////


/** A global P2Environment variable */
P2Environment *env = 0;


/** \return  a global P2Environment variable */
P2Environment *environment()
{
    return env;
}


////////////////////////////////////////////////////////////////////////////////


/** Main function for the Phase2 GUI. */
int main( int argc, char **argv )
{
    env = new P2Environment();

    QApplication a( argc, argv );

    // Single top-level widget.
    P2MainWindow w( 0 );

    // If the display area is small, maximize the main window.
    #ifdef ARM_COMPILE
        w.showMaximized( );
    #else
        w.show( );
    #endif

    int ret = a.exec( );

    delete env;

    return ret;
}

