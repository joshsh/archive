#include "P2Layout.h"
#include "P2Frame.h"

// Abandon collision resolution after this many iterations.
#define MAX_COLLISIONS  1000


////////////////////////////////////////////////////////////////////////////////


P2Layout::P2Layout( QWidget *parent )
    : QLayout( parent )
{
    #ifdef DEBUG
        cout << indent()
             << "P2Layout[" << (int) this << "]::P2Layout( "
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
    // Delete all layout items.
    for ( int i = 0; i < children.size(); i++ )
        delete children.at( i );
    //while ( ( item = children.takeAt( 0 ) ) )
    //    delete item;
}


////////////////////////////////////////////////////////////////////////////////


void P2Layout::addItem( QLayoutItem *item )
{
    children.append( item );

    generateSpanningTree();

    //if ( children.size() == 1 )
    //    contentRectangle = item->geometry();
    //else
    //    contentRectangle = contentRectangle.unite( item->geometry() );

    //cachedSizeHint
    //    = contentRectangle.size() + QSize( 2 * margin(), 2 * margin() );

    adjustGeometry();
}


// Warning: this is NOT an efficient way to add multiple items at a time.
void P2Layout::addWidget( P2BasicWidget *widget, const QPoint &position )
{

    // Adjust the position of the new item so it does not collide.
    QPoint adjustedPosition = findBestPosition(
        QRect( position, widget->sizeHint() ) );// item->geometry() );
    widget->setGeometry( QRect( adjustedPosition, widget->sizeHint() ) );
    //item->setGeometry( QRect( position, item->sizeHint() ) );

    addItem( new QWidgetItem( widget ) );
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
    cout << indentPlus()
         << "+ P2Layout::refreshContentRectangle()" << endl;

    if ( !children.size() )
        contentRectangle = QRect( QPoint( margin(), margin() ), QSize( 0, 0 ) );

    else
    {
//cout << "#### __p" << endl; cout.flush();

        contentRectangle = children.at( 0 )->geometry();
//cout << "#### __q" << endl; cout.flush();

        for ( int i = 1; i < children.size(); i++ )
            contentRectangle = contentRectangle.unite( children.at( i )->geometry() );
//cout << "#### __r" << endl; cout.flush();

    }
//cout << "#### a" << endl; cout.flush();

    cachedSizeHint = contentRectangle.size() + QSize( 2 * margin(), 2 * margin() );
//cout << "#### b" << endl; cout.flush();

    ( ( P2Frame* ) parentWidget() )->setSize( cachedSizeHint );
//( ( P2Frame* ) parentWidget() )->updateGeometry();
//( ( P2Frame* ) parentWidget() )->update();

    cout << indentMinus()
         << "- P2Layout::refreshContentRectangle()" << endl;
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

void P2Layout::expandChildrenByMargin()
{
    // Expand all layout items by a 1px-wide right and bottom margin.
    for ( int i = 0; i < children.size(); i++ )
    {
        QLayoutItem *item = children.at( i );
        item->setGeometry( QRect(
            item->geometry().topLeft(),
            item->geometry().size() + QSize( spacing(), spacing() ) ) );
    }
}


void P2Layout::shrinkChildrenByMargin()
{
    // Restore original sizes of layout items.
    for ( int i = 0; i < children.size(); i++ )
    {
        QLayoutItem *item = children.at( i );
        item->setGeometry( QRect(
            item->geometry().topLeft(),
            item->geometry().size() - QSize( spacing(), spacing() ) ) );
    }
}


int P2Layout::resolveCollisions()
{
    int size = children.size();

    expandChildrenByMargin();

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

    shrinkChildrenByMargin();

cout << indent() << "collisions found = " << iterations << endl;
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


QPoint P2Layout::findBestPosition( const QRect &rect )
{
//cout << "children.size() = " << children.size() << endl;
//cout << "rect.size() = ( " << rect.width() << ", " << rect.height() << " )" << endl;
//cout << "Original position = ( " << rect.topLeft().x() << ", " << rect.topLeft().y() << " )" << endl;
    QList< Displacement > queue;
    queue.append( Displacement( 0,
        // Original rectangle with a margin.
        QRect( rect.topLeft() - QPoint( spacing(), spacing() ),
               rect.size() + QSize( 2 * spacing(), 2 * spacing() ) ) ) );

    int minCost = INT__INFINITY;
    QRect bestPosition;// = rect.topLeft();

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

//cout << "Adjusted position: (" << bestPosition.topLeft().x() + spacing() << ", " << bestPosition.topLeft().y() + spacing() << " )" << endl;

    // Top-left corner of the "best position" rectangle without the margin.
    return bestPosition.topLeft() + QPoint( spacing(), spacing() );
}


//- Should have an argument -- which item to check for consistency.
void P2Layout::adjustGeometry()
{
cout << indentPlus()
     << "+ P2Layout[" << (int) this << "]::adjustGeometry()" << endl;

    //QRect beforeGeometry = contentRectangle;


    //- If new item collides
    //      resolveCollisions()
    //      refreshContentRectangle();
    //      justifyContents();

    //- If size has changed
    //      notify parent of the change in size (can this be automatic?)
//cout << "#### 1" << endl; cout.flush();
    //if ( root )
    //    root->updateFromSpanningTree();

//cout << "#### 2" << endl; cout.flush();

    applySpanningTree();

    if ( resolveCollisions() ) //|| !root )
//int foo = 1;
        generateSpanningTree();
//cout << "#### 3" << endl; cout.flush();

    // Compensate for any overall displacement which may have occurred.
    refreshContentRectangle();
//cout << "#### 4" << endl; cout.flush();
    justifyContents();

cout << indentMinus()
     << "- P2Layout[" << (int) this << "]::adjustGeometry()" << endl;
}


// Spanning tree ///////////////////////////////////////////////////////////////



void P2Layout::generateSpanningTree()
{
cout << indentPlus()
     << "+ P2Layout[" << (int) this << "]::generateSpanningTree()" << endl;
cout << indent() << "children.size() = " << children.size() << endl;

    tree = P2FreeFormLayoutTree( children );

cout << indentMinus()
     << "- P2Layout[" << (int) this << "]::generateSpanningTree()" << endl;
}


void P2Layout::applySpanningTree()
{
cout << indentPlus()
     << "+ P2Layout[" << (int) this << "]::applySpanningTree()" << endl;
cout << indent() << "children.size() = " << children.size() << endl;

    tree.applyTo( children );

cout << indentMinus()
     << "- P2Layout[" << (int) this << "]::applySpanningTree()" << endl;
}

