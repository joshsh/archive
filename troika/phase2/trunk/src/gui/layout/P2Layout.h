#ifndef P2Layout_h
#define P2Layout_h

///////////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "P2Widget.h"
#include "P2Environment.h"

#include <QtGui>

///////////////////////////////////////////////////////////////////////////////

class P2Layout : public QLayout
{
    Q_OBJECT

public:

    P2Layout( QWidget *parent );

    ~P2Layout();

    void addItem( QLayoutItem *item );

    int count() const;
    QLayoutItem *itemAt( int index ) const;
    QLayoutItem *takeAt( int index );

    virtual void adjustGeometry() = 0;

public slots:

    virtual void childResizeEvent( QResizeEvent *event ) = 0;

signals:

    void resized();

protected:

    /** Show names and types of children.  A debugging method. */
    void showChildren() const;

    /** An auxiliary QList for quick iteration by index over this P2Layout's
        child widgets. */
    QList<QLayoutItem *> children;

};

///////////////////////////////////////////////////////////////////////////////

#endif  // P2Layout_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
