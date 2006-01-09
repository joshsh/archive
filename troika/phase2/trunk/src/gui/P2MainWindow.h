#ifndef P2MainWindow_h
#define P2MainWindow_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class P2MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    P2MainWindow( QWidget* parent );

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

    /** Turns inactive frame visibility on or off. */
    void viewCommandLine();
    void viewForward();
    void viewBack();
    void viewShowFrames();

    void helpManual();
    void helpAboutPhase2();

private:

    void createMenusAndToolbar();

    QIcon checkmark;

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2MainWindow_h
