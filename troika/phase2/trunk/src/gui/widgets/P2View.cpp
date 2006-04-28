#include <widgets/P2View.h>


///////////////////////////////////////////////////////////////////////////////


P2View::P2View( Object *o, P2Binder &eb )
    : P2Widget()
{
    binder = &eb;

    borderWidget = new QWidget();
    borderWidget->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    setFocusPolicy( Qt::NoFocus );

    objectFrame = new P2ObjectFrame( o, "root", eb, true );
    objectFrame->setParent( borderWidget );

    scrollArea = new QScrollArea( this );
    // No frame around the scroll area, thanks.
    scrollArea->setFrameStyle( QFrame::NoFrame );
    scrollArea->setFocusPolicy( Qt::NoFocus );

//scrollArea->setBackgroundRole(QPalette::Dark);
//scrollArea->setWidgetResizable( true );
//scrollArea->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    scrollArea->setWidget( borderWidget );
    objectFrame->setGeometry( QRect(
        QPoint( WINDOW__CONTENTS__PADDING, WINDOW__CONTENTS__PADDING ),
        objectFrame->sizeHint()
            + QSize( 2 * WINDOW__CONTENTS__PADDING, 2 * WINDOW__CONTENTS__PADDING ) ) );

    connect(
        objectFrame,    SIGNAL( resized( QResizeEvent* ) ),
        this,           SLOT(   update( QResizeEvent* ) ) );

    connect(
        this,   SIGNAL( resized( QResizeEvent* ) ),
        this,   SLOT(   resizeScrollArea( QResizeEvent* ) ) );

    update( 0 );
    refresh( *eb.getEnv() );
}

/*
QSize P2View::sizeHint() const
{
    return borderWidget->size();
cout << "P2View::sizeHint() = (" << borderWidget->size().width() << ", " << borderWidget->size().height() << ")" << endl;
cout << "size() = (" << size().width() << ", " << size().height() << ")" << endl;
}
*/

P2ObjectFrame *P2View::focusFrame()
{
    return objectFrame;
}


void P2View::refresh( const P2Environment &env )
{
    objectFrame->refresh( env );
}


void P2View::resizeScrollArea( QResizeEvent *event )
{
    scrollArea->resize( size() );
}


void P2View::update( QResizeEvent *event )
{
    //- Center the child widget in x if it is small enough.
    //- Center the child widget in y if it is small enough.

    borderWidget->resize( objectFrame->sizeHint()
        + QSize( 2 * WINDOW__CONTENTS__PADDING, 2 * WINDOW__CONTENTS__PADDING ) );

    QWidget::update();

    resizeScrollArea( 0 );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
