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

    // Build the menu bar.
    QMenuBar *menubar = this->menuBar();
//menubar->setFrameStyle( QFrame::NoFrame );
    QMenu *fileMenu = menubar->addMenu( "&File" );
    fileMenu->addAction( "&Quit", this, SLOT( quit() ), 0 );
    QMenu *viewMenu = menubar->addMenu( "&View" );
    QAction *newAction = viewMenu->addAction( "Show idle &frames", this,
        SLOT( toggleFrameVisibility() ), 0 );
    newAction->setCheckable( true );
    newAction->setChecked( false );

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


void P2MainWindow::quit( )
{
    this->close( );
}


void P2MainWindow::toggleFrameVisibility()
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

