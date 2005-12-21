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

    bool isFrame() { return false; }

    /** \note  Without an overloaded sizeHint, Qt apparently assumes
        QSize( 0, 0 ). */
    QSize sizeHint() const;  // Must be const.

protected:

    bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin );
    bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ) { return false; }

    //void paintEvent( QPaintEvent *event );

private:

    class CheckBoxWrapper : public QCheckBox
    {
    public:
        CheckBoxWrapper( P2CheckBox *parent )
            : QCheckBox( parent ) { }

        void mousePressEvent( QMouseEvent *event )
        {
            if ( ( ( P2CheckBox* ) parentWidget() )->handleMousePressEvent( event, CHILD_NOFRAME ) )
                QCheckBox::mousePressEvent( event );
        }
    };

    CheckBoxWrapper *checkbox;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2CheckBox_h

