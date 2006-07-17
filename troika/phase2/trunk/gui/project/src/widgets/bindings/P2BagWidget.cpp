#include <QtGui>

extern "C"
{
#include <collection/Array.h>
}

#include <widgets/bindings/P2BagWidget.h>
#include <widgets/P2ObjectFrame.h>


////////////////////////////////////////////////////////////////////////////////


P2BagWidget::P2BagWidget( const Object *o, P2Binder *eb )
    : P2BoxArray( QBoxLayout::LeftToRight )
{
    Array *a = ( Array* ) object__value( o );
    int size = array__size( a );

    for ( int i = 0; i < size; i++ )
    {
        Object *child = ( Object* ) array__get( a, i );
        P2ObjectFrame *f = new P2ObjectFrame( child, 0, *eb );
        add( f );
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
