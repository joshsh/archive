#include "P2CheckBox.h"


////////////////////////////////////////////////////////////////////////////////


P2CheckBox::P2CheckBox()
        : P2BasicWidget()
{
    #ifdef DEBUG
        cout << "P2CheckBox[" <<  (int) this << "]::P2CheckBox()" << endl;
    #endif

    //setFixedSize( 20, 20 );

    // Note: a P2CheckBox object owns its CheckBoxWrapper as a child widget.
    checkbox = new CheckBoxWrapper( this );
    //new CheckBoxWrapper( this );
}


QSize P2CheckBox::sizeHint() const
{
    return checkbox->sizeHint();
}


bool P2CheckBox::handleMousePressEvent( QMouseEvent *event, bool childIsBinder )
{
    //checkbox->mousePressEvent( event );
    return ( ( P2BasicWidget* ) parentWidget() )->handleMousePressEvent( event, childIsBinder );
}

