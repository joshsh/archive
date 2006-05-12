#include <widgets/P2MainWindow.h>
#include <widgets/dialogs/RenameDialog.h>

    #include <widgets/P2TabWidget.h>

////////////////////////////////////////////////////////////////////////////////


P2MainWindow::P2MainWindow( P2Binder &b, QString fileName )
    : QMainWindow(),
      //aboutDialog( new AboutDialog( this ) ),
      binder( &b ),
      environment( b.getEnv() ),
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

    QString title( "Phase2 GUI" );

    #ifdef ARM_LINUX
    title += " (ARM)";
    #endif

    if ( !fileName.isNull() )
        title += " -- " + fileName;

    setWindowTitle( title );

    // Only limit the size of the main window if it's to be displayed on a
    // large screen (otherwise allow the application to maximize it).
    #ifndef ARM_COMPILE
        setGeometry( QRect( 500, 500, SL5600__DISPLAY_WIDTH, SL5600__DISPLAY_HEIGHT ) );
        //setMinimumSize( QSize( SL5600__DISPLAY_WIDTH, SL5600__DISPLAY_HEIGHT ) );
        //setMaximumSize( QSize( SL5600__DISPLAY_WIDTH, SL5600__DISPLAY_HEIGHT ) );
    #endif

    viewStack = new P2ViewStack( environment, this );
    viewStack->addView( environment__root( environment->getEnv() ) );
    setCentralWidget( viewStack );

    connect(
        viewStack,  SIGNAL( backEnabled() ),
        this,       SLOT( backEnabled() ) );
    connect(
        viewStack,  SIGNAL( backDisabled() ),
        this,       SLOT( backDisabled() ) );
    connect(
        viewStack,  SIGNAL( forwardEnabled() ),
        this,       SLOT( forwardEnabled() ) );
    connect(
        viewStack,  SIGNAL( forwardDisabled() ),
        this,       SLOT( forwardDisabled() ) );

    createMenusAndToolbar( *environment );

    connect(
        viewStack,      SIGNAL( hasFocusObject( bool ) ),
        editCopyAction, SLOT( setEnabled( bool ) ) );
    connect(
        viewStack,      SIGNAL( hasFocusObject( bool ) ),
        editCutAction,  SLOT( setEnabled( bool ) ) );

//    singleView = new P2View(
//        environment__root( environment->getEnv() ),
//        environment );
//    setCentralWidget( singleView );

    // Changes in the environment are to have immediate effect on the window and
    // its contents.
    connect( environment, SIGNAL( changed() ), this, SLOT( refresh() ) );
}


void P2MainWindow::createMenusAndToolbar( const P2Environment &env )
{
    QIcon icon;

    // Build the menu bar.
    QMenuBar *menubar = new QMenuBar( this );
    setMenuBar( menubar );
    menubar->setFocusPolicy( Qt::NoFocus );

    QToolBar *toolbar = 0;

    if ( TOOLBAR )
    {
        toolbar = new QToolBar( tr( "Tool Bar" ) );
        this->addToolBar( Qt::BottomToolBarArea, toolbar );
        toolbar->setMovable( false );
        //toolbar->setTearOffEnabled( false );
        toolbar->setFocusPolicy( Qt::NoFocus );
    }

    // File menu. //////////////////////////////////////////////////////////////

    QMenu *fileMenu = menubar->addMenu( tr( "&File" ) );

    // New.
    icon = QIcon( ":/fileNew.png" );
    QAction *fileNewAction = new QAction( icon, tr( "&New" ), this );
    fileNewAction->setStatusTip( tr( "Create a new file" ) );
    connect( fileNewAction, SIGNAL( triggered() ), this, SLOT( fileNew() ) );
    fileNewAction->setShortcut( Qt::CTRL + Qt::Key_N );
    fileNewAction->setEnabled( false );
    fileMenu->addAction( fileNewAction );
    if ( TOOLBAR & TOOLBAR__FILE & TOOLBAR__FILE__NEW )
        toolbar->addAction( fileNewAction );

    // Open.
    icon = QIcon( ":/fileOpen.png" );
    QAction *fileOpenAction = new QAction( icon, tr( "&Open" ), this );
    fileOpenAction->setStatusTip( tr( "Open an existing file" ) );
    connect( fileOpenAction, SIGNAL( triggered() ), this, SLOT( fileOpen() ) );
    fileOpenAction->setShortcut( Qt::CTRL + Qt::Key_O );
    fileOpenAction->setEnabled( false );
    fileMenu->addAction( fileOpenAction );
    if ( TOOLBAR & TOOLBAR__FILE & TOOLBAR__FILE__OPEN )
        toolbar->addAction( fileOpenAction );

    // Save.
    icon = QIcon( ":/fileSave.png" );
    QAction *fileSaveAction = new QAction( icon, tr( "&Save" ), this );
    fileSaveAction->setStatusTip( tr( "Save the current document to disk" ) );
    connect( fileSaveAction, SIGNAL( triggered() ), this, SLOT( fileSave() ) );
    fileSaveAction->setShortcut( Qt::CTRL + Qt::Key_S );
    fileSaveAction->setEnabled( false );
    fileMenu->addAction( fileSaveAction );
    if ( TOOLBAR & TOOLBAR__FILE & TOOLBAR__FILE__SAVE )
        toolbar->addAction( fileSaveAction );

    // Save As.
    icon = QIcon( ":/fileSaveAs.png" );
    QAction *fileSaveAsAction = new QAction( icon, tr( "Save &As..." ), this );
    fileSaveAsAction->setStatusTip( tr( "Save the current document as a new file" ) );
    connect( fileSaveAsAction, SIGNAL( triggered() ), this, SLOT( fileSaveAs() ) );
    fileSaveAsAction->setEnabled( false );
    fileMenu->addAction( fileSaveAsAction );
    if ( TOOLBAR & TOOLBAR__FILE & TOOLBAR__FILE__SAVE_AS )
        toolbar->addAction( fileSaveAsAction );

    // Close.
    icon = QIcon( ":/fileClose.png" );
    QAction *fileCloseAction = new QAction( icon, tr( "&Close" ), this );
    fileCloseAction->setStatusTip( tr( "Close the current document" ) );
    connect( fileCloseAction, SIGNAL( triggered() ), this, SLOT( fileClose() ) );
    fileCloseAction->setShortcut( Qt::CTRL + Qt::Key_W );
    fileCloseAction->setEnabled( false );
    fileMenu->addAction( fileCloseAction );

    fileMenu->addSeparator();

    // Print.
    icon = QIcon( ":/filePrint.png" );
    QAction *filePrintAction = new QAction( icon, tr( "&Print" ), this );
    filePrintAction->setStatusTip( tr( "Print the document" ) );
    connect( filePrintAction, SIGNAL( triggered() ), this, SLOT( filePrint() ) );
    filePrintAction->setShortcut( Qt::CTRL + Qt::Key_P );
    filePrintAction->setEnabled( false );
    fileMenu->addAction( filePrintAction );
    if ( TOOLBAR & TOOLBAR__FILE & TOOLBAR__FILE__PRINT )
        toolbar->addAction( filePrintAction );

    fileMenu->addSeparator();

    // Quit.
    icon = QIcon( ":/fileQuit.png" );
    QAction *fileQuitAction = new QAction( icon, tr( "&Quit" ), this );
    fileQuitAction->setStatusTip( tr( "Exit Phase2" ) );
    connect( fileQuitAction, SIGNAL( triggered() ), this, SLOT( fileQuit() ) );
    fileQuitAction->setShortcut( Qt::CTRL + Qt::Key_Q );
    fileMenu->addAction( fileQuitAction );
    if ( TOOLBAR & TOOLBAR__FILE & TOOLBAR__FILE__QUIT )
        toolbar->addAction( fileQuitAction );

    if ( TOOLBAR & TOOLBAR__FILE &
        ( TOOLBAR__EDIT | TOOLBAR__VIEW | TOOLBAR__HELP ) )
        toolbar->addSeparator();

    // Edit menu. //////////////////////////////////////////////////////////////

    QMenu *editMenu = menubar->addMenu( tr( "&Edit" ) );
    editMenu->setFocusPolicy( Qt::NoFocus );

    // Undo.
    icon = QIcon( ":/editUndo.png" );
    QAction *editUndoAction = new QAction( icon, tr( "&Undo" ), this );
    editUndoAction->setStatusTip( tr( "Undo the last operation" ) );
    connect( editUndoAction, SIGNAL( triggered() ), this, SLOT( editUndo() ) );
    editUndoAction->setShortcut( Qt::CTRL + Qt::Key_Z );
    editUndoAction->setEnabled( false );
    editMenu->addAction( editUndoAction );
    if ( TOOLBAR & TOOLBAR__EDIT & TOOLBAR__EDIT__UNDO )
        toolbar->addAction( editUndoAction );

    // Redo.
    icon = QIcon( ":/editRedo.png" );
    QAction *editRedoAction = new QAction( icon, tr( "&Redo" ), this );
    editRedoAction->setStatusTip( tr( "Redo the last operation" ) );
    connect( editRedoAction, SIGNAL( triggered() ), this, SLOT( editRedo() ) );
    editRedoAction->setShortcut( Qt::CTRL + Qt::Key_Y );
    editRedoAction->setEnabled( false );
    editMenu->addAction( editRedoAction );
    if ( TOOLBAR & TOOLBAR__EDIT & TOOLBAR__EDIT__REDO )
        toolbar->addAction( editRedoAction );

    editMenu->addSeparator();

    // Cut.
    icon = QIcon( ":/editCut.png" );
    editCutAction = new QAction( icon, tr( "Cu&t" ), this );
    editCutAction->setStatusTip( tr( "Cut the current selection's contents to the "
                                "clipboard" ) );
    connect( editCutAction, SIGNAL( triggered() ), this, SLOT( editCut() ) );
    editCutAction->setShortcut( Qt::CTRL + Qt::Key_X );
    editCutAction->setEnabled( false );
    editMenu->addAction( editCutAction );
    if ( TOOLBAR & TOOLBAR__EDIT & TOOLBAR__EDIT__CUT )
        toolbar->addAction( editCutAction );

    // Copy.
    icon = QIcon( ":/editCopy.png" );
    editCopyAction = new QAction( icon, tr( "&Copy" ), this );
    editCopyAction->setStatusTip( tr( "Copy the current selection's contents to the "
                                 "clipboard" ) );
    connect( editCopyAction, SIGNAL( triggered() ), this, SLOT( editCopy() ) );
    editCopyAction->setShortcut( Qt::CTRL + Qt::Key_C );
    editCopyAction->setEnabled( false );
    editMenu->addAction( editCopyAction );
    if ( TOOLBAR & TOOLBAR__EDIT & TOOLBAR__EDIT__COPY )
        toolbar->addAction( editCopyAction );

    // Paste.
    icon = QIcon( ":/editPaste.png" );
    editPasteAction = new QAction( icon, tr( "&Paste" ), this );
    editPasteAction->setStatusTip( tr( "Paste the clipboard's contents into the current "
                                  "selection" ) );
    connect( editPasteAction, SIGNAL( triggered() ), this, SLOT( editPaste() ) );
    connect(
        environment,        SIGNAL( hasClipboardObject( bool ) ),
        editPasteAction,    SLOT( setEnabled( bool ) ) );
    editPasteAction->setShortcut( Qt::CTRL + Qt::Key_V );
    editPasteAction->setEnabled( false );
    editMenu->addAction( editPasteAction );
    if ( TOOLBAR & TOOLBAR__EDIT & TOOLBAR__EDIT__PASTE )
        toolbar->addAction( editPasteAction );

    // Delete.
    icon = QIcon( ":/editDelete.png" );
    QAction *editDeleteAction = new QAction( icon, tr( "&Delete" ), this );
    editDeleteAction->setStatusTip( tr( "Delete the current selection" ) );
    connect( editDeleteAction, SIGNAL( triggered() ), this, SLOT( editDelete() ) );
    editDeleteAction->setShortcut( Qt::Key_Delete );
    editDeleteAction->setEnabled( false );
    editMenu->addAction( editDeleteAction );
    if ( TOOLBAR & TOOLBAR__EDIT & TOOLBAR__EDIT__DELETE )
        toolbar->addAction( editDeleteAction );

    // Rename.
    //icon = QIcon( ":/editDelete.png" );
    QAction *editRenameAction = new QAction( tr( "&Rename" ), this );
    editRenameAction->setStatusTip( tr( "Rename the current selection" ) );
    connect( editRenameAction, SIGNAL( triggered() ), this, SLOT( editRename() ) );
    editRenameAction->setShortcut( Qt::Key_F2 );
    editMenu->addAction( editRenameAction );
    if ( TOOLBAR & TOOLBAR__EDIT & TOOLBAR__EDIT__RENAME )
        toolbar->addAction( editRenameAction );

    // editMenu->addSeparator();

    // Select All.
    // action = editMenu->addAction(
    //     "Select &All", this, SLOT( editSelectAll() ), Qt::CTRL + Qt::Key_A );

    // editMenu->addSeparator();

    // Find.
    // icon = QIcon( ":/editFind.png" );
    // action = editMenu->addAction(
    //     icon, "&Find", this, SLOT( edit() ), Qt::CTRL + Qt::Key_F );

    if ( TOOLBAR__EDIT &
        ( TOOLBAR__VIEW | TOOLBAR__HELP ) )
        toolbar->addSeparator();

    // View menu. //////////////////////////////////////////////////////////////

    QMenu *viewMenu = menubar->addMenu( tr( "&View" ) );

//QIcon::Mode mode;  // QIcon::Active  QIcon::Disabled
//QIcon::State state;  // QIcon::On  QIcon::Off
//icon = QIcon();
//icon.addPixmap( QPixmap::fromImage( image ), mode, state );

    // Command Line.
    icon = QIcon( ":/viewCommandLine.png" );
    QAction *viewCommandLineAction = new QAction( icon, tr( "&Command Line" ), this );
    viewCommandLineAction->setStatusTip( tr( "Switch to the command line view" ) );
    connect( viewCommandLineAction, SIGNAL( triggered() ), this, SLOT( viewCommandLine() ) );
    viewCommandLineAction->setShortcut( Qt::Key_F7 );
    viewCommandLineAction->setEnabled( false );
    viewMenu->addAction( viewCommandLineAction );
    if ( TOOLBAR & TOOLBAR__VIEW & TOOLBAR__VIEW__COMMAND_LINE )
    toolbar->addAction( viewCommandLineAction );

    viewMenu->addSeparator();

    // Back.
    icon = QIcon( ":/viewBack.png" );
    viewBackAction = new QAction( icon, tr( "&Back" ), this );
    viewBackAction->setStatusTip( tr( "Navigate to the previous item" ) );
    //viewBackAction->setToolTip( tr( "Navigate to the previous item" ) );
    connect( viewBackAction, SIGNAL( triggered() ), this, SLOT( viewBack() ) );
    viewBackAction->setShortcut( Qt::ALT + Qt::Key_Left );
    viewBackAction->setEnabled( false );
    viewMenu->addAction( viewBackAction );
    if ( TOOLBAR & TOOLBAR__VIEW & TOOLBAR__VIEW__BACK )
        toolbar->addAction( viewBackAction );

    // Forward.
    icon = QIcon( ":/viewForward.png" );
    viewForwardAction = new QAction( icon, tr( "&Forward" ), this );
    viewForwardAction->setStatusTip( tr( "Navigate to the next item" ) );
    connect( viewForwardAction, SIGNAL( triggered() ), this, SLOT( viewForward() ) );
    viewForwardAction->setShortcut( Qt::ALT + Qt::Key_Right );
    viewForwardAction->setEnabled( false );
    viewMenu->addAction( viewForwardAction );
    if ( TOOLBAR & TOOLBAR__VIEW & TOOLBAR__VIEW__FORWARD )
        toolbar->addAction( viewForwardAction );

    // New Window.
    icon = QIcon( ":/viewNewWindow.png" );
    QAction *viewNewWindowAction = new QAction( icon, tr( "&New Window" ), this );
    viewNewWindowAction->setStatusTip( tr( "Open a new top-level window" ) );
    connect( viewNewWindowAction, SIGNAL( triggered() ), this, SLOT( viewNewWindow() ) );
    //viewNewWindowAction->setShortcut( ... );
    viewMenu->addAction( viewNewWindowAction );
    if ( TOOLBAR & TOOLBAR__VIEW & TOOLBAR__VIEW__NEW_WINDOW )
        toolbar->addAction( viewNewWindowAction );

    viewMenu->addSeparator();

    // Split Horizontal.
    icon = QIcon( ":/viewSplitHorizontal.png" );
    QAction *viewSplitHorizontalAction = new QAction( icon, tr( "Split &Horizontal" ), this );
    viewSplitHorizontalAction->setStatusTip( tr( "Split the current view horizontally" ) );
    connect( viewSplitHorizontalAction, SIGNAL( triggered() ), this, SLOT( viewSplitHorizontal() ) );
    //viewSplitHorizontalAction->setShortcut( ... );
    viewSplitHorizontalAction->setEnabled( false );
    viewMenu->addAction( viewSplitHorizontalAction );
    if ( TOOLBAR & TOOLBAR__VIEW & TOOLBAR__VIEW__SPLIT_HORIZONTAL )
        toolbar->addAction( viewSplitHorizontalAction );

    // Split Vertical.
    icon = QIcon( ":/viewSplitVertical.png" );
    QAction *viewSplitVerticalAction = new QAction( icon, tr( "Split &Vertical" ), this );
    viewSplitVerticalAction->setStatusTip( tr( "Split the current view vertically" ) );
    connect( viewSplitVerticalAction, SIGNAL( triggered() ), this, SLOT( viewSplitVertical() ) );
    //viewSplitVerticalAction->setShortcut( ... );
    viewSplitVerticalAction->setEnabled( false );
    viewMenu->addAction( viewSplitVerticalAction );
    if ( TOOLBAR & TOOLBAR__VIEW & TOOLBAR__VIEW__SPLIT_VERTICAL )
        toolbar->addAction( viewSplitVerticalAction );

    // Close Current View.
    icon = QIcon( ":/viewCloseCurrentView.png" );
    QAction *viewCloseCurrentViewAction = new QAction( icon, tr( "Close Current View" ), this );
    viewCloseCurrentViewAction->setStatusTip( tr( "Close the current view" ) );
    connect( viewCloseCurrentViewAction, SIGNAL( triggered() ), this, SLOT( viewCloseCurrentView() ) );
    //viewCloseCurrentViewAction->setShortcut( ... );
    viewCloseCurrentViewAction->setEnabled( false );
    viewMenu->addAction( viewCloseCurrentViewAction );
    if ( TOOLBAR & TOOLBAR__VIEW & TOOLBAR__VIEW__CLOSE_CURRENT_VIEW )
        toolbar->addAction( viewCloseCurrentViewAction );

    viewMenu->addSeparator();

    // Show Frames.
//QIcon::Mode mode;  // QIcon::Active  QIcon::Disabled
//QIcon::State state;  // QIcon::On  QIcon::Off

    //icon = QIcon( ":/viewShowFrames.png" );
    viewShowFramesAction = new QAction( checkmark, tr( "&Show Frames" ), this );
    //viewShowFramesAction = new QAction( tr( "&Show Frames" ), this );
    viewShowFramesAction->setStatusTip( tr( "Draw borders around all frames" ) );
    connect( viewShowFramesAction, SIGNAL( triggered() ), this, SLOT( viewShowFrames() ) );
    viewShowFramesAction->setShortcut( Qt::Key_F6 );
    viewShowFramesAction->setCheckable( true );
    viewShowFramesAction->setChecked( env.getIdleFrameVisibility() );
    viewMenu->addAction( viewShowFramesAction );
    if ( TOOLBAR & TOOLBAR__VIEW & TOOLBAR__VIEW__SHOW_FRAMES )
        toolbar->addAction( viewShowFramesAction );

    viewShowNamesAction = new QAction( checkmark, tr( "Show Names" ), this );
    viewShowNamesAction->setStatusTip( tr( "Display lexical names" ) );
    connect( viewShowNamesAction, SIGNAL( triggered() ), this, SLOT( viewShowNames() ) );
    //viewShowNamesAction->setShortcut( Qt::Key_F6 );
    viewShowNamesAction->setCheckable( true );
    viewShowNamesAction->setChecked( env.getNameVisibility() );
    viewMenu->addAction( viewShowNamesAction );
    if ( TOOLBAR & TOOLBAR__VIEW & TOOLBAR__VIEW__SHOW_NAMES )
        toolbar->addAction( viewShowNamesAction );

    if ( TOOLBAR__VIEW & ( TOOLBAR__HELP ) )
        toolbar->addSeparator();

    // Help menu. //////////////////////////////////////////////////////////////

    QMenu *helpMenu = menubar->addMenu( tr( "&Help" ) );

    // Manual.
    icon = QIcon( ":/helpManual.png" );
    QAction *helpManualAction = new QAction( icon, tr( "&Manual" ), this );
    helpManualAction->setStatusTip( tr( "Show Phase2's help manual" ) );
    connect( helpManualAction, SIGNAL( triggered() ), this, SLOT( helpManual() ) );
    helpManualAction->setShortcut( Qt::Key_F1 );
    helpManualAction->setEnabled( false );
    helpMenu->addAction( helpManualAction );
    if ( TOOLBAR & TOOLBAR__HELP & TOOLBAR__HELP__MANUAL )
        toolbar->addAction( helpManualAction );

    // About Phase2.
    icon = QIcon( ":/helpAboutPhase2.png" );
    QAction *helpAboutPhase2Action = new QAction( icon, tr( "&About Phase2" ), this );
    helpAboutPhase2Action->setStatusTip( tr( "Show Phase2's About box" ) );
    connect( helpAboutPhase2Action, SIGNAL( triggered() ), this, SLOT( helpAboutPhase2() ) );
    helpMenu->addAction( helpAboutPhase2Action );
    if ( TOOLBAR & TOOLBAR__HELP & TOOLBAR__HELP__ABOUT_PHASE2 )
        toolbar->addAction( helpAboutPhase2Action );

//menubar->addAction( viewBackAction );
//menubar->addAction( viewForwardAction );
}


////////////////////////////////////////////////////////////////////////////////


void P2MainWindow::refresh()
{
    viewShowFramesAction->setChecked( environment->getIdleFrameVisibility() );
    viewShowNamesAction->setChecked( environment->getNameVisibility() );

    viewStack->refresh( *environment );
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
    environment->setClipboardObject( viewStack->focusObject() );
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
    P2Widget *w = viewStack->focusWidget();

    if ( w )
    {
        RenameDialog dialog( this, w );
        dialog.exec();

        refresh();
    }
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
    viewStack->goForward();
}


void P2MainWindow::viewBack()
{
    viewStack->goBack();
}


void P2MainWindow::viewNewWindow()
{
    newMainWindow( *binder );
}


void P2MainWindow::viewSplitHorizontal()
{
    cout << "void P2MainWindow::viewSplitHorizontal()" << endl;
}


void P2MainWindow::viewSplitVertical()
{
    cout << "void P2MainWindow::viewSplitVertical()" << endl;
}


void P2MainWindow::viewCloseCurrentView()
{
    cout << "void P2MainWindow::viewCloseCurrentView()" << endl;
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


void P2MainWindow::
backEnabled()
{
    viewBackAction->setEnabled( true );
}


void P2MainWindow::
backDisabled()
{
    viewBackAction->setEnabled( false );
}


void P2MainWindow::
forwardEnabled()
{
    viewForwardAction->setEnabled( true );
}


void P2MainWindow::
forwardDisabled()
{
    viewForwardAction->setEnabled( false );
}


/*
void P2MainWindow::aboutDialogDestroyed()
{
    aboutDialog = 0;
}
*/


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
