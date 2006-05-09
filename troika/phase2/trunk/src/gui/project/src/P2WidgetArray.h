#ifndef P2WidgetArray_h
#define P2WidgetArray_h

///////////////////////////////////////////////////////////////////////////////

#include <widgets/P2Widget.h>

///////////////////////////////////////////////////////////////////////////////

class P2WidgetArray : public QObject
{
    Q_OBJECT

public:

    P2WidgetArray();
    ~P2WidgetArray();

    int count() const;
    QLayoutItem *itemAt( int i ) const;
    QLayoutItem *takeAt( int i );

    void add( P2Widget *widget );
    void addItem( QLayoutItem *item );

    /* ! */
    QList<QLayoutItem*> list();

signals:

    /** Emitted whenever a child widget has been resized. */
    void resized();

public slots:

    void refresh( const P2Environment &env );

protected:

    QList<QLayoutItem*> kids;

protected slots:

    void childResizeEvent();

};

///////////////////////////////////////////////////////////////////////////////

#endif  // P2WidgetArray_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
