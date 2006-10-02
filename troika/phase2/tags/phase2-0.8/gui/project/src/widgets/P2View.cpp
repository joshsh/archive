#include <widgets/P2View.h>


///////////////////////////////////////////////////////////////////////////////


P2View::P2View( const Object *o, P2Environment *env )
    : P2Widget(),
      constBinder( new P2Binder( env ) )
{
    //connect(
    //    constBinder,    SIGNAL( objectViewRequest( const Object *o ) ),
    //    this,           SLOT( objectViewRequest( const Object *o ) ) );

    borderWidget = new QWidget();
    borderWidget->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    setFocusPolicy( Qt::NoFocus );

    objectFrame = new P2ObjectFrame( o, constBinder->objectFullName( o ), *constBinder, true );
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
        objectFrame,    SIGNAL( resized() ),
        this,           SLOT(   update() ) );

    connect(
        this,   SIGNAL( resized() ),
        this,   SLOT(   resizeScrollArea() ) );

    connect(
        constBinder,    SIGNAL( hasFocusWidget( bool ) ),
        this,           SIGNAL( hasFocusObject( bool ) ) );

    update();
    refresh( *constBinder->getEnv() );
}


const P2Binder *P2View::binder() const
{
    return constBinder;
}


/*
QSize P2View::sizeHint() const
{
    return borderWidget->size();
cout << "P2View::sizeHint() = (" << borderWidget->size().width() << ", " << borderWidget->size().height() << ")" << endl;
cout << "size() = (" << size().width() << ", " << size().height() << ")" << endl;
}
*/


P2Widget *P2View::focusWidget()
{
    return constBinder->getFocusWidget();
}


const Object *P2View::focusObject()
{
                       /* !            */
    P2ObjectFrame *f = ( P2ObjectFrame* ) constBinder->getFocusWidget();

    if ( f )
        return f->object();

    else
        return 0;
}


void P2View::refresh( const P2Environment &env )
{
    objectFrame->refresh( env );
}


void P2View::resizeScrollArea()
{
    scrollArea->resize( size() );
}


void P2View::update()
{
    //- Center the child widget in x if it is small enough.
    //- Center the child widget in y if it is small enough.

    borderWidget->resize( objectFrame->sizeHint()
        + QSize( 2 * WINDOW__CONTENTS__PADDING, 2 * WINDOW__CONTENTS__PADDING ) );

    QWidget::update();

    resizeScrollArea();
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
