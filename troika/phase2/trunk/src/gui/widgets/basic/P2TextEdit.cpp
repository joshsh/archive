#include <widgets/basic/P2TextEdit.h>


////////////////////////////////////////////////////////////////////////////////


P2TextEdit::P2TextEdit( char *text, bool readOnly )
        : P2BasicWidget()
{
    #ifdef DEBUG
        cout << indent()
             << "P2TextEdit[" <<  (int) this << "]::P2TextEdit()" << endl;
    #endif

    // Note: a P2TextEdit object owns its TextEditWrapper as a child widget.
    textEdit = new TextEditWrapper( text, this );
    textEdit->setReadOnly( readOnly );

textEdit->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
}


QSize P2TextEdit::sizeHint() const
{
    return textEdit->sizeHint();
    //return QSize( 50, 50 );
    //return textEdit->minimumSize();
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
