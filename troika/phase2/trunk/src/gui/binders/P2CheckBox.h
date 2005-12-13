#ifndef P2CheckBox_h
#define P2CheckBox_h

////////////////////////////////////////////////////////////////////////////////

#include "../global.h"
#include "../P2BasicWidget.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

/** \brief  A wrapper for QCheckbox. */
class P2CheckBox : public P2BasicWidget
{

public:

    P2CheckBox();

    ~P2CheckBox();

    bool isBinder() { return false; }

    QSize sizeHint() const;  // Must be const.

protected:

    bool handleMousePressEvent( QMouseEvent *event, bool childIsBinder );
    //void paintEvent( QPaintEvent *event );

private:

    class CheckBoxWrapper : public QCheckBox {
    public:
        CheckBoxWrapper( P2CheckBox *parent ) : QCheckBox( parent ) { }
        void mousePressEvent( QMouseEvent *event ) {
            if ( ( ( P2CheckBox* ) parentWidget() )->handleMousePressEvent( event, false ) )
                QCheckBox::mousePressEvent( event );
        }
    };

    //QCheckBox *checkbox;
    CheckBoxWrapper *checkbox;
    //CheckBoxWrapper checkbox( 0 );
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2CheckBox_h

