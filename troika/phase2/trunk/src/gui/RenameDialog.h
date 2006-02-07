#ifndef RenameDialog_h
#define RenameDialog_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2Frame.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

/** \todo  Constrain length + whitespace of text in name field.  Check for name
 *  collision.
 *  \note  It would be more user-friendly to replace this dialog with a modal
 *  text entry field where the frame title normally appears.  The user submits
 *  the new name by pressing enter, and the new name is either silently
 *  accepted, or rejected with an accompanying warning dialog. */
class RenameDialog : public QDialog
{
    Q_OBJECT

public:

    RenameDialog( QWidget *parent, P2Frame *frameToRename );

public slots:

    void accept();
    void done( int r );
    void reject();

private:

    P2Frame *frameToRename;
    QTextEdit *nameField;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // RenameDialog_h
