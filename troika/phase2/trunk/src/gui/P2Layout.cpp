#include "P2Layout.h"

    #include <QtGui>

    #include <iostream.h>


////////////////////////////////////////////////////////////////////////////////


P2Layout::P2Layout( QWidget *parent )
    : QLayout( parent )
{
    // Width-1px border.
    setMargin( 1 );

    // No resizing for now.
    setSizeConstraint( QLayout::SetFixedSize );

    // Minimum distance of 1px between child widgets.
    setSpacing( 1 );
}


P2Layout::~P2Layout()
{
    QLayoutItem *l;

    // Delete all child widgets.
    while ( ( l = takeAt( 0 ) ) )
        delete l;
}


////////////////////////////////////////////////////////////////////////////////


void P2Layout::addItem( QLayoutItem *item )
{
    children.append( new P2LayoutItem( item ) );

    QPoint p( 2, 2 );

    for ( int i = 0; i < children.size(); i++ )
    {
        QLayoutItem *item = children.at( i )->item;
        QSize s = item->sizeHint();
        item->setGeometry( QRect( p, s ) );
        s = children.at( i )->size();
        p += QPoint( s.width(), s.height() );
        //children.at( i )->item->setGeometry( QRect( 0, 0, 20, 20 ) );
    }

    findBoundingRectangle();
}


void P2Layout::addWidget( QWidget *widget )
{
    // Create a QLayoutItem to hold a QWidget (a QWidgetItem) and add it.
    addItem( new QWidgetItem( widget ) );
}


int P2Layout::count() const
{
    return children.size();
}


QLayoutItem *P2Layout::itemAt( int index ) const
{
    P2LayoutItem *p2item = children.value(index);
    if (p2item)
        return p2item->item;
    else
        return 0;
}


QLayoutItem *P2Layout::takeAt(int index)
{
    if ( index >= 0 && index < children.size() )
        return children.takeAt(index)->item;
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
    return size;//calculateSize();
}


QSize P2Layout::sizeHint() const
{
    //return QSize( 50, 50 );
    return size;//calculateSize();
}


void P2Layout::findBoundingRectangle()// const
{
    QRect r = children.at( 0 )->item->geometry();
    QPoint p = r.topLeft();
    int minx = p.x(); cout << "minx = " << minx << endl;
    int miny = p.y(); cout << "miny = " << miny << endl;
    p = r.bottomRight();
    int maxx = p.x(); cout << "maxx = " << maxx << endl;
    int maxy = p.y(); cout << "maxy = " << maxy << endl;

    for ( int i = 0; i < children.size(); i++ )
    {
        int x, y;

        r = children.at( i )->item->geometry();
        //r = QRect( children.at( i )->item->geometry().topLeft(), children.at( i )->size() );

        cout << "p = r.topLeft();" << endl;
        p = r.topLeft();
        x = p.x();
        y = p.y();
        if ( x < minx ) {minx = x; cout << "minx = " << minx << endl; }
        if ( y < miny ) {miny = y; cout << "miny = " << miny << endl; }

        cout << "p = r.bottomRight();" << endl;
        p = r.bottomRight();
        x = p.x();
        y = p.y();
        if ( x > maxx ) {maxx = x; cout << "maxx = " << maxx << endl; }
        if ( y > maxy ) {maxy = y; cout << "maxy = " << maxy << endl; }
    }

cout << "children.size() = " << children.size() << endl;
cout << "P2Layout bounding rectangle:" << endl
     << "  Upper left corner at (" << minx << ", " << miny << ")" << endl
     << "  Width = " << maxx - minx << endl
     << "  Height = " << maxy - miny << endl;

    size = QSize( QSize( maxx - minx + 4, maxy - miny + 4 ) );
    //boundingRectangle = QRect( QPoint( minx, miny ), QSize( maxx - minx + 4, maxy - miny + 4 ) );
}


/** \warning  Assumes at least one child widget.
    \note  minimumSize() and sizeHint() are not distinguished for now. */
QSize P2Layout::calculateSize() const
{
    return size;
    //QRect r = boundingRectangle;
    //return QSize( r.width(), r.height() );
}


void P2Layout::setGeometry(const QRect &rect)
{
/*
        ItemWrapper *center = 0;
        int eastWidth = 0;
        int westWidth = 0;
        int northHeight = 0;
        int southHeight = 0;
        int centerHeight = 0;
        int i;
*/
    QLayout::setGeometry(rect);


/*
        for (i = 0; i < children.size(); ++i) {
            ItemWrapper *wrapper = children.at(i);
            QLayoutItem *item = wrapper->item;
            Position position = wrapper->position;

            if (position == North) {
                item->setGeometry(QRect(rect.x(), northHeight, rect.width(),
                                        item->sizeHint().height()));

                northHeight += item->geometry().height() + spacing();
            } else if (position == South) {
                item->setGeometry(QRect(item->geometry().x(),
                                        item->geometry().y(), rect.width(),
                                        item->sizeHint().height()));

                southHeight += item->geometry().height() + spacing();

                item->setGeometry(QRect(rect.x(),
                                  rect.y() + rect.height() - southHeight + spacing(),
                                  item->geometry().width(),
                                  item->geometry().height()));
            } else if (position == Center) {
                center = wrapper;
            }
        }

        centerHeight = rect.height() - northHeight - southHeight;

        for (i = 0; i < children.size(); ++i) {
            ItemWrapper *wrapper = children.at(i);
            QLayoutItem *item = wrapper->item;
            Position position = wrapper->position;

            if (position == West) {
                item->setGeometry(QRect(rect.x() + westWidth, northHeight,
                                        item->sizeHint().width(), centerHeight));

                westWidth += item->geometry().width() + spacing();
            } else if (position == East) {
                item->setGeometry(QRect(item->geometry().x(), item->geometry().y(),
                                        item->sizeHint().width(), centerHeight));

                eastWidth += item->geometry().width() + spacing();

                item->setGeometry(QRect(
                                  rect.x() + rect.width() - eastWidth + spacing(),
                                  northHeight, item->geometry().width(),
                                  item->geometry().height()));
            }
        }

        if (center)
            center->item->setGeometry(QRect(westWidth, northHeight,
                                            rect.width() - eastWidth - westWidth,
                                            centerHeight));
*/
}

