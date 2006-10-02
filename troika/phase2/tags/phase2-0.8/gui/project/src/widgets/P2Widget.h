#ifndef P2Widget_h
#define P2Widget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

#include <P2Environment.h>

////////////////////////////////////////////////////////////////////////////////

class P2Widget : public QWidget
{
    Q_OBJECT

public:

    /** A convenience method. */
    void setCenter( const QPoint &p );

    /** A convenience method. */
    void setPosition( const QPoint &p );

    /** Overloaded method which additionally emits the 'renamed' signal. */
    void setObjectName( QString s );

    bool isActive();
    void setActive( bool a );

signals:

    void resized();
    void renamed();

    void activated();
    void deactivated();

public slots:

    virtual void refresh( const P2Environment &env ) = 0;

protected:

    /** Constructs an empty P2Widget. */
    P2Widget();

    void resizeEvent( QResizeEvent *event );

private:

    bool active;

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Widget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
