#include "P2MainWindow.h"  // P2MainWindow, QMainWindow, QWidget
#include "P2CentralWidget.h"  // P2CentralWidget

    #include "P2Binder.h"

// ? WFlags

//#include <q3popupmenu.h>  // QPopupMenu
#include <QMenuBar>  // QMenu, QMenuBar

// #include <qtextview.h>
// #include <qmultilineedit.h>

////////////////////////////////////////////////////////////////////////////////

P2MainWindow::P2MainWindow( QWidget* parent, Qt::WFlags fl )
    : QMainWindow( parent, fl )
{
    //~ Just testing the macros.
    #ifdef ARM_COMPILE
        #ifdef X86_COMPILE
            setWindowTitle("Phase2 GUI (???)");
        #else
            setWindowTitle("Phase2 GUI (ARM)");
        #endif
    #else
        #ifdef X86_COMPILE
            setWindowTitle("Phase2 GUI (X86)");
        #else
            setWindowTitle("Phase2 GUI (???)");
        #endif
    #endif

    // Only limit the size of the main window if it's to be displayed on a
    // large screen (otherwise allow the application to maximize it).
    #ifndef ARM_COMPILE
        setGeometry( QRect( 500, 500, 240, 320 ) );
        setMinimumSize( QSize( 240, 320 ) );
        setMaximumSize( QSize( 240, 320 ) );
    #endif

    QMenuBar *menubar = this->menuBar( );
    QMenu *fileMenu = menubar->addMenu( "&File" );
    fileMenu->addAction( "&Quit", this, SLOT( quit() ), 0 );

    P2CentralWidget *cw = new P2CentralWidget( this );
    setCentralWidget( cw );

    // QTextView *tv = new QTextView( "Salut!", 0, this, "Log viewer" );
    // setCentralWidget( tv );

    // QMultiLineEdit *mle = new QMultiLineEdit( this );
    // mle->setText( "Salut!" );
    // setCentralWidget( mle );
}


void P2MainWindow::quit( )
{
    this->close( );
}
