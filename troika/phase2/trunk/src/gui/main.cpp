/*
export QTDIR=/usr/local/Trolltech/Qt-4.0.1/
export PATH=$QTDIR/bin:$PATH
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH

export QTDIR=/usr/local/Trolltech/Qt-4.1.0/
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


QList< P2MainWindow* > windows;


////////////////////////////////////////////////////////////////////////////////


/** Create a new top-level widget. */
void newMainWindow()
{
    P2MainWindow *w = new P2MainWindow( 0, 0 );
/*
            Qt::Window
            | Qt::WindowTitleHint
            | Qt::WindowSystemMenuHint
            | Qt::WindowMinMaxButtonsHint );
//*/

    windows.append( w );

    w->show();

    // If the display area is small, maximize the first main window.
    #ifdef ARM_COMPILE
        if ( windows.size() == 1 )
            w->showMaximized( );
    #endif
}


void refreshAll()
{
    for ( int i = 0; i < windows.size(); i++ )
        windows.at( i )->refresh();
}


/** Main function for the Phase2 GUI. */
int main( int argc, char **argv )
{
    env = new P2Environment();

    QApplication a( argc, argv );

cout << "a.startDragDistance():" << a.startDragDistance() << endl;
cout << "a.startDragTime():" << a.startDragTime() << endl;
//a.setStartDragTime( 5000 );

    // In the beginning there was a single top-level window.
    newMainWindow();

    int ret = a.exec( );

    delete env;

    while ( windows.size() )
        delete windows.takeFirst();

    return ret;
}

