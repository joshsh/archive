#include "AboutDialog.h"


////////////////////////////////////////////////////////////////////////////////


AboutDialog::AboutDialog( QWidget *parent )
    : QDialog( parent, 0 )
{
    #ifdef DEBUG
        cout << indent()
             << "AboutDialog[" << (int) this << "]::AboutDialog("
             << (int) parent << ")" << endl;
    #endif

    setModal( false );

    setWindowTitle( tr( "About Phase2" ) );

    // Tabs ----------------------------

    QScrollArea *aboutTab = new QScrollArea;
    aboutTab->setWidget(
        new QLabel( "Some words about Phase2." ) );

    QScrollArea *authorsTab = new QScrollArea;
    authorsTab->setWidget(
        new QLabel( "Some words about Josh and his amazing Jazz-dancing monkeys" ) );

    QScrollArea *licenseTab = new QScrollArea;
    licenseTab->setWidget(
        new QLabel( "The license" ) );

    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->addTab( aboutTab, tr( "About" ) );
    tabWidget->addTab( authorsTab, tr( "Author" ) );
    tabWidget->addTab( licenseTab, tr( "License" ) );

    // Button --------------------------

    QPushButton *closeButton = new QPushButton( QIcon( ":/fileClose.png" ), tr( "&Close" ) );

    connect( closeButton, SIGNAL( clicked() ), this, SLOT( hide() ) );

    // Layout --------------------------

    QHBoxLayout *titleLayout = new QHBoxLayout;
    //QImage *image = new QImage( ":/phase2-med.png" );
    titleLayout->addWidget(
        new QLabel( "<b>Phase2 version 0.5</b>" ) );

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch( 1 );
    buttonLayout->addWidget( closeButton );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout( titleLayout );
    mainLayout->addWidget( tabWidget );
    mainLayout->addLayout( buttonLayout );
    setLayout( mainLayout );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
