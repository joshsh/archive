#include "P2TextEdit.h"


////////////////////////////////////////////////////////////////////////////////


P2TextEdit::P2TextEdit( char *text, bool readOnly )
        : P2BasicWidget()
{
    #ifdef DEBUG
        cout << "P2TextEdit[" <<  (int) this << "]::P2TextEdit()" << endl;
    #endif

    // Note: a P2TextEdit object owns its TextEditWrapper as a child widget.
    textEdit = new TextEditWrapper( text, this );
    textEdit->setReadOnly( readOnly );

    textEdit->setFrameStyle( QFrame::NoFrame );
}


QSize P2TextEdit::sizeHint() const
{
    return textEdit->sizeHint();
    //return QSize( 50, 50 );
    //return textEdit->minimumSize();
}


bool P2TextEdit::handleMousePressEvent( QMouseEvent *event, bool childIsBinder )
{
    return ( ( P2BasicWidget* ) parentWidget() )->handleMousePressEvent( event, childIsBinder );
}

