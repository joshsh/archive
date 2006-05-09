#include <widgets/P2Widget.h>


////////////////////////////////////////////////////////////////////////////////


P2Widget::P2Widget()
    : QWidget( 0, 0 ),
      active( false )
{
    //setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );

    connect(    this,   SIGNAL( activated() ),
                this,   SLOT( update() ) );
    connect(    this,   SIGNAL( deactivated() ),
                this,   SLOT( update() ) );
}


bool P2Widget::isActive()
{
    return active;
}


void P2Widget::setActive( bool a )
{
    bool before = active;
    active = a;
    if ( active != before )
    {
        if ( active )
            emit activated();
        else
            emit deactivated();
    }
}


void P2Widget::setObjectName( QString s )
{
//cout << "renaming..." << endl; cout.flush();
    if ( s.compare( objectName() ) )
    {
        QObject::setObjectName( s );
        emit renamed();
    }
//cout << "...renamed" << endl; cout.flush();
}


void P2Widget::setCenter( const QPoint &p )
{
    setGeometry( geometry().translated( p - geometry().center() ) );
}


void P2Widget::setPosition( const QPoint &p )
{
    setGeometry( geometry().translated( p - geometry().topLeft() ) );
}


void P2Widget::resizeEvent( QResizeEvent *event )
{
    event = 0;
    emit resized();
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
