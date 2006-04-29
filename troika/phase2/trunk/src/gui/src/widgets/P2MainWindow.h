#ifndef P2MainWindow_h
#define P2MainWindow_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

#include <P2Binder.h>
#include <widgets/P2ViewStack.h>
//#include <widgets/P2View.h>
#include <widgets/dialogs/AboutDialog.h>

////////////////////////////////////////////////////////////////////////////////

class P2MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    P2MainWindow( P2Binder &b );

public slots:

    /** \note  A P2MainWindow is bound to the same P2Environment for its entire
        lifetime. */
    void refresh();

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
    void editRename();
    void editSelectAll();
    void editFind();

    void viewCommandLine();
    void viewForward();
    void viewBack();
    void viewNewWindow();
    void viewSplitHorizontal();
    void viewSplitVertical();
    void viewCloseCurrentView();
    /** Turns inactive frame visibility on or off. */
    void viewShowFrames();
    /** Turns label display on or off. */
    void viewShowNames();

    void helpManual();
    void helpAboutPhase2();

private:

    QIcon checkmark;

    P2ViewStack *viewStack;
    //P2View *singleView;

    QAction *editCopyAction, *editCutAction, *editPasteAction;

    // Checkable actions which are bound to the environment.
    QAction *viewShowNamesAction, *viewShowFramesAction;

    // Actions which are bound to the view stack.
    QAction *viewBackAction, *viewForwardAction;

    void createMenusAndToolbar( const P2Environment &env );

    P2Binder *binder;
    P2Environment *environment;

    enum ViewMode { layoutMode, commandLineMode };

    ViewMode viewMode;

    //AboutDialog *aboutDialog;

private slots:

    void backEnabled();
    void backDisabled();
    void forwardEnabled();
    void forwardDisabled();

//    void aboutDialogDestroyed();

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2MainWindow_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
