#include "P2BasicWidget.h"
#include "P2Layout.h"


////////////////////////////////////////////////////////////////////////////////


P2BasicWidget::P2BasicWidget()
    : P2Widget()
{
    #ifdef DEBUG
        cout << indent()
             << "P2BasicWidget[" << (int) this << "]::P2BasicWidget()" << endl;
    #endif

    //setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );
}


bool P2BasicWidget::isFrame()
{
    return false;
}


void P2BasicWidget::refresh()
{
    // Do nothing.
}

