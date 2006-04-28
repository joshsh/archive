#ifndef P2PlusMinus_h
#define P2PlusMinus_h

////////////////////////////////////////////////////////////////////////////////

#include <widgets/P2BasicWidget.h>


////////////////////////////////////////////////////////////////////////////////

class P2PlusMinus : public P2BasicWidget
{
    Q_OBJECT

public:

    P2PlusMinus( QWidget *parent, bool initiallyPlus = false );

    QSize sizeHint() const;

signals:

    void clickedPlus();
    void clickedMinus();


protected:


bool handleMousePressEvent( QMouseEvent *event, EventOrigin origin ){ return false; }
bool handleMouseMoveEvent( QMouseEvent *event, EventOrigin origin ){ return false; }







    void paintEvent( QPaintEvent *event );

    void mousePressEvent( QMouseEvent *event );

private:

    bool isPlus;
    int scale;
    QSize cachedSizeHint;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2PlusMinus_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
