#include "P2MainWindow.h"  // P2MainWindow
#include "P2CentralWidget.h"  // P2CentralWidget

// ? WFlags

#include <qwidget.h>  // QWidget
#include <qmainwindow.h>  // QMainWindow

#include <qpopupmenu.h>  // QPopupMenu
#include <qmenubar.h>  // QMenuBar

// #include <qtextview.h>
// #include <qmultilineedit.h>

////////////////////////////////////////////////////////////////////////////////

P2MainWindow::P2MainWindow( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    //~ Just testing the macros.
    #ifdef ARM_COMPILE
        #ifdef X86_COMPILE
            setCaption("Troika Edit (???)");
        #else
            setCaption("Troika Edit (ARM)");
        #endif
    #else
        #ifdef X86_COMPILE
            setCaption("Troika Edit (X86)");
        #else
            setCaption("Troika Edit (???)");
        #endif
    #endif

    // Only limit the size of the main window if it's to be displayed on a
    // large screen (otherwise allow the application to maximize it).
    #ifndef ARM_COMPILE
        setGeometry( 500, 500, 240, 320 );
        setMinimumSize( 240, 320 );
        setMaximumSize( 240, 320 );
    #endif

    QMenuBar *menubar = this->menuBar( );
    QPopupMenu *mnuFile = new QPopupMenu( this, "MenuFile" );
    menubar->insertItem( "&File", mnuFile );
    mnuFile->insertItem( "&Quit", this, SLOT( quit()), 0, 1 );

    P2CentralWidget *w = new P2CentralWidget( this, "central widget" );
    setCentralWidget( w );

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
