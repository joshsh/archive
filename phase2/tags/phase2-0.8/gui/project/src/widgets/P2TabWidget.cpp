#include <widgets/P2TabWidget.h>

////////////////////////////////////////////////////////////////////////////////


P2TabWidget::P2TabWidget( QWidget *parent )
    : QWidget( parent )
{
    tabWidget = 0;
    singlePage = 0;
}


void P2TabWidget::add( P2Widget *widget, const QString &label )
{
    if ( !singlePage )
    {
        singlePage = widget;
        widget->setParent( this );
        singleLabel = label;
    }

    else
    {
        if ( !tabWidget )
        {
            tabWidget = new QTabWidget( this );
            tabWidget->addTab( singlePage, singleLabel );
        }

        tabWidget->addTab( widget, label );
        tabWidget->setCurrentIndex( tabWidget->indexOf( widget ) );
    }
}


void P2TabWidget::removeCurrent()
{
    if ( tabWidget )
    {
        // Remove the current widget.
        QWidget *w = tabWidget->currentWidget();
        tabWidget->removeTab( tabWidget->currentIndex() );
        delete( w );

        // If only one tab left, eliminate the tab widget.
        if ( tabWidget->count() == 1 )
        {
            singlePage = ( P2Widget* ) tabWidget->currentWidget();
            singleLabel = tabWidget->tabText( tabWidget->currentIndex() );
            tabWidget->removeTab( tabWidget->currentIndex() );

            delete( tabWidget );
            tabWidget = 0;
            singlePage->setParent( this );
        }
    }

    else if ( singlePage )
    {
        singlePage->deleteLater();
        singlePage = 0;
    }
}


int P2TabWidget::count() const
{
    if ( tabWidget )
        return tabWidget->count();
    else if ( singlePage )
        return 1;
    else
        return 0;
}


QSize P2TabWidget::sizeHint() const
{
    if ( !tabWidget )
        return singlePage->sizeHint();
    else
        return tabWidget->sizeHint();
}


void P2TabWidget::refresh( const P2Environment &env )
{
    if ( tabWidget )
    {
        for ( int i = 0; i < tabWidget->count(); i++ )
            ( ( P2Widget* ) tabWidget->widget( i ) )->refresh( env );
    }

    else if ( singlePage )
        singlePage->refresh( env );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
