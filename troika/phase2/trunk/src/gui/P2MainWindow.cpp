#include "P2MainWindow.h"

// ? WFlags


////////////////////////////////////////////////////////////////////////////////


P2MainWindow::P2MainWindow( QWidget* parent, Qt::WFlags fl )
    : QMainWindow( parent, fl )
{
    #ifdef DEBUG
        cout << "P2MainWindow[" << (int) this << "]::P2MainWindow( "
             << (int) parent << ", "
             << fl << " )" << endl;
    #endif

    environment = new P2Environment();

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
        setGeometry( QRect( 500, 500, 240, 320 ) );
        setMinimumSize( QSize( 240, 320 ) );
        setMaximumSize( QSize( 240, 320 ) );
    #endif

    QMenuBar *menubar = this->menuBar();
    QMenu *fileMenu = menubar->addMenu( "&File" );
    fileMenu->addAction( "&Quit", this, SLOT( quit() ), 0 );
    QMenu *viewMenu = menubar->addMenu( "&View" );
    QMenu *frameVisibilityMenu = viewMenu->addMenu( "Frames" );
    frameVisibilityMenu->addAction( "Always visible", this,
        SLOT( showIdleFrames() ), 0 );
    frameVisibilityMenu->addAction( "Hidden when idle", this,
        SLOT( hideIdleFrames() ), 0 );
    QAction *newAction = viewMenu->addAction( "new action" );
    newAction->setCheckable( true );
    newAction->setChecked( true );

    centralWidget = new P2CentralWidget( environment );
    centralWidget->setParent( this );

    //centralWidget = new P2CentralWidget( this, environment );
    setCentralWidget( centralWidget );

    setWindowIcon( QIcon( P2GUI_ICON ) );

    // QTextView *tv = new QTextView( "Salut!", 0, this, "Log viewer" );
    // setCentralWidget( tv );

    // QMultiLineEdit *mle = new QMultiLineEdit( this );
    // mle->setText( "Salut!" );
    // setCentralWidget( mle );
}


P2MainWindow::~P2MainWindow()
{
    delete environment;
    delete centralWidget;
}


void P2MainWindow::quit( )
{
    this->close( );
}


void P2MainWindow::showIdleFrames( )
{
    environment->idleFramesAreVisible = true;

    QWidget cover( this, 0 );
    cover.setGeometry( contentsRect() );
    cover.setVisible( true );
    cover.setVisible( false );
}


void P2MainWindow::hideIdleFrames( )
{
    environment->idleFramesAreVisible = false;

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
