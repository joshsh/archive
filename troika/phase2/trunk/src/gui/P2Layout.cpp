#include "P2Layout.h"
#include "P2Frame.h"

// Abandon collision resolution after this many iterations.
#define MAX_COLLISIONS  1000


////////////////////////////////////////////////////////////////////////////////


P2Layout::P2Layout( QWidget *parent )
    : QLayout( parent )
{
    #ifdef DEBUG
        cout << "P2Layout[" << (int) this << "]::P2Layout( "
             << (int) parent << " )" << endl;
    #endif

    // No resizing for now.
    setSizeConstraint( QLayout::SetFixedSize );

    // Minimum distance of 1 pixel between child widgets.
    setSpacing( 1 );

    // Border padding of 2 pixels around content rectangle.
    setMargin( 2 );

    // Initially empty layout has a minimal content rectangle.
    contentRectangle = QRect( QPoint( margin(), margin() ), QSize( 0, 0 ) );
    cachedSizeHint = contentRectangle.size() + QSize( 2 * margin(), 2 * margin() );
}


P2Layout::~P2Layout()
{
    //P2LayoutItem *item;

    // Delete all layout items.
    for ( int i = 0; i < children.size(); i++ )
        delete children.at( i );
    //while ( ( item = children.takeAt( 0 ) ) )
    //    delete item;
}


////////////////////////////////////////////////////////////////////////////////


void P2Layout::addItem( QLayoutItem *item )
{
    QRect beforeGeometry = contentRectangle;

    children.append( ( P2LayoutItem* ) item );

    if ( children.size() == 1 )
        contentRectangle = item->geometry();
    else
        contentRectangle = contentRectangle.unite( item->geometry() );

    //cachedSizeHint
    //    = contentRectangle.size() + QSize( 2 * margin(), 2 * margin() );

    adjustGeometry();

    if ( ( contentRectangle != beforeGeometry ) )
cout << "foooooooooooo" << endl;

    if ( ( contentRectangle != beforeGeometry )
      && ( ( P2Frame* ) parentWidget() )->isDependent )
    {
cout << "FOOOOOOOOOOOO" << endl;
        P2Layout *parentLayout
            = ( P2Layout* ) ( ( P2Frame* ) parentWidget() )->layout();
        parentLayout->adjustGeometry();
    }
}


void P2Layout::addWidget( QWidget *widget, QPoint position )
{
    QWidgetItem *item = new QWidgetItem( widget );
    item->setGeometry( QRect( position, item->sizeHint() ) );
    addItem( item );
}


int P2Layout::count() const
{
    return children.size();
}


QLayoutItem *P2Layout::itemAt( int index ) const
{
    return children.value(index);
}


QLayoutItem *P2Layout::takeAt( int index )
{
    if ( index >= 0 && index < children.size() )
        return children.takeAt( index );
    else
        return 0;
}


// Size geometry ///////////////////////////////////////////////////////////////


Qt::Orientations P2Layout::expandingDirections() const
{
    // Really, P2Layouts will not be resizable.
    return Qt::Horizontal | Qt::Vertical;
}


bool P2Layout::hasHeightForWidth() const
{
    // P2Layouts are rigid, with no wrapping behavior.
    return false;
}


QSize P2Layout::minimumSize() const
{
    return cachedSizeHint;
}


QSize P2Layout::sizeHint() const
{
    return cachedSizeHint;
}


void P2Layout::setGeometry( const QRect &rect )
{
    QLayout::setGeometry( rect );

    //...
}


// Coordination ////////////////////////////////////////////////////////////////


void P2Layout::refreshContentRectangle()
{
    if ( !children.size() )
        contentRectangle = QRect( QPoint( margin(), margin() ), QSize( 0, 0 ) );

    else
    {
        contentRectangle = children.at( 0 )->geometry();
        for ( int i = 1; i < children.size(); i++ )
            contentRectangle = contentRectangle.unite( children.at( i )->geometry() );
    }

    cachedSizeHint = contentRectangle.size() + QSize( 2 * margin(), 2 * margin() );
}


void P2Layout::justifyContents()
{
    // Content origin should be at (2, 2).
    int xoffset = contentRectangle.x() - 2;
    int yoffset = contentRectangle.y() - 2;

    if ( xoffset || yoffset )
    {
        QPoint negOffset( -xoffset, -yoffset );

        for ( int i = 0; i < children.size(); i++ )
        {
            QLayoutItem *item = children.at( i );
            item->setGeometry( item->geometry().translated( negOffset ) );
        }

        contentRectangle = contentRectangle.translated( negOffset );
    }
}


int abs( int x )
{
    return x < 0 ? -x : x;
}


int P2Layout::resolveCollisions()
{
    int size = children.size();

    // Expand all layout items by a 1px-wide right and bottom margin.
    for ( int i = 0; i < size; i++ )
    {
        QLayoutItem *item = children.at( i );
        item->setGeometry( QRect(
            item->geometry().topLeft(),
            item->geometry().size() + QSize( 1, 1 ) ) );
    }

    // Find all initial collisions.
    QList<int> collisions;
    for ( int i = 0; i < size; i++ )
    {
        QLayoutItem *a = children.at( i );
        for ( int j = i + 1; j < children.size(); j++ )
        {
            QLayoutItem *b = children.at( j );
            if ( a->geometry().intersects( b->geometry() ) )
                collisions.append( ( i * size ) + j );
        }
    }

    int iterations = 0;

    // While there are any collisions in the queue, resolve them in order.  New
    // collisions may result in the process, and some collisions may be solved
    // "accidentally" before they are encountered in the queue.
    while ( collisions.size() && ( iterations++ < MAX_COLLISIONS ) )
    {
        int index = collisions.takeFirst();
        QLayoutItem *a = children.at( index / size );
        QLayoutItem *b = children.at( index % size );

        QRect rectA = a->geometry();
        QRect rectB = b->geometry();
        QRect intersection = rectA.intersect( rectB );
        if ( !intersection.isEmpty() )
        {
            // Find the offset necessary to resolve the collision by displacing
            // the first frame in each of the the four compass directions.
            int offset[4];
            offset[0] = rectB.y() - ( rectA.y() + rectA.height() ); // north
            offset[1] = ( rectB.y() + rectB.height() ) - rectA.y();  // south
            offset[2] = ( rectB.x() + rectB.width() ) - rectA.x();  // east
            offset[3] = rectB.x() - ( rectA.x() + rectA.width() );  // west

            // Find the direction with the smallest offset.
            int j = 0;
            for ( int i = 1; i < 4; i++ )
                if ( abs( offset[i] ) < abs( offset[j] ) )
                    j = i;

            // Decide how to split up the offset.  Larger widgets take a smaller
            // share, as they are more likely to interfere with other widgets.
            int volumeA = rectA.width() * rectB.height();
            int volumeB = rectB.width() * rectB.height();
            double weight = ( double ) volumeB / ( double ) ( volumeA + volumeB );

            // Displace the frames in opposite directions by a total distance
            // equal to the offset.
            int offsetA = ( int ) ( offset[j] * weight );
            //int offsetA = offset[j] / 2;
            int offsetB = offsetA - offset[j];

            if ( j < 2 )  // North / south.
            {
                a->setGeometry( a->geometry().translated( QPoint( 0, offsetA ) ) );
                b->setGeometry( b->geometry().translated( QPoint( 0, offsetB ) ) );
            }
            else  // East / west.
            {
                a->setGeometry( a->geometry().translated( QPoint( offsetA, 0 ) ) );
                b->setGeometry( b->geometry().translated( QPoint( offsetB, 0 ) ) );
            }

            // Check for new collisions resulting from the displacement.
            for ( int i = 0; i < children.size(); i++ )
            {
                QLayoutItem *item = children.at( i );

                // Note: a and b don't need to be checked against each other,
                // and would cause problems if checked against themselves.
                if ( item != a && item != b )
                {
                    if ( item->geometry().intersects( a->geometry() ) )
                        collisions.append( ( index / size ) * size + i );
                    if ( item->geometry().intersects( b->geometry() ) )
                        collisions.append( ( index % size ) * size + i );
                }
            }
        }
    }

    // Restore original sizes of layout items.
    for ( int i = 0; i < children.size(); i++ )
    {
        QLayoutItem *item = children.at( i );
        item->setGeometry( QRect(
            item->geometry().topLeft(),
            item->geometry().size() - QSize( 1, 1 ) ) );
    }

cout << "collisions found = " << iterations << endl;
    return iterations;
}


//- Should have an argument -- which item to check for consistency.
void P2Layout::adjustGeometry()
{
    //- If new item collides
    //      resolveCollisions()
    //      refreshContentRectangle();
    //      justifyContents();

    //- If size has changed
    //      notify parent of the change in size (can this be automatic?)

    //~ Call this function only when necessary.
    resolveCollisions();

    // Compensate for any overall displacement which may have occurred.
    refreshContentRectangle();
    justifyContents();


    //setGeometry( QRect( contentRectangle.topLeft() - QPoint( 2, 2 ),
    //                    contentRectangle.size() + QSize( 4, 4 ) ) );

    //parentWidget()->updateGeometry();
}

