#ifndef AboutDialog_h
#define AboutDialog_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

#include <global.h>

////////////////////////////////////////////////////////////////////////////////

/** An "about Phase2" dialog.
    \note  When the user closes the window by clicking the X, is it deleted or
    merely hidden? */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:

    AboutDialog( QWidget *parent );

};

////////////////////////////////////////////////////////////////////////////////

#endif  // AboutDialog_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
