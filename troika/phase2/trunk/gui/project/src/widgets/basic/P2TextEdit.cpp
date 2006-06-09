#include <widgets/basic/P2TextEdit.h>


////////////////////////////////////////////////////////////////////////////////


P2TextEdit::P2TextEdit( QString text, bool readOnly, const QColor &color )
        : P2BasicWidget()
{
    // Note: a P2TextEdit object owns its TextEditWrapper as a child widget.
    textEdit = new TextEditWrapper( text, this, color );
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
