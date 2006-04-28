#include <widgets/bindings/P2ObjectFrame.h>

////////////////////////////////////////////////////////////////////////////////


P2ObjectFrame::P2ObjectFrame( Object *o, QString title, P2Binder &b, bool initiallyExpanded )
    : P2Widget()
{
    object = o;
    binder = &b;

    contentWidget = 0;
    showContent = initiallyExpanded;

    //setFocusPolicy( Qt::ClickFocus );
    setFocusPolicy( Qt::StrongFocus );

    titleWidget = new P2Title( title );
    titleWidget->setParent( this );

    plusMinus = new P2PlusMinus( this, !showContent );
    showPlusMinus = true;
    connect(    plusMinus,  SIGNAL( clickedPlus() ),
                this,       SLOT( expand() ) );
    connect(    plusMinus,  SIGNAL( clickedMinus() ),
                this,       SLOT( contract() ) );

    // Update if renamed, so as to show the new name in the title.
    setObjectName( title );
    connect(
        this,   SIGNAL( renamed() ),
        this,   SLOT( update2() ) );

    refresh( *b.getEnv() );

    if ( showContent )
        expand();
    else
        contract();
}


void P2ObjectFrame::setTitle( QString s )
{
    if ( s != objectName() )
    {
        setObjectName( s );
        update( 0 );
    }
}


QSize P2ObjectFrame::sizeHint() const
{
    return cachedSizeHint;
}


void P2ObjectFrame::expand()
{
    // The object widget is not created until the frame is expanded for the
    // first time, avoiding unchecked recursion.
    if ( !contentWidget )
    {
        contentWidget = binder->objectWidget( object );
        contentWidget->setParent( this );

        connect(    contentWidget,  SIGNAL( resized( QResizeEvent* ) ),
                    this,           SLOT(   update( QResizeEvent* ) ) );
    }

    showContent = true;
    update( 0 );
}


void P2ObjectFrame::contract()
{
    showContent = false;
    update( 0 );
}


void P2ObjectFrame::refresh( const P2Environment &env )
{
    showTitle = binder->getEnv()->getNameVisibility();
    titleWidget->setVisible( showTitle );

    showBorder = binder->getEnv()->getIdleFrameVisibility();

    // Refresh the object widget if it exists (whether visible or not).
    if ( contentWidget )
        contentWidget->refresh( env );

// Necessary if showTitle has changed...
    //update( 0 );
}


static int
max( int a, int b )
{
    return ( a > b ) ? a : b;
}


void P2ObjectFrame::update( QResizeEvent *e )
{
    e = 0;

    QSize sizeBefore = cachedSizeHint;

    // Overall dimensions.
    int maxx = 0, maxy = 0;

    // Find borderRect and align content widget.
    if ( showContent )
    {
        // Border dimensions.
        int w, h;

        // Border offset.
        int bx = 0, by = 0;

        // Content offset.
        int cx, cy;

        w = contentWidget->sizeHint().width()
            + ( 2 * ( FRAME__BORDER_THICKNESS + FRAME__CONTENTS__PADDING ) );

        if ( showTitle )
        {
            int minWidth = titleWidget->sizeHint().width()
                + ( 2 * ( FRAME__BORDER_THICKNESS + FRAME__LABEL__X_MARGIN + FRAME__LABEL__X_PADDING ) );

            if ( w < minWidth )
                w = minWidth;
        }

        h = contentWidget->sizeHint().height()
            + FRAME__BORDER_THICKNESS + ( 2 * FRAME__CONTENTS__PADDING );

        if ( showPlusMinus )
        {
            bx += plusMinus->sizeHint().width() / 2;
            if ( !showTitle )
            {
                by += plusMinus->sizeHint().height() / 2;
                h += plusMinus->sizeHint().height() / 2;
            }
        }

        if ( showTitle )
        {
            by += titleWidget->sizeHint().height() / 2;
            h += titleWidget->sizeHint().height() / 2;
            cy = titleWidget->sizeHint().height() + FRAME__CONTENTS__PADDING;
        }

        else
        {
            h += FRAME__BORDER_THICKNESS;
            cy = by + FRAME__BORDER_THICKNESS + FRAME__CONTENTS__PADDING;
        }

        switch ( FRAME__CONTENTS__ALIGNMENT )
        {
            case Qt::AlignLeft:

                cx = bx + FRAME__BORDER_THICKNESS + FRAME__CONTENTS__PADDING;
                break;

            case Qt::AlignRight:

                cx = bx + w - ( FRAME__BORDER_THICKNESS + FRAME__CONTENTS__PADDING + contentWidget->sizeHint().width() );
                break;

            case Qt::AlignHCenter:

                cx = bx + ( w - contentWidget->sizeHint().width() ) / 2;
                break;
        }

        borderRect = QRect( bx, by, w, h );
        contentWidget->setPosition( QPoint( cx, cy ) );

        maxx = max( maxx, bx + w + 1 );
        maxy = max( maxy, by + h + 1 );
    }

    // Align title widget.
    // Note: the title is always left-aligned for now.
    if ( showTitle )
    {
        int x = FRAME__LABEL__X_PADDING;

        if ( showPlusMinus )
            x += plusMinus->sizeHint().width();
        else
            x += FRAME__BORDER_THICKNESS + FRAME__LABEL__X_MARGIN;

        titleWidget->setPosition( QPoint( x, 0 ) );

        maxx = max( maxx, x + titleWidget->sizeHint().width() );
        maxy = max( maxy, titleWidget->sizeHint().height() );
    }

    // Align plus/minus widget.
    if ( showPlusMinus )
    {
        int y = ( showTitle )
            ? ( titleWidget->sizeHint().height() - plusMinus->sizeHint().height() ) / 2
            : 0;

        plusMinus->setGeometry( QRect( QPoint( 0, y ), plusMinus->sizeHint() ) );

        maxx = max( maxx, plusMinus->sizeHint().width() );

                              /* Not a typo. */
        maxy = max( maxy, titleWidget->sizeHint().height() );
    }

    cachedSizeHint = QSize( maxx, maxy );

    if ( contentWidget )
        contentWidget->setVisible( showContent );

    if ( cachedSizeHint != sizeBefore )
    {
        resize( cachedSizeHint );
        emit resized( 0 );
    }
}


void P2ObjectFrame::update2()
{
    update( 0 );
}


void P2ObjectFrame::paintEvent( QPaintEvent *event )
{
/*
cout << "paintEvent!" << endl; cout.flush();
cout << "  borderRect.topLeft() = (" << borderRect.x() << ", " << borderRect.y() << ")" << endl;
cout << "  borderRect.size() = (" << borderRect.width() << ", " << borderRect.height() << ")" << endl;
//*/

    // QPaintEvent object is not used.
    event = 0;

    if ( showContent )
    {
        QPainter painter( this );

        QColor c = isActive()
            ? QColor( COLOR__FRAME__ACTIVE )
            : showBorder
                ? QColor( COLOR__FRAME__INACTIVE )
                : Qt::white;

        // Draw border rectangle.
        QPen pen( Qt::NoBrush, 1, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin );
        pen.setColor( c );
        painter.setPen( pen );
        painter.drawRect( borderRect );

        // Draw opaque background rectangle for text.
        if ( showTitle )
        {
            painter.setPen( QColor( COLOR__BACKGROUND ) );
            painter.setBrush( QBrush( QColor( COLOR__BACKGROUND ), Qt::SolidPattern ) );

            QRect r;

            if ( showPlusMinus )
                r = QRect(
                        QPoint( plusMinus->sizeHint().width(), 0 ),
                        QSize(
                            titleWidget->sizeHint().width() + ( 2 * FRAME__LABEL__X_PADDING ),
                            titleWidget->sizeHint().height() ) );
            else
                r = QRect(
                        QPoint( titleWidget->geometry().x(), 0 ),
                        QSize(
                            titleWidget->sizeHint().width() + ( 2 * FRAME__LABEL__X_PADDING ),
                            titleWidget->sizeHint().height() ) );

            painter.drawRect( r );
        }
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
