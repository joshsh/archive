/*
export QTDIR=/usr/local/Trolltech/Qt-4.1.0/
export PATH=$QTDIR/bin:$PATH
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
*/

#include "global.h"
#include "P2MainWindow.h"
#include "P2Environment.h"

#include <QtGui>


////////////////////////////////////////////////////////////////////////////////

QList< P2MainWindow* > windows;


/** Create a new top-level widget. */
void newMainWindow( P2Environment &env )
{
    P2MainWindow *w = new P2MainWindow( env, 0 );
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


/** Main function for the Phase2 GUI. */
int main( int argc, char **argv )
{
    P2Environment env;

    QApplication a( argc, argv );

cout << "a.startDragDistance():" << a.startDragDistance() << endl;
cout << "a.startDragTime():" << a.startDragTime() << endl;
//a.setStartDragTime( 5000 );

    // In the beginning there was a single top-level window.
    newMainWindow( env );

    int ret = a.exec();

    while ( windows.size() )
        delete windows.takeFirst();

    return ret;
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
