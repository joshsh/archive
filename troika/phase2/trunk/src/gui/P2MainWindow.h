#ifndef P2MainWindow_h
#define P2MainWindow_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2CentralWidget.h"
#include "P2ScrollArea.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class P2MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    P2MainWindow( QWidget* parent, Qt::WFlags flags = 0 );

    void refresh();

public slots:

    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void fileClose();
    void filePrint();
    void fileQuit();

    void editUndo();
    void editRedo();
    void editCut();
    void editCopy();
    void editPaste();
    void editDelete();
    void editSelectAll();
    void editFind();

    void viewCommandLine();
    void viewForward();
    void viewBack();
    void viewNewWindow();
    /** Turns inactive frame visibility on or off. */
    void viewShowFrames();
    /** Turns label display on or off. */
    void viewShowNames();

    void helpManual();
    void helpAboutPhase2();

private:

    QIcon checkmark;
    P2CentralWidget *centralWidget;
    //QScrollArea *scrollArea;

    // Checkable actions which are bound to the environment.
    QAction *viewShowNamesAction, *viewShowFramesAction;

    void createMenusAndToolbar();

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2MainWindow_h
