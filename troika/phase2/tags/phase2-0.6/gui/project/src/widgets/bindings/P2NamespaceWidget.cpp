#include <QtGui>

extern "C"
{
#include <Namespace.h>
}

#include <widgets/bindings/P2NamespaceWidget.h>
#include <widgets/bindings/P2ObjectFrame.h>

////////////////////////////////////////////////////////////////////////////////


P2NamespaceWidget::P2NamespaceWidget( const Object *o, P2Binder *eb )
    : P2BoxArray( QBoxLayout::TopToBottom )
{
    Namespace *ns = ( Namespace* ) object__value( o );

    Array *keys = namespace__keys( ns );
    int size = array__size( keys );
    for ( int i = 0; i < size; i++ )
    {
        char *key = ( char* ) array__get( keys, i );
        Object *child = namespace__lookup_simple( ns, key );
        //P2Widget *w = eb->objectWidget( child );
        P2ObjectFrame *f = new P2ObjectFrame( child, QString( key ), *eb );
        //P2Frame *f = new P2Frame( w, *eb->getEnv() );
        //f->setObjectName( key );
        add( f );
    }
    array__delete( keys );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
