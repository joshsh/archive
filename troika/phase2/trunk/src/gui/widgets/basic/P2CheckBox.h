#ifndef P2CheckBox_h
#define P2CheckBox_h

////////////////////////////////////////////////////////////////////////////////

#include <widgets/P2BasicWidget.h>

////////////////////////////////////////////////////////////////////////////////

/** \brief  A wrapper for QCheckbox. */
class P2CheckBox : public P2BasicWidget
{

public:

    P2CheckBox();

    /** \note  Without an overloaded sizeHint, Qt apparently assumes
        QSize( 0, 0 ). */
    QSize sizeHint() const;  // Must be const.

private:

    class CheckBoxWrapper : public QCheckBox
    {
    public:
        CheckBoxWrapper( P2CheckBox *parent )
            : QCheckBox( parent ) { }
/*
        void mousePressEvent( QMouseEvent *event )
        {
            if ( ( ( P2CheckBox* ) parentWidget() )->handleMousePressEvent( event, CHILD_NOFRAME ) )
                QCheckBox::mousePressEvent( event );
        }
*/
    };

    CheckBoxWrapper *checkbox;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2CheckBox_h


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
