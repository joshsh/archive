#ifndef P2TextEdit_h
#define P2TextEdit_h

////////////////////////////////////////////////////////////////////////////////

#include "../global.h"
#include "../P2BasicWidget.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

/** \brief  A wrapper for QTextEdit. */
class P2TextEdit : public P2BasicWidget
{

public:

    P2TextEdit( char *text, bool readOnly );

    bool isFrame() { return false; }

    /** \note  Without an overloaded sizeHint, Qt apparently assumes
        QSize( 0, 0 ). */
    QSize sizeHint() const;  // Must be const.

protected:

    bool handleMousePressEvent( QMouseEvent *event, bool childIsBinder );

private:

    class TextEditWrapper : public QTextEdit
    {
    public:
        TextEditWrapper( char *text, P2TextEdit *parent )
            : QTextEdit( text, parent ) { }

        void mousePressEvent( QMouseEvent *event )
        {
            if ( ( ( P2TextEdit* ) parentWidget() )->handleMousePressEvent( event, false ) )
                QTextEdit::mousePressEvent( event );
        }
    };

    TextEditWrapper *textEdit;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2TextEdit_h

