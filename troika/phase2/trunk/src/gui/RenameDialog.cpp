#include "RenameDialog.h"


////////////////////////////////////////////////////////////////////////////////


RenameDialog::RenameDialog( QWidget *parent, P2Frame *frameToRename )
    : QDialog( parent, 0 )
{
    #ifdef DEBUG
        cout << indent()
             << "RenameDialog[" << (int) this << "]::RenameDialog(...)" << endl;
    #endif

    this->frameToRename = frameToRename;

    // Input/output in this window will not proceed until the user either
    // submits a new name for the focus frame or clicks cancel / close.
    setModal( true );

    setWindowTitle( tr( "Rename item" ) );

    // Name field ----------------------

    QGroupBox *nameBox = new QGroupBox( tr( "New name" ), this );
    nameBox->setAlignment( Qt::AlignLeft );
    nameBox->setCheckable( false );
    nameBox->setFlat( false );

    nameField = new QTextEdit( frameToRename->objectName() );
    //nameField->setAcceptRichText( false );
    //nameField->setWrapMode( QTextEdit::NoWrap );

    //~
    nameField->setFixedSize( QSize( 180, 30 ) );
    //nameField->setFixedSize( QSize( 180, nameField->minimumSize().height() ) );
    //nameField->resize( 50, 20 );//nameField->height() );
    //nameField->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

    QVBoxLayout *nameBoxLayout = new QVBoxLayout;
    nameBoxLayout->addWidget( nameField );
    nameBox->setLayout(nameBoxLayout);

    // Buttons -------------------------

    QPushButton *okButton = new QPushButton( tr( "OK" ), this );
    QPushButton *cancelButton = new QPushButton( tr( "Cancel" ), this );

    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    // Overall layout ------------------

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget( okButton );
    hLayout->addWidget( cancelButton );
    QWidget *hLayoutWidget = new QWidget;
    hLayoutWidget->setLayout( hLayout );

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget( nameBox );
    vLayout->addWidget( hLayoutWidget );

    setLayout( vLayout );
}


void RenameDialog::accept()
{
    cout << indent()
         << "void RenameDialog::accept()" << endl;

    //~ Check for name collision.

    // Note: this will remove an object from its dictionary if nameField is empty.
    frameToRename->setObjectName( nameField->toPlainText() );

    //emit( accepted() );
    emit( done( 0 ) ); hide();
}


void RenameDialog::done( int r )
{
    cout << indent()
         << "void RenameDialog::done( int r )" << endl;
}


void RenameDialog::reject()
{
    cout << indent()
         << "void RenameDialog::reject()" << endl;

    //emit( accepted() );
    emit( done( 0 ) ); hide();
}

