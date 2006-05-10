#ifndef P2TabWidget_h
#define P2TabWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

#include <widgets/P2Widget.h>

////////////////////////////////////////////////////////////////////////////////

class P2TabWidget : public QWidget
{
    Q_OBJECT

public:

    P2TabWidget( QWidget *parent = 0 );

    void add( P2Widget *widget, const QString &label );
    void removeCurrent();

    int count() const;

    QSize sizeHint() const;

public slots:

    void refresh( const P2Environment &env );

private:

    QTabWidget *tabWidget;

    P2Widget *singlePage;
    QString singleLabel;

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2TabWidget_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
