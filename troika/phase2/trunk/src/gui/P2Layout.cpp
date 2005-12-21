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

    // Frames may be (programmatically) resized, but may not be smaller than
    // minimumSize.
    setSizeConstraint( QLayout::SetDefaultConstraint );

    // Minimum distance of 1 pixel between child widgets.
    setSpacing( 1 );

    // Border padding of 2 pixels around content rectangle.
    setMargin( 2 );

    // The initially empty layout has a minimal content rectangle.
    refreshContentRectangle();
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
    children.append( ( P2LayoutItem* ) item );

    //if ( children.size() == 1 )
    //    contentRectangle = item->geometry();
    //else
    //    contentRectangle = contentRectangle.unite( item->geometry() );

    //cachedSizeHint
    //    = contentRectangle.size() + QSize( 2 * margin(), 2 * margin() );

    adjustGeometry();
}


void P2Layout::addWidget( P2BasicWidget *widget, const QPoint &position )
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
cout << "Befo': (" << cachedSizeHint.width() << ", " << cachedSizeHint.height() << ")" << endl;

    if ( !children.size() )
        contentRectangle = QRect( QPoint( margin(), margin() ), QSize( 0, 0 ) );

    else
    {
        contentRectangle = children.at( 0 )->geometry();
        for ( int i = 1; i < children.size(); i++ )
            contentRectangle = contentRectangle.unite( children.at( i )->geometry() );
    }

    cachedSizeHint = contentRectangle.size() + QSize( 2 * margin(), 2 * margin() );
cout << "Aftuh: (" << cachedSizeHint.width() << ", " << cachedSizeHint.height() << ")" << endl;

    ( ( P2Frame* ) parentWidget() )->setSize( cachedSizeHint );
//( ( P2Frame* ) parentWidget() )->updateGeometry();
//( ( P2Frame* ) parentWidget() )->update();
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

/*
static int abs( int x )
{
    return x < 0 ? -x : x;
}
*/

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
cout << "+ P2Layout[" << (int) this << "]::adjustGeometry()" << endl;

    //QRect beforeGeometry = contentRectangle;


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

cout << "- P2Layout[" << (int) this << "]::adjustGeometry()" << endl;
}


////////////////////////////////////////////////////////////////////////////////


P2LayoutEdge::P2LayoutEdge( P2LayoutItem *src, P2LayoutItem *dest )
{
    int src_pos[2] = { src->geometry().x(),
                       src->geometry().x() + src->geometry().width() };
    int dest_pos[2] = { dest->geometry().x(),
                        dest->geometry().x() + dest->geometry().width() };

    x_combo = 0;
    x_offset = abs( dest_pos[0] - src_pos[0] );

    for ( int i = 1; i < 4; i++ )
    {
        int offset = abs( dest_pos[ i % 2 ] - src_pos[ i / 2 ] );

        if ( offset < x_offset )
        {
            x_combo = i;
            x_offset = offset;
        }
    }

    x_offset = dest_pos[ x_combo % 2 ] - src_pos[ x_combo / 2 ];

    src_pos[0] = src->geometry().y();
    src_pos[1] = src->geometry().y() + src->geometry().height();
    dest_pos[0] = dest->geometry().y();
    dest_pos[1] = dest->geometry().y() + dest->geometry().height();

    y_combo = 0;
    y_offset = abs( dest_pos[0] - src_pos[0] );

    for ( int i = 1; i < 4; i++ )
    {
        int offset = abs( dest_pos[ i % 2 ] - src_pos[ i / 2 ] );

        if ( offset < y_offset )
        {
            y_combo = i;
            y_offset = offset;
        }
    }

    y_offset = dest_pos[ y_combo % 2 ] - src_pos[ y_combo / 2 ];
}


int P2LayoutEdge::compareTo( const P2LayoutEdge &otherEdge )
{
    int minOffset = x_offset < y_offset
        ? x_offset : y_offset;
    int otherMinOffset = otherEdge.x_offset < otherEdge.y_offset
        ? otherEdge.x_offset : otherEdge.y_offset;

    if ( minOffset == otherMinOffset )
        return 0;

    else if ( minOffset < otherMinOffset )
        return -1;

    else
        return 1;
}


void P2LayoutEdge::setDestPosition( P2LayoutItem *src, P2LayoutItem *dest )
{
    int x = src->geometry().x() + x_offset;
    if ( x_combo / 2 )
        x += src->geometry().width();
    if ( x_combo % 2 )
        x -= dest->geometry().width();

    int y = src->geometry().y() + y_offset;
    if ( y_combo / 2 )
        y += src->geometry().height();
    if ( y_combo % 2 )
        y -= dest->geometry().height();

    dest->setGeometry( QRect( QPoint( x, y ), dest->geometry().size() ) );
}

