#include "P2MainWindow.h"
#include "dialogs/RenameDialog.h"


////////////////////////////////////////////////////////////////////////////////


P2MainWindow::P2MainWindow( P2Environment &env, Qt::WFlags flags )
    : QMainWindow( 0, flags ),
      //aboutDialog( new AboutDialog( this ) ),
      environment( &env ),
      viewMode( layoutMode )
{
    #ifdef DEBUG
        cout << indent()
             << "P2MainWindow[" << (int) this << "]::P2MainWindow("
             << (int) &env << ")" << endl;
    #endif

    //environment = &env;
    //viewMode = layoutMode;

    // Define the window icon.
    setWindowIcon( QIcon( P2GUI_ICON ) );

    // Create a checkmark icon for QMenu actions.
    checkmark = QIcon();
    QPixmap pixmap = QPixmap( ":/check.png", 0, Qt::AutoColor );
    checkmark.addPixmap( pixmap, QIcon::Active, QIcon::On );
    pixmap = QPixmap( ":/empty.png", 0, Qt::AutoColor );
    checkmark.addPixmap( pixmap, QIcon::Active, QIcon::Off );

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
        QBrush( QColor( COLOR__BACKGROUND ), Qt::SolidPattern ) ) );  // Background color.
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
        //setMinimumSize( QSize( SL5600__DISPLAY_WIDTH, SL5600__DISPLAY_HEIGHT ) );
        //setMaximumSize( QSize( SL5600__DISPLAY_WIDTH, SL5600__DISPLAY_HEIGHT ) );
    #endif

    createMenusAndToolbar( env );

    // Create the central widget.
    centralWidget = new P2CentralWidget( env );

    // Place the central widget in a scroll area.
    P2ScrollArea *scrollArea = new P2ScrollArea( centralWidget );
    setCentralWidget( scrollArea );

    // Changes in the environment are to have immediate effect on the window and
    // its contents.
    connect( &env, SIGNAL( changed() ), this, SLOT( refresh() ) );
}


void P2MainWindow::createMenusAndToolbar( const P2Environment &env )
{
    QAction *action;
    QIcon icon;

    // Build the menu bar.
    //QMenuBar *menubar = this->menuBar();
    QMenuBar *menubar = new QMenuBar( this );
    setMenuBar( menubar );
//menubar->setTearOffEnabled( false );
//menubar->setFrameStyle( QFrame::NoFrame );
    QToolBar *toolbar = this->addToolBar( tr( "Tool Bar" ) );
//toolbar->setTearOffEnabled( false );

    // File menu. //////////////////////////////////////////////////////////////

    QMenu *fileMenu = menubar->addMenu( tr( "&File" ) );

    // New.
    icon = QIcon( ":/fileNew.png" );
    action = new QAction( icon, tr( "&New" ), this );
    action->setStatusTip( tr( "Create a new file" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( fileNew() ) );
    action->setShortcut( Qt::CTRL + Qt::Key_N );
    fileMenu->addAction( action );
    #ifdef TOOLBAR__FILE__NEW
        toolbar->addAction( action );
    #endif

    // Open.
    icon = QIcon( ":/fileOpen.png" );
    action = new QAction( icon, tr( "&Open" ), this );
    action->setStatusTip( tr( "Open an existing file" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( fileOpen() ) );
    action->setShortcut( Qt::CTRL + Qt::Key_O );
    fileMenu->addAction( action );
    #ifdef TOOLBAR__FILE__OPEN
        toolbar->addAction( action );
    #endif

    // Save.
    icon = QIcon( ":/fileSave.png" );
    action = new QAction( icon, tr( "&Save" ), this );
    action->setStatusTip( tr( "Save the current document to disk" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( fileSave() ) );
    action->setShortcut( Qt::CTRL + Qt::Key_S );
    fileMenu->addAction( action );
    #ifdef TOOLBAR__FILE__SAVE
        toolbar->addAction( action );
    #endif

    // Save As.
    icon = QIcon( ":/fileSaveAs.png" );
    action = new QAction( icon, tr( "Save &As..." ), this );
    action->setStatusTip( tr( "Save the current document as a new file" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( fileSaveAs() ) );
    fileMenu->addAction( action );
    #ifdef TOOLBAR__FILE__SAVE_AS
        toolbar->addAction( action );
    #endif

    // Close.
    icon = QIcon( ":/fileClose.png" );
    action = new QAction( icon, tr( "&Close" ), this );
    action->setStatusTip( tr( "Close the current document" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( fileClose() ) );
    action->setShortcut( Qt::CTRL + Qt::Key_W );
    fileMenu->addAction( action );

    fileMenu->addSeparator();

    // Print.
    icon = QIcon( ":/filePrint.png" );
    action = new QAction( icon, tr( "&Print" ), this );
    action->setStatusTip( tr( "Print the document" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( filePrint() ) );
    action->setShortcut( Qt::CTRL + Qt::Key_P );
    fileMenu->addAction( action );
    #ifdef TOOLBAR__FILE__PRINT
        toolbar->addAction( action );
    #endif

    fileMenu->addSeparator();

    // Quit.
    icon = QIcon( ":/fileQuit.png" );
    action = new QAction( icon, tr( "&Quit" ), this );
    action->setStatusTip( tr( "Exit Phase2" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( fileQuit() ) );
    action->setShortcut( Qt::CTRL + Qt::Key_Q );
    fileMenu->addAction( action );
    #ifdef TOOLBAR__FILE__QUIT
        toolbar->addAction( action );
    #endif

    #if defined TOOLBAR__FILE_ACTIONS && \
        ( defined TOOLBAR__EDIT_ACTIONS \
       || defined TOOLBAR__VIEW_ACTIONS \
       || defined TOOLBAR__HELP_ACTIONS )
        toolbar->addSeparator();
    #endif

    // Edit menu. //////////////////////////////////////////////////////////////

    QMenu *editMenu = menubar->addMenu( tr( "&Edit" ) );

    // Undo.
    icon = QIcon( ":/editUndo.png" );
    action = new QAction( icon, tr( "&Undo" ), this );
    action->setStatusTip( tr( "Undo the last operation" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( editUndo() ) );
    action->setShortcut( Qt::CTRL + Qt::Key_Z );
    editMenu->addAction( action );
    #ifdef TOOLBAR__EDIT__UNDO
        toolbar->addAction( action );
    #endif

    // Redo.
    icon = QIcon( ":/editRedo.png" );
    action = new QAction( icon, tr( "&Redo" ), this );
    action->setStatusTip( tr( "Redo the last operation" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( editRedo() ) );
    action->setShortcut( Qt::CTRL + Qt::Key_Y );
    editMenu->addAction( action );
    #ifdef TOOLBAR__EDIT__REDO
        toolbar->addAction( action );
    #endif

    editMenu->addSeparator();

    // Cut.
    icon = QIcon( ":/editCut.png" );
    action = new QAction( icon, tr( "Cu&t" ), this );
    action->setStatusTip( tr( "Cut the current selection's contents to the "
                                "clipboard" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( editCut() ) );
    action->setShortcut( Qt::CTRL + Qt::Key_X );
    editMenu->addAction( action );
    #ifdef TOOLBAR__EDIT__CUT
        toolbar->addAction( action );
    #endif

    // Copy.
    icon = QIcon( ":/editCopy.png" );
    action = new QAction( icon, tr( "&Copy" ), this );
    action->setStatusTip( tr( "Copy the current selection's contents to the "
                                 "clipboard" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( editCopy() ) );
    action->setShortcut( Qt::CTRL + Qt::Key_C );
    editMenu->addAction( action );
    #ifdef TOOLBAR__EDIT__COPY
        toolbar->addAction( action );
    #endif

    // Paste.
    icon = QIcon( ":/editPaste.png" );
    action = new QAction( icon, tr( "&Paste" ), this );
    action->setStatusTip( tr( "Paste the clipboard's contents into the current "
                                  "selection" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( editPaste() ) );
    action->setShortcut( Qt::CTRL + Qt::Key_V );
    editMenu->addAction( action );
    #ifdef TOOLBAR__EDIT__PASTE
        toolbar->addAction( action );
    #endif

    // Delete.
    icon = QIcon( ":/editDelete.png" );
    action = new QAction( icon, tr( "&Delete" ), this );
    action->setStatusTip( tr( "Delete the current selection" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( editDelete() ) );
    action->setShortcut( Qt::Key_Delete );
    editMenu->addAction( action );
    #ifdef TOOLBAR__EDIT__DELETE
        toolbar->addAction( action );
    #endif

    // Rename.
    //icon = QIcon( ":/editDelete.png" );
    action = new QAction( tr( "&Rename" ), this );
    action->setStatusTip( tr( "Rename the current selection" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( editRename() ) );
    action->setShortcut( Qt::Key_F2 );
    editMenu->addAction( action );
    #ifdef TOOLBAR__EDIT__RENAME
        toolbar->addAction( action );
    #endif

    // editMenu->addSeparator();

    // Select All.
    // action = editMenu->addAction(
    //     "Select &All", this, SLOT( editSelectAll() ), Qt::CTRL + Qt::Key_A );

    // editMenu->addSeparator();

    // Find.
    // icon = QIcon( ":/editFind.png" );
    // action = editMenu->addAction(
    //     icon, "&Find", this, SLOT( edit() ), Qt::CTRL + Qt::Key_F );

    #if defined TOOLBAR__EDIT_ACTIONS && \
        ( defined TOOLBAR__VIEW_ACTIONS \
       || defined TOOLBAR__HELP_ACTIONS )
        toolbar->addSeparator();
    #endif

    // View menu. //////////////////////////////////////////////////////////////

    QMenu *viewMenu = menubar->addMenu( tr( "&View" ) );

//QIcon::Mode mode;  // QIcon::Active  QIcon::Disabled
//QIcon::State state;  // QIcon::On  QIcon::Off
//icon = QIcon();
//icon.addPixmap( QPixmap::fromImage( image ), mode, state );

    // Command Line.
    icon = QIcon( ":/viewCommandLine.png" );
    action = new QAction( icon, tr( "&Command Line" ), this );
    action->setStatusTip( tr( "Switch to the command line view" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( viewCommandLine() ) );
    action->setShortcut( Qt::Key_F7 );
    action->setEnabled( false );
    viewMenu->addAction( action );
    #ifdef TOOLBAR__VIEW__COMMAND_LINE
        toolbar->addAction( action );
    #endif

    viewMenu->addSeparator();

    // Back.
    icon = QIcon( ":/viewBack.png" );
    action = new QAction( icon, tr( "&Back" ), this );
    action->setStatusTip( tr( "Navigate to the previous item" ) );
    //action->setToolTip( tr( "Navigate to the previous item" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( viewBack() ) );
    action->setShortcut( Qt::ALT + Qt::Key_Left );
    viewMenu->addAction( action );
    #ifdef TOOLBAR__VIEW__BACK
        toolbar->addAction( action );
    #endif

    // Forward.
    icon = QIcon( ":/viewForward.png" );
    action = new QAction( icon, tr( "&Forward" ), this );
    action->setStatusTip( tr( "Navigate to the next item" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( viewForward() ) );
    action->setShortcut( Qt::ALT + Qt::Key_Right );
    viewMenu->addAction( action );
    #ifdef TOOLBAR__VIEW__FORWARD
        toolbar->addAction( action );
    #endif

    // New Window.
    icon = QIcon( ":/viewNewWindow.png" );
    action = new QAction( icon, tr( "&New Window" ), this );
    action->setStatusTip( tr( "Open a new top-level window" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( viewNewWindow() ) );
    //action->setShortcut( ... );
    viewMenu->addAction( action );
    #ifdef TOOLBAR__VIEW__NEW_WINDOW
        toolbar->addAction( action );
    #endif

    viewMenu->addSeparator();

    // Show Frames.
//QIcon::Mode mode;  // QIcon::Active  QIcon::Disabled
//QIcon::State state;  // QIcon::On  QIcon::Off

    //icon = QIcon( ":/viewShowFrames.png" );
    action = viewShowFramesAction = new QAction( checkmark, tr( "&Show Frames" ), this );
    //action = new QAction( tr( "&Show Frames" ), this );
    action->setStatusTip( tr( "Draw borders around all frames" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( viewShowFrames() ) );
    action->setShortcut( Qt::Key_F6 );
    action->setCheckable( true );
    action->setChecked( env.getIdleFrameVisibility() );
    viewMenu->addAction( action );
    #ifdef TOOLBAR__VIEW__SHOW_FRAMES
        toolbar->addAction( action );
    #endif

    action = viewShowNamesAction = new QAction( checkmark, tr( "Show Names" ), this );
    action->setStatusTip( tr( "Display lexical names" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( viewShowNames() ) );
    //action->setShortcut( Qt::Key_F6 );
    action->setCheckable( true );
    action->setChecked( env.getNameVisibility() );
    viewMenu->addAction( action );
    #ifdef TOOLBAR__VIEW__SHOW_NAMES
        toolbar->addAction( action );
    #endif

    #if defined TOOLBAR__VIEW_ACTIONS && ( defined TOOLBAR__HELP_ACTIONS )
        toolbar->addSeparator();
    #endif

    // Help menu. //////////////////////////////////////////////////////////////

    QMenu *helpMenu = menubar->addMenu( tr( "&Help" ) );

    // Manual.
    icon = QIcon( ":/helpManual.png" );
    action = new QAction( icon, tr( "&Manual" ), this );
    action->setStatusTip( tr( "Show Phase2's help manual" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( helpManual() ) );
    action->setShortcut( Qt::Key_F1 );
    helpMenu->addAction( action );
    #ifdef TOOLBAR__HELP__MANUAL
        toolbar->addAction( action );
    #endif

    // About Phase2.
    icon = QIcon( ":/helpAboutPhase2.png" );
    action = new QAction( icon, tr( "&About Phase2" ), this );
    action->setStatusTip( tr( "Show Phase2's About box" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( helpAboutPhase2() ) );
    helpMenu->addAction( action );
    #ifdef TOOLBAR__HELP__ABOUT_PHASE2
        toolbar->addAction( action );
    #endif
}


////////////////////////////////////////////////////////////////////////////////


void P2MainWindow::refresh()
{
    viewShowFramesAction->setChecked( environment->getIdleFrameVisibility() );
    viewShowNamesAction->setChecked( environment->getNameVisibility() );

    centralWidget->refresh( *environment );
    //update();

/*
    //~ temporary
    QWidget cover( this, 0 );
    cover.setGeometry( contentsRect() );
    cover.setVisible( true );
    cover.setVisible( false );
*/

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

    // This works, but with a hefty flicker.
    //setVisible( false );
    //setVisible( true );
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


void P2MainWindow::fileClose()
{
    cout << "void P2MainWindow::fileClose()" << endl;
}


void P2MainWindow::filePrint()
{
    cout << "void P2MainWindow::filePrint()" << endl;
}


void P2MainWindow::fileQuit()
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


void P2MainWindow::editDelete()
{
    cout << "void P2MainWindow::editDelete()" << endl;
}


void P2MainWindow::editRename()
{
    RenameDialog dialog( this, centralWidget->focusFrame() );
    dialog.exec();

    refresh();
}


void P2MainWindow::editSelectAll()
{
    cout << "void P2MainWindow::editSelectAll()" << endl;
}


void P2MainWindow::editFind()
{
    cout << "void P2MainWindow::editFind()" << endl;
}


////////////////////////////////////////////////////////////////////////////////


void P2MainWindow::viewCommandLine()
{
    cout << "void P2MainWindow::viewCommandLine()" << endl;
}


void P2MainWindow::viewForward()
{
    cout << "void P2MainWindow::viewForward()" << endl;
}


void P2MainWindow::viewBack()
{
    cout << "void P2MainWindow::viewBack()" << endl;
}


void P2MainWindow::viewNewWindow()
{
    newMainWindow( *environment );
}


void P2MainWindow::viewShowFrames()
{
    environment->setIdleFrameVisibility(
        !environment->getIdleFrameVisibility() );
}


void P2MainWindow::viewShowNames()
{
    environment->setNameVisibility( !environment->getNameVisibility() );
}


////////////////////////////////////////////////////////////////////////////////


void P2MainWindow::helpManual()
{
    cout << "void P2MainWindow::helpManual()" << endl;
}


void P2MainWindow::helpAboutPhase2()
{
    //if ( !aboutDialog || !aboutDialog->isVisible() )
   // {
   //     if ( aboutDialog )
   //         aboutDialog->deleteLater();

        AboutDialog *aboutDialog = new AboutDialog( this );
  //      connect( aboutDialog, SIGNAL( destroyed() ), this, SLOT( aboutDialogDestoyed() ) )
        aboutDialog->show();
  //  }
}


////////////////////////////////////////////////////////////////////////////////

/*
void P2MainWindow::aboutDialogDestroyed()
{
    aboutDialog = 0;
}
*/


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
