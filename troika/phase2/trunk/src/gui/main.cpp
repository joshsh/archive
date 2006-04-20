/*
export QTDIR=/usr/local/Trolltech/Qt-4.1.0/
export PATH=$QTDIR/bin:$PATH
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
*/

#include <QtGui>

#include <widgets/P2MainWindow.h>
#include <P2Environment.h>
#include <P2Binder.h>

extern "C"
{
#include <Compiler.h>
}


////////////////////////////////////////////////////////////////////////////////

QList< P2MainWindow* > windows;


/** Create a new top-level widget. */
void newMainWindow( P2Binder &b )
{
    P2MainWindow *w = new P2MainWindow( b );
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


static void
delete_windows()
{
    while ( windows.size() )
        delete windows.takeFirst();
}


/** Main function for the Phase2 GUI. */
int main( int argc, char **argv )
{
    P2Environment env;
    //compiler__deserialize( env.getCompiler(), "guitest.p2" );
    compiler__deserialize( env.getCompiler(), "../api/test.p2" );

    P2Binder binder( env );

    QApplication a( argc, argv );

//cout << "a.startDragDistance():" << a.startDragDistance() << endl;
//cout << "a.startDragTime():" << a.startDragTime() << endl;
//a.setStartDragTime( 5000 );

    // In the beginning there was a single top-level window.
    newMainWindow( binder );

    int ret = a.exec();

    delete_windows();

    return ret;
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
