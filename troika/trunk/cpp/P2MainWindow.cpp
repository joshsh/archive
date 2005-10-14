#include "P2MainWindow.h"  // P2MainWindow
#include "P2CentralWidget.h"  // P2CentralWidget

// ? WFlags

#include <qwidget.h>  // QWidget
#include <qmainwindow.h>  // QMainWindow

#include <qpopupmenu.h>  // QPopupMenu
#include <qmenubar.h>  // QMenuBar

////////////////////////////////////////////////////////////////////////////////

P2MainWindow::P2MainWindow(QWidget* parent, const char* name, WFlags fl)
    : QMainWindow(parent, name, fl)
{
    //~ Just testing the macros.
    #ifdef ARM_COMPILE
        setCaption("Troika Edit (ARM)");
    #else
    #ifdef X86_COMPILE
        setCaption("Troika Edit (X86)");
    #else
        setCaption("Troika Edit (???)");
    #endif
    #endif

    //setGeometry( 40, 40, 300, 300 );

    // Only limit the size of the main window if it's to be displayed on a
    // large screen (otherwise allow the application to maximize it).
    #ifndef ARM_COMPILE
        setMinimumSize(240, 320);
        setMaximumSize(240, 320);
    #endif

    QMenuBar *menubar = this->menuBar();
    QPopupMenu *mnuFile = new QPopupMenu(this, "MenuFile");
    menubar->insertItem("&File", mnuFile);
    mnuFile->insertItem("&Quit", this, SLOT(cmdFileQuit()), 0, 1);

    P2CentralWidget *w = new P2CentralWidget(this, "central widget");

    // Origin at (10,10).
    //w.setGeometry( 10, 10, 200, 120 );

    setCentralWidget(w);
}


void P2MainWindow::cmdFileQuit()
{
    this->close();
}
