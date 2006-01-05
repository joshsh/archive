#include "P2MainWindow.h"
#include "P2CentralWidget.h"


////////////////////////////////////////////////////////////////////////////////


P2MainWindow::P2MainWindow( QWidget* parent )
    : QMainWindow( parent )
{
    #ifdef DEBUG
        cout << "P2MainWindow[" << (int) this << "]::P2MainWindow( "
             << (int) parent << " )" << endl;
    #endif

    // Define the window icon.
    setWindowIcon( QIcon( P2GUI_ICON ) );

    // Define the palette.
    setPalette( QPalette(
        palette().foreground(),  // Foreground color.
        palette().button(),  // Button color.
        palette().light(),
        palette().dark(),
        palette().mid(),
        palette().text(),
        palette().brightText(),
        palette().base(),
        QBrush( QColor( BACKGROUND_COLOR ), Qt::SolidPattern ) ) );  // Background color.
    //setPalette( QPalette( QColor( BACKGROUND_COLOR ) ) );

    //setMinimumSize( 240, 320 );
    //setMaximumSize( 240, 320 );

    //~ Just testing the macros.
    #ifdef ARM_COMPILE
        #ifdef X86_COMPILE
            setWindowTitle( "Phase2 GUI (???)" );
        #else
            setWindowTitle( "Phase2 GUI (ARM)" );
        #endif
    #else
        #ifdef X86_COMPILE
            setWindowTitle( "Phase2 GUI (X86)" );
        #else
            setWindowTitle( "Phase2 GUI (???)" );
        #endif
    #endif

    // Only limit the size of the main window if it's to be displayed on a
    // large screen (otherwise allow the application to maximize it).
    #ifndef ARM_COMPILE
        setGeometry( QRect( 500, 500, SL5600__DISPLAY_WIDTH, SL5600__DISPLAY_HEIGHT ) );
        setMinimumSize( QSize( SL5600__DISPLAY_WIDTH, SL5600__DISPLAY_HEIGHT ) );
        setMaximumSize( QSize( SL5600__DISPLAY_WIDTH, SL5600__DISPLAY_HEIGHT ) );
    #endif

    QAction *action;

    // Build the menu bar.
    QMenuBar *menubar = this->menuBar();
//menubar->setFrameStyle( QFrame::NoFrame );

    // File menu.
    QMenu *fileMenu = menubar->addMenu( "&File" );
    action = fileMenu->addAction( "&New", this, SLOT( fileNew() ), 0 );
    action->setIcon( QIcon( ":/fileNew.png" ) );
    action = fileMenu->addAction( "&Open", this, SLOT( fileOpen() ), 0 );
    action->setIcon( QIcon( ":/fileOpen.png" ) );
    action = fileMenu->addAction( "&Save", this, SLOT( fileSave() ), 0 );
    action->setIcon( QIcon( ":/fileSave.png" ) );
    action = fileMenu->addAction( "Save &As", this, SLOT( fileSaveAs() ), 0 );
    action->setIcon( QIcon( ":/fileSaveAs.png" ) );
    fileMenu->addSeparator();
    action = fileMenu->addAction( "&Print", this, SLOT( filePrint() ), 0 );
    action->setIcon( QIcon( ":/filePrint.png" ) );
    fileMenu->addSeparator();
    action = fileMenu->addAction( "E&xit", this, SLOT( fileExit() ), 0 );
    action->setIcon( QIcon( ":/fileExit.png" ) );

    //- Edit menu.
    QMenu *editMenu = menubar->addMenu( "&Edit" );
    action = editMenu->addAction( "&Undo", this, SLOT( editUndo() ), 0 );
    action->setIcon( QIcon( ":/editUndo.png" ) );
    action = editMenu->addAction( "&Redo", this, SLOT( editRedo() ), 0 );
    action->setIcon( QIcon( ":/editRedo.png" ) );
    editMenu->addSeparator();
    action = editMenu->addAction( "Cu&t", this, SLOT( editCut() ), 0 );
    action->setIcon( QIcon( ":/editCut.png" ) );
    action = editMenu->addAction( "&Copy", this, SLOT( editCopy() ), 0 );
    action->setIcon( QIcon( ":/editCopy.png" ) );
    action = editMenu->addAction( "&Paste", this, SLOT( editPaste() ), 0 );
    action->setIcon( QIcon( ":/editPaste.png" ) );
    // action = editMenu->addAction( "&Find", this, SLOT( edit() ), 0 );
    // action->setIcon( QIcon( ":/editFind.png" ) );

    // View menu.
    QMenu *viewMenu = menubar->addMenu( "&View" );
    action = viewMenu->addAction( "&Console", this, SLOT( viewConsole() ), 0 );
    // action->setIcon( QIcon( ":/viewConsole.png" ) );
    action->setEnabled( false );
    viewMenu->addSeparator();
    action = viewMenu->addAction( "&Forward", this, SLOT( viewForward() ), 0 );
    action->setIcon( QIcon( ":/viewForward.png" ) );
    action = viewMenu->addAction( "&Back", this, SLOT( viewBack() ), 0 );
    action->setIcon( QIcon( ":/viewBack.png" ) );
    viewMenu->addSeparator();
    action = viewMenu->addAction(
        "&Show Frames", this, SLOT( viewShowFrames() ), 0 );
    action->setIcon( QIcon( ":/viewShowFrames.png" ) );
    action->setCheckable( true );
    action->setChecked( false );

    //- Help menu.
    QMenu *helpMenu = menubar->addMenu( "&Help" );
    action = helpMenu->addAction( "&Manual", this, SLOT( helpManual() ), 0 );
    action->setIcon( QIcon( ":/helpManual.png" ) );
    helpMenu->addSeparator();
    action = helpMenu->addAction( "&About Phase2", this, SLOT( helpAboutPhase2() ), 0 );
    action->setIcon( QIcon( ":/helpAboutPhase2.png" ) );

    // Create the central widget.
    P2CentralWidget *centralWidget = new P2CentralWidget(
        contentsRect().size() );
    //setCentralWidget( centralWidget );

    QScrollArea *scrollArea = new QScrollArea();
    //scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget( centralWidget );
    setCentralWidget( scrollArea );

cout << "width = " << centralWidget->width() << endl;
cout << "height = " << centralWidget->height() << endl;
cout << "position = (" << centralWidget->x() << ", " << centralWidget->y() << ")" << endl;
}


////////////////////////////////////////////////////////////////////////////////


void P2MainWindow::fileNew()
{
    cout << "void P2MainWindow::fileNew()" << endl;
}


void P2MainWindow::fileOpen()
{
    cout << "void P2MainWindow::fileOpen()" << endl;
}


void P2MainWindow::fileSave()
{
    cout << "void P2MainWindow::fileSave()" << endl;
}


void P2MainWindow::fileSaveAs()
{
    cout << "void P2MainWindow::fileSaveAs()" << endl;
}


void P2MainWindow::filePrint()
{
    cout << "void P2MainWindow::filePrint()" << endl;
}


void P2MainWindow::fileExit()
{
    this->close();
}


////////////////////////////////////////////////////////////////////////////////


void P2MainWindow::editUndo()
{
    cout << "void P2MainWindow::editUndo()" << endl;
}


void P2MainWindow::editRedo()
{
    cout << "void P2MainWindow::editRedo()" << endl;
}


void P2MainWindow::editCut()
{
    cout << "void P2MainWindow::editCut()" << endl;
}


void P2MainWindow::editCopy()
{
    cout << "void P2MainWindow::editCopy()" << endl;
}


void P2MainWindow::editPaste()
{
    cout << "void P2MainWindow::editPaste()" << endl;
}


////////////////////////////////////////////////////////////////////////////////


void P2MainWindow::viewConsole()
{
    cout << "void P2MainWindow::viewConsole()" << endl;
}


void P2MainWindow::viewForward()
{
    cout << "void P2MainWindow::viewForward()" << endl;
}


void P2MainWindow::viewBack()
{
    cout << "void P2MainWindow::viewBack()" << endl;
}


void P2MainWindow::viewShowFrames()
{
    environment()->idleFramesAreVisible = !environment()->idleFramesAreVisible;

    //~ temporary
    QWidget cover( this, 0 );
    cover.setGeometry( contentsRect() );
    cover.setVisible( true );
    cover.setVisible( false );

    //show();
    //update();
    //update( visibleRegion() );
    //update( 0, 0, width(), height() );
    //update( QRegion( QRect( QPoint( 0, 0 ), QSize( geometry().width(), geometry().height() ) ), QRegion::Rectangle ) );
    //update( QRect( QPoint( 0, 0 ), QSize( geometry().width(), geometry().height() ) ) );
    //update( geometry() );
    //centralWidget->doUpdate();
    //centralWidget->update( 0, 0, centralWidget->geometry().width(), centralWidget->geometry().height() );
    //centralWidget->update();
}


////////////////////////////////////////////////////////////////////////////////


void P2MainWindow::helpManual()
{
    cout << "void P2MainWindow::helpManual()" << endl;
}


void P2MainWindow::helpAboutPhase2()
{
    cout << "void P2MainWindow::helpAboutPhase2()" << endl;
}

