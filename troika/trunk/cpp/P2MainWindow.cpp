#include <qpopupmenu.h>
#include <qmenubar.h>
#include "P2MainWindow.h"

#include <qpushbutton.h>

//#include <qnamespace.h>

// For QColorGroup.
#include <qpalette.h>

// For QColor.
#include <qcolor.h>

class MyWidget : public QWidget
{
public:
    MyWidget( QWidget *parent=0, const char *name=0 );
};
MyWidget::MyWidget( QWidget *parent, const char *name )
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









P2MainWindow::P2MainWindow(QWidget* parent, const char* name, WFlags fl)
    : QMainWindow(parent, name, fl)
{
    setCaption("Troika Edit");
    //setGeometry( 40, 40, 300, 300 );
    setMinimumSize(240, 320);
    setMaximumSize(240, 320);

    QMenuBar *menubar = this->menuBar();
    QPopupMenu *mnuFile = new QPopupMenu(this, "MenuFile");
    menubar->insertItem("&File", mnuFile);
    mnuFile->insertItem("&Quit", this, SLOT(cmdFileQuit()), 0, 1);

    MyWidget *w = new MyWidget(this, "central widget");

    // Origin at (10,10).
    //w.setGeometry( 10, 10, 200, 120 );

    setCentralWidget(w);
}

void P2MainWindow::cmdFileQuit()
{
    this->close();
}
