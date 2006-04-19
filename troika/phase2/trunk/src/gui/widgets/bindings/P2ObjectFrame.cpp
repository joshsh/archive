#include <widgets/bindings/P2ObjectFrame.h>

////////////////////////////////////////////////////////////////////////////////


P2ObjectFrame::P2ObjectFrame( Object *o, QString title, P2EnvironmentBinder *eb, bool initiallyExpanded )
    : P2Widget()
{
    object = o;
    this->title = title;
    binder = eb;
    expanded = initiallyExpanded;

    stackedWidget = new QStackedWidget( this );

    P2Text *titleWidget = new P2Text( title, QColor( 0xBF, 0xBF, 0xFF, 0xFF ) );
    stackedWidget->addWidget( titleWidget );

    plusMinus = new P2PlusMinus( this, true );
    plusMinus->setGeometry( QRect( QPoint( 0, 0 ), plusMinus->sizeHint() ) );

    connect(    plusMinus,  SIGNAL( clickedPlus() ),
                this,       SLOT( expand() ) );
    connect(    plusMinus,  SIGNAL( clickedMinus() ),
                this,       SLOT( contract() ) );

    if ( expanded )
        expand();
}


QSize P2ObjectFrame::sizeHint() const
{
cout << "P2ObjectFrame::sizeHint" << endl;
    return stackedWidget->currentWidget()->sizeHint();
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

stackedWidget->resize( stackedWidget->currentWidget()->sizeHint() );
resize( sizeHint() );
QWidget::update();
//refresh( *binder->getEnv() );
    emit resized( 0 );
}


void P2ObjectFrame::contract()
{
    stackedWidget->setCurrentIndex( 0 );

stackedWidget->resize( stackedWidget->currentWidget()->sizeHint() );
resize( sizeHint() );
    emit resized( 0 );
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
resize( sizeHint() );
    emit resized( e );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
