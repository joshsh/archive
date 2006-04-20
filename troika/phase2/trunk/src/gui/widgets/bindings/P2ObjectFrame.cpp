#include <widgets/bindings/P2ObjectFrame.h>

////////////////////////////////////////////////////////////////////////////////


P2ObjectFrame::P2ObjectFrame( Object *o, QString title, P2Binder &b, bool initiallyExpanded )
    : P2Widget()
{
    object = o;
    this->title = title;
    binder = &b;
    expanded = initiallyExpanded;

    stackedWidget = new QStackedWidget( this );

    plusMinus = new P2PlusMinus( this, true );
    plusMinus->setGeometry( QRect( QPoint( 0, 0 ), plusMinus->sizeHint() ) );

    P2Text *titleWidget = new P2Text( title, QColor( COLOR__FRAME__INACTIVE ) );
    stackedWidget->addWidget( titleWidget );

    connect(    plusMinus,  SIGNAL( clickedPlus() ),
                this,       SLOT( expand() ) );
    connect(    plusMinus,  SIGNAL( clickedMinus() ),
                this,       SLOT( contract() ) );

    if ( expanded )
        expand();
    else
        contract();
}


void P2ObjectFrame::setTitle( QString s )
{
    frame->setTitle( s );
}


QSize P2ObjectFrame::sizeHint() const
{
    QSize s = QRect( 0, 0, 0, 0 ).unite( stackedWidget->geometry() ).size();
    if ( !stackedWidget->currentIndex() )
        s += QSize( 11, 0 );

    return s;
    //return stackedWidget->currentWidget()->sizeHint();
}


void P2ObjectFrame::expand()
{
    // The object widget is not created until the frame is expanded for the
    // first time, avoiding unchecked recursion.
    if ( stackedWidget->count() < 2 )
    {
        P2Widget *w = binder->objectWidget( object );
        P2Frame *f = new P2Frame( w, title, *binder->getEnv() );
        stackedWidget->addWidget( f );

        connect(    f,      SIGNAL( resized( QResizeEvent* ) ),
                    this,   SLOT(   update( QResizeEvent* ) ) );
    }

    stackedWidget->setCurrentIndex( 1 );

    int offset = ( ( P2Frame* ) stackedWidget->currentWidget() )->border().y()
        - ( plusMinus->sizeHint().height() / 2 );
    plusMinus->setGeometry( QRect( QPoint( 0, offset), plusMinus->sizeHint() ) );

    stackedWidget->setGeometry( QRect(
        QPoint( 0, 0 ), stackedWidget->sizeHint() ) );

    update( 0 );
}


void P2ObjectFrame::contract()
{
    stackedWidget->setCurrentIndex( 0 );

    int offset = ( stackedWidget->currentWidget()->sizeHint().height()
        - plusMinus->sizeHint().height()  ) / 2;
    plusMinus->setGeometry( QRect( QPoint( 0, offset ), plusMinus->sizeHint() ) );

    stackedWidget->setGeometry( QRect(
        QPoint( plusMinus->sizeHint().width() + FRAME__CONTENTS__SPACING, 0 ),
        stackedWidget->sizeHint() ) );

    update( 0 );
}


void P2ObjectFrame::refresh( const P2Environment &env )
{
    // Refresh the object widget if it exists.
    if ( stackedWidget->count() > 1 )
    {
        P2Widget *w = ( P2Widget* ) stackedWidget->widget( 1 );
        w->refresh( env );
    }
}


void P2ObjectFrame::update( QResizeEvent *e )
{
    stackedWidget->resize( stackedWidget->currentWidget()->sizeHint() );
    resize( sizeHint() );
    emit resized( e );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
