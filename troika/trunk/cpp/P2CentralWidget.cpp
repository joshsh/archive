#include "P2CentralWidget.h"  // P2CentralWidget

#include <qapplication.h>  // qApp

#include <qwidget.h>  // QWidget
#include <qpushbutton.h>  // QPushButton

#include <qpalette.h>  // QColorGroup
#include <qcolor.h>  // QColor

////////////////////////////////////////////////////////////////////////////////

P2CentralWidget::P2CentralWidget( QWidget *parent, const char *name )
        : QWidget( parent, name )
{
    QColorGroup cg = this->colorGroup();    // if this is a widget.
    cg.setColor(QColorGroup::Background, Qt::white);

    // But how do you re-define QWidget::PaletteBackground?
    setBackgroundMode(QWidget::PaletteBackground);

    //setMinimumSize( 240, 320 );
    //setMaximumSize( 240, 320 );

    // "The widget name has nothing to do with the button text; it just happens
    // to be similar in this case."
    QPushButton *quit = new QPushButton( "Quit", this, "quit" );
    quit->setGeometry( 62, 40, 75, 30 );
    quit->setFont( QFont( "Times", 18, QFont::Bold ) );

    // This my button.
    QPushButton *bleah = new QPushButton( "Hide", this, "bleah" );
    bleah->setGeometry( 62, 10, 75, 30 );
    bleah->setFont( QFont( "Times", 18, QFont::Bold ) );

    //QTextEdit *editor = new QTextEdit(this, "editor");
    //editor->setGeometry( 20, 70, 50, 50 );


    // "Because the MyWidget class doesn't know about the application object, it
    // has to connect to Qt's pointer to it, qApp."
    connect( quit, SIGNAL(clicked()), qApp, SLOT(quit()) );

    connect( bleah, SIGNAL(clicked()), quit, SLOT(hide()) );
}
