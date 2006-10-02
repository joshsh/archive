#ifndef P2TextEdit_h
#define P2TextEdit_h

////////////////////////////////////////////////////////////////////////////////

#include <widgets/P2BasicWidget.h>

////////////////////////////////////////////////////////////////////////////////

/** \brief  A wrapper for QTextEdit. */
class P2TextEdit : public P2BasicWidget
{

public:

    P2TextEdit( QString text, bool readOnly, const QColor &color );

    /** \note  Without an overloaded sizeHint, Qt apparently assumes
        QSize( 0, 0 ). */
    QSize sizeHint() const;  // Must be const.

private:

    class TextEditWrapper : public QTextEdit
    {
    public:
        TextEditWrapper( QString text, P2TextEdit *parent, const QColor &color )
            : QTextEdit( text, parent )
        {
            // Prevent the 1-pixel foreground-color frame.
            setFrameStyle( QFrame::NoFrame );

            // Prevent border/margin/padding around text and cursor.
            QTextFrame *frame = document()->rootFrame();
            QTextFrameFormat format = frame->frameFormat();
            format.setBorder( 0 );
            format.setMargin( 0 );
            format.setPadding( 0 );
            frame->setFrameFormat( format );

            setLineWrapMode( QTextEdit::WidgetWidth );

            setTextColor( color );

            QFont f = currentFont();
            f.setBold( true );
            setCurrentFont( f );
        }
/*
        void mousePressEvent( QMouseEvent *event )
        {
            if ( ( ( P2TextEdit* ) parentWidget() )->handleMousePressEvent( event, CHILD_NOFRAME ) )
                QTextEdit::mousePressEvent( event );
        }
*/
    };

    TextEditWrapper *textEdit;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2TextEdit_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
