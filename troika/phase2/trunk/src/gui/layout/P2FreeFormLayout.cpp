#include "P2FreeFormLayout.h"

// Abandon collision resolution after this many iterations.
#define MAX_COLLISIONS  1000


////////////////////////////////////////////////////////////////////////////////


P2FreeFormLayout::P2FreeFormLayout( QWidget *parent )
    : P2Layout( parent )
{
    #ifdef DEBUG__LAYOUT
        cout << indent()
             << "P2FreeFormLayout[" << (int) this << "]::P2FreeFormLayout( "
             << (int) parent << " )" << endl;
    #endif

    // The initially empty layout has a minimal content rectangle.
    setContentOffset( QPoint( 0, 0 ) );

    generateSpanningTree();

    //adjustGeometry();
    //justifyContents();
}


////////////////////////////////////////////////////////////////////////////////


// Warning: this is NOT an efficient way to add multiple items at a time.
void P2FreeFormLayout::add( P2Widget *widget, const QPoint &position )
{
    // Adjust the position of the new item so it does not collide.
    QPoint adjustedPosition = findBestPosition(
        QRect( position, widget->sizeHint() ) );

    widget->setGeometry( QRect( adjustedPosition, widget->sizeHint() ) );

    addWidget( widget );
}


// Size geometry ///////////////////////////////////////////////////////////////


Qt::Orientations P2FreeFormLayout::expandingDirections() const
{
    // Really, P2FreeFormLayouts will not be resizable.
    return Qt::Horizontal | Qt::Vertical;
}


bool P2FreeFormLayout::hasHeightForWidth() const
{
    // P2FreeFormLayouts are rigid, with no wrapping behavior.
    return false;
}


// Coordination ////////////////////////////////////////////////////////////////


void P2FreeFormLayout::justifyContents()
{
    if ( !children.size() )
        contentRectangle = QRect(
            QPoint( 0, 0 ), QSize( 0, 0 ) );

    else
    {
        contentRectangle = children.at( 0 )->geometry();

        for ( int i = 1; i < children.size(); i++ )
            contentRectangle = contentRectangle.unite( children.at( i )->geometry() );
    }

    cachedSizeHint = contentRectangle.size()
        + QSize( contentOffset.x(), contentOffset.y() )
        + QSize( margin(), margin() );

    QPoint actualOffset = contentOffset;
    if ( ( receivedMinimumSize.width() > 0 )
      && ( contentRectangle.width() + contentOffset.x() + margin() < receivedMinimumSize.width() )
      && ( FRAME__CONTENTS__ALIGNMENT != Qt::AlignLeft ) )
    {
        int xoffset;

        switch ( FRAME__CONTENTS__ALIGNMENT )
        {
            case Qt::AlignRight:

                xoffset = receivedMinimumSize.width() - ( margin() + contentRectangle.width() );
                break;

            case Qt::AlignHCenter:

                xoffset = ( receivedMinimumSize.width() - contentRectangle.width() ) / 2;
                break;
        }

        actualOffset.setX( xoffset );

        cachedSizeHint.setWidth( receivedMinimumSize.width() );
    }
    // Note: no need for special vertical alignment at this stage.

    // Content origin should be at (2, 2).
    int xoffset = contentRectangle.x() - actualOffset.x();
    int yoffset = contentRectangle.y() - actualOffset.y();

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

    emit resized();
}


bool P2FreeFormLayout::tooClose( const QRect &a, const QRect &b )
{
    //~ Replace this with something more efficient.
    QSize padding( spacing(), spacing() );
    QRect a2 = QRect( a.topLeft(), a.size() + padding );
    QRect b2 = QRect( b.topLeft(), b.size() + padding );
    return a2.intersects( b2 );
}


int P2FreeFormLayout::resolveCollisions()
{
    int size = children.size();
    QSize padding( spacing(), spacing() );

    //expandChildrenByMargin();

    // Find all initial collisions.
    QList<int> collisions;
    for ( int i = 0; i < size; i++ )
    {
        QLayoutItem *a = children.at( i );
        for ( int j = i + 1; j < children.size(); j++ )
        {
            QLayoutItem *b = children.at( j );
            if ( tooClose( a->geometry(), b->geometry() ) )
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

        QRect rectA = QRect( a->geometry().topLeft(),
            a->geometry().size() + padding );
        QRect rectB = QRect( b->geometry().topLeft(),
            b->geometry().size() + padding );

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
                    if ( tooClose( item->geometry(), a->geometry() ) )
                        collisions.append( ( index / size ) * size + i );
                    if ( tooClose( item->geometry(), b->geometry() ) )
                        collisions.append( ( index % size ) * size + i );
                }
            }
        }
    }

    //shrinkChildrenByMargin();

    #ifdef DEBUG__LAYOUT
        cout << indent()
             << "collisions found = " << iterations << endl;
    #endif

    return iterations;
}


struct Displacement
{
    Displacement( int c, QRect p )
    {
        cost = c;
        position = p;
    }
    QRect position;
    int cost;
};


QPoint P2FreeFormLayout::findBestPosition( const QRect &rect )
{
    QList< Displacement > queue;
    queue.append( Displacement( 0,
        // Original rectangle with a margin.
        QRect( rect.topLeft() - QPoint( spacing(), spacing() ),
               rect.size() + QSize( 2 * spacing(), 2 * spacing() ) ) ) );

    int minCost = INT__INFINITY;
    QRect bestPosition;

    while ( ( queue.size() ) )
    {
        Displacement d = queue.takeFirst();
        int cost = d.cost;
        QRect r = d.position;

        // Only consider this position if there is a cost advantage.
        if ( cost < minCost )
        {
            bool doesIntersect = false;

            for ( int i = 0; i < children.size(); i++ )
            {
                QRect r2 = children.at( i )->geometry();

                // Check for collision.
                if ( r.intersects( r2 ) )
                {
                    doesIntersect = true;
                    int offset;

                    // Displace the rectangle in each of the four compass
                    // directions to resolve the immediate collision.
                    offset = r2.x() + r2.width() - r.x();
                    queue.append( Displacement( cost + offset,
                        r.translated( QPoint( offset, 0 ) ) ) );
                    offset = r2.x() - ( r.x() + r.width() ) ;
                    queue.append( Displacement( cost + offset,
                        r.translated( QPoint( offset, 0 ) ) ) );
                    offset = r2.y() + r2.height() - r.y();
                    queue.append( Displacement( cost + offset,
                        r.translated( QPoint( 0, offset ) ) ) );
                    offset = r2.y() - ( r.y() + r.height() ) ;
                    queue.append( Displacement( cost + offset,
                        r.translated( QPoint( 0, offset ) ) ) );
                }
            }

            if ( !doesIntersect )
            {
                minCost = cost;
                bestPosition = r;
            }
        }
    }

    // Top-left corner of the "best position" rectangle without the margin.
    return bestPosition.topLeft() + QPoint( spacing(), spacing() );
}


//- Should have an argument -- which item to check for consistency.
void P2FreeFormLayout::adjustGeometry()
{
    #ifdef DEBUG__LAYOUT
        cout << indentPlus()
             << "+ P2FreeFormLayout[" << (int) this << "]::adjustGeometry()" << endl;
    #endif

    //QRect beforeGeometry = contentRectangle;

    applySpanningTree();

    if ( resolveCollisions() )
        generateSpanningTree();

    // Compensate for any overall displacement which may have occurred.
    justifyContents();

    #ifdef DEBUG__LAYOUT
        cout << indentMinus()
             << "- P2FreeFormLayout[" << (int) this << "]::adjustGeometry()" << endl;
    #endif
}


// Spanning tree ///////////////////////////////////////////////////////////////


void P2FreeFormLayout::generateSpanningTree()
{
    #ifdef DEBUG__LAYOUT
        cout << indentPlus()
             << "+ P2FreeFormLayout[" << (int) this << "]::generateSpanningTree()" << endl;
        cout << indent() << "children.size() = " << children.size() << endl;
        showChildren();
    #endif

    tree = P2FreeFormLayoutTree( children );

    #ifdef DEBUG__LAYOUT
        cout << indentMinus()
             << "- P2FreeFormLayout[" << (int) this << "]::generateSpanningTree()" << endl;
    #endif
}


void P2FreeFormLayout::applySpanningTree()
{
    #ifdef DEBUG__LAYOUT
        cout << indentPlus()
             << "+ P2FreeFormLayout[" << (int) this << "]::applySpanningTree()" << endl;
        cout << indent() << "children.size() = " << children.size() << endl;
    #endif

    tree.applyTo( children );

    #ifdef DEBUG__LAYOUT
        cout << indentMinus()
             << "- P2FreeFormLayout[" << (int) this << "]::applySpanningTree()" << endl;
    #endif
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
